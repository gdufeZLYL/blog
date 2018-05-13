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
上面介绍的AOF重写程序aof_rewrite函数可以很好地完成创建一个新AOF文件的任务,但是,因为这个函数会进行大量的写入操作,所以调用这个函数的线程将被长时间阻塞,因为Redis服务器使用单个线程来处理命令请求,所以如果由服务器直接调用aof_rewrite函数的话,那么在重写AOF文件期间,服务期将无法处理客户端发来的命令请求.

很明显,作为一种辅佐性的维护手段,Redis不希望AOF重写造成服务器无法处理请求,所以Redis决定将AOF重写程序放到子进程里执行,这样做可以同时达到两个目的:
* 子进程进行AOF重写期间,服务器进程(父进程)可以继续处理命令请求.
* 子进程带有服务器进程的数据副本,使用子进程而不是线程,可以在避免使用锁的情况下,保证数据的安全性.

不过,使用子进程也有一个问题需要解决,因为子进程在进行AOF重写期间,服务器进程还需要继续处理命令请求,而新的命令可能会对现有的数据库状态进行修改,从而使得服务器当前的数据库状态和重写后的AOF文件所保存的数据库状态不一致.

例子: 数据不一致

![数据不一致](https://github.com/gdufeZLYL/blog/blob/master/images/20180513234025.png)

为了解决这种数据不一致问题,Redis服务器设置了一个AOF重写缓冲区,这个缓冲区在服务器创建子进程之后开始使用,当Redis服务器执行完一个写命令之后,它会同时将这个写命令发送给AOF缓冲区和AOF重写缓冲区,如图11-4所示:

![服务器同时将命令发送给AOF文件和AOF重写缓冲区](https://github.com/gdufeZLYL/blog/blob/master/images/20180513234340.png)

这也就是说,在子进程执行AOF重写期间,服务器进程需要执行以下三个工作:
1. 执行客户端发来的命令.
2. 将执行后的写命令追加到AOF缓冲区.
3. 将执行后的写命令追加到AOF重写缓冲区.

这样一来可以保证:
* AOF缓冲区的内容会定期被写入和同步到AOF文件,对现有AOF文件的处理工作如常进行.
* 从创建子进程开始,服务器执行的所有写命令都会被记录到AOF重写缓冲区里面.

当子进程完成AOF重写工作之后,它会向父进程发送一个信号,父进程在接到该信号之后,会调用一个信号处理函数,并执行以下工作:
1. 将AOF重写缓冲区中的所有内容写入到新AOF文件中,这时新AOF文件所保存的数据库状态将和服务器当前的数据库状态一致.
2. 对新的AOF文件进行改名,原子地覆盖现有的AOF文件,完成新旧两个AOF文件的替换.

在整个AOF后台重写过程中,只有信号处理函数执行时会对服务器进程(父进程)造成阻塞,在其他时候,AOF后台重写都不会阻塞父进程,这将AOF重写对服务器性能造成的影响降到了最低.

例子: AOF文件后台重写的执行过程

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180514000404.png)
