# Redis的过期键删除策略
Redis服务器实际使用的是惰性删除和定期删除两种策略:通过配合使用这两种删除策略,服务器可以很好地在合理使用CPU时间和避免内存空间之间取得平衡.

# 惰性删除策略的实现
过期键的惰性删除策略由db.c/expireIfNeeded函数实现,所有读写数据库的Redis命令在执行之前都会调用expireIfNeeded函数对输入键进行检查:
* 如果输入键已经过期,那么expireIfNeeded函数将输入键从数据库中删除.
* 如果输入键未过期,那么expireIfNeeded函数不做动作.

命令调用expireIfNeeded函数的过程:

![命令调用expireIfNeeded来删除过期键](https://github.com/gdufeZLYL/blog/blob/master/images/20180512093443.png)

expireIfNeeded函数就像一个过滤器,它可以在命令真正执行之前,过滤掉过期的输入键,从而避免命令接触到过期键.

### 两种情况:键存在和键不存在
因为每个被访问的键都可能因为过期而被expireIfNeeded函数删除,所以每个命令的实现函数都必须能同时处理键存在以及键不存在这两种情况:
* 当键存在时,命令按照键存在的情况执行.
* 当键不存在或者键因为过期而被expireIfNeeded函数删除时,命令按照键不存在的情况执行.

例子:GET命令的执行过程

![GET命令的执行过程](https://github.com/gdufeZLYL/blog/blob/master/images/20180512094425.png)

# 定期删除策略的实现
过期键的定期删除策略由redis.c/activeExpireCycle函数实现,每当Redis的服务器周期性操作redis.c/serverCron函数执行时,activeExpireCycle函数就会被调用,它在规定的时间内,分多次遍历服务器中的各个数据库,从数据库的expires字典中随机检查一部分键的过期时间,并删除其中的过期键.
伪代码:
```python
# 默认每次检查的数据库数量
DEFAULT_DB_NUMBERS = 16

# 默认每个数据库检查的键数量
DEFAULT_KEY_NUMBERS = 20

# 全局变量,记录检查进度
current_db = 0

def activeExpireCycle():

    # 初始化要检查的数据库数量
    # 如果服务器的数据库数量比DEFAULT_DB_NUMBERS药效
    # 那么以服务器的数据库数量为准
    if server.dbnum < DEFAULT_DB_NUMBERS:
        db_numbers = server.dbnum
    else:
        db_number = DEFAULT_DB_NUMBERS

    # 遍历各个数据库
    for i in range(db_numbers):

        # 如果current_db的值等于服务器的数据库数量
        # 这表示检查程序已经遍历了服务器的所有数据库一次
        # 将current_db重置为0,开始新的一轮遍历
        if current_db == server.dbnum:
            current_db = 0
        
        # 获取当前要处理的数据库
        redisDb = server.db[current_db]

        # 将数据库索引增1,指向下一个要处理的数据库
        current_db += 1

        # 检查数据库键
        for j in range(DEFAULT_KEY_NUMBERS):

            # 如果数据库中没有一个键带有过期时间,那么跳过这个数据库
            if redisDb.expires.size() == 0: break

            # 随机获取一个带有过期时间的键
            key_with_ttl = redisDb.expires.get_random_key()

            # 检查键是否过期,如果过期就删除它
            if is_expired(key_with_ttl):
                delete_key(key_with_ttl)
            
            # 已达到时间上限,停止处理
            if reach_time_limit(): return
```
activeExpireCycle函数的工作模式:

![activeExpireCycle函数的工作模式](https://github.com/gdufeZLYL/blog/blob/master/images/20180512101026.png)