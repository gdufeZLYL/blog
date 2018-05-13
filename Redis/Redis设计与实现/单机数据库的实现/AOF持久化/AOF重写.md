# AOF重写的原因
> 因为AOF持久化是通过保存被执行的写命令来记录数据库状态的,所以随着服务器运行时间的流逝,AOF文件中的内容会越来越多,文件的体积也会越来越大,如果不加以控制的话,体积过大的AOF文件很可能对Redis服务器、甚至整个宿主计算机造成影响,并且AOF文件的体积越大,使用AOF文件来进行数据还原所需的时间就越多.

例子:
客户端执行了一下命令:
```
redis> RPUSH list "A" "B"   // ["A","B"]
(integer) 2

redis> RPUSH list "C"   // ["A", "B", "C"]
(integer) 3

redis> RPUSH list "D" "E"   // ["A", "B", "C", "D", "E"]
(integer) 5

redis> LPOP list    // ["B", "C", "D", "E"]
"A"

redis> LPOP list    // ["C", "D", "E"]
"B"

redis> RPUSH list "F" "G"   // ["C", "D", "E", "F", "G"]
(integer) 5
```
分析:

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180513204332.png)

# AOF文件重写的实现
> 虽然Redis将生成新AOF文件替换旧AOF文件的功能命名为"AOF文件重写",但实际上,AOF文件重写并不需要对现有的AOF文件进行任何读取、分析或者写入操作,这个功能是通过读取服务器当前的数据库状态来实现的.

例子:
如果服务器对list键执行了以下命令:
```
redis> RPUSH list "A" "B"   // ["A","B"]
(integer) 2

redis> RPUSH list "C"   // ["A", "B", "C"]
(integer) 3

redis> RPUSH list "D" "E"   // ["A", "B", "C", "D", "E"]
(integer) 5

redis> LPOP list    // ["B", "C", "D", "E"]
"A"

redis> LPOP list    // ["C", "D", "E"]
"B"

redis> RPUSH list "F" "G"   // ["C", "D", "E", "F", "G"]
(integer) 5
```
分析:

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180513203508.png)

例子:
如果服务器对animals键执行了以下命令:
```
redis> SADD animals "Cat"   // {"Cat"}
(integer) 1

redis> SADD animals "Dog" "Panda" "Tiger"   // {"Cat", "Dog", "Panda", "Tiger"}
(integer) 3

redis> SREM animals "Cat"   // {"Dog", "Panda", "Tiger"}
(integer) 1

redis> SADD animals "Lion" "Cat"    // {"Dog", "Panda", "Tiger", "Lion", "Cat"}
(integer) 2
```
分析:

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180513203930.png)

AOF重写功能实现原理
> 首先从数据库中读取键现在的值,然后用一条命令去记录键值对,代替之前记录这个键值对的多条命令.

重写过程伪代码:
```python
def aof_rewrite(new_aof_file_name):
    # 创建新AOF文件
    f = create_file(new_aof_file_name)

    # 遍历数据库
    for db in redisServer.db:
        # 忽略空数据库
        if db.is_empty(): continue

        # 写入SELECT命令,指定数据库号码
        f.write_command("SELECT" + db.id)

        # 遍历数据库中的所有键
        for key in db:
            #忽略已过期的键
            if key.is_expired(): continue

            # 根据键的类型对键进行重写
            if key.type == String:
                rewrite_string(key)
            elif key.type == List:
                rewrite_list(key)
            elif key.type == Hash:
                rewrite_hash(key)
            elif key.type == Set:
                rewrite_set(key)
            elif key.type == SortedSet:
                rewrite_sorted_set(key)
            
            # 如果键带有过期时间,那么过期时间也要被重写
            if key.have_expire_time():
                rewrite_expire_time(key)
    
    # 写入完毕,关闭文件
    f.close()

def rewrite_string(key):
    # 使用GET命令获取字符串键的值
    value = GET(key)

    # 使用SET命令重写字符串键
    f.write_command(SET, key, value)

def rewrite_list(key):
    # 使用LRANGE命令获取列表键包含的所有元素
    item1, item2, ..., itemN = LRANGE(key, 0, -1)

    # 使用RPUSH命令重写列表键
    f.write_command(RPUSH, key, item1, item2, ..., itemN)

def rewrite_hash(key):
    # 使用HGETALL命令获取哈希键包含的所有键值对
    field1, value1, field2, value2, ..., fieldN, valueN = HGETALL(key)

    # 使用HMSET命令重写哈希键
    f.write_command(HMSET, key, field1, value1, field2, value2, ..., fieldN, valueN)

def rewrite_sorted_set(key):
    # 使用ZRANGE命令获取有序集合键包含的所有元素
    member1, score1, member2, score2, ..., memberN, scoreN = ZRANGE(key, 0, -1, "WITHSCORES")

    # 使用ZADD命令重写有序集合键
    f.write_command(ZADD, key, score1, member1, score2, member2, ..., scoreN, memberN)

def rewrite_expire_time(key):
    # 获取毫秒精度的键过期时间戳
    timestamp = get_expire_time_in_unixstamp(key)

    # 使用PEXPIREAT命令重写键的过期时间
    f.write_command(PEXPIREAT, key, timestamp)
```
例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180513220206.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180513223354.png)

注意: 重写程序可能使用多态命令,而不单单使用一条命令

![注意](https://github.com/gdufeZLYL/blog/blob/master/images/20180513224449.png)

# AOF后台重写
