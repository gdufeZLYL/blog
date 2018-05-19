# WATCH命令
> WATCH命令是一个乐观锁,它可以在EXEC命令执行之前,监视任意数量的数据库键,并在EXEC命令执行时,检查被监视的键是否至少有一个已经被修改过了,如果是的话,服务器将拒绝执行食物,并向客户端返回代表事务执行失败的空回复.

一个事务执行失败的例子:
```
redis> WATCH "name"
OK

redis> MULTI
OK

redis> SET "name" "peter"
QUEUED

redis> EXEC
(nil)
```
表19-1展示了上面的例子是如何失败的:

![上面的例子是如何失败的](https://github.com/gdufeZLYL/blog/blob/master/images/20180519084151.png)

# 使用WATCH命令监视数据库键
每个Redis数据库都保存着一个watched_keys字典,这个字典的键是某个被WATCH命令监视的数据库键,而字典的值则是一个链表,链表中记录了所有监视相应数据库键的客户端:
```c++
typedef struct redisDb {
    // ...

    // 正在被WATCH命令监视的键
    dict *watched_keys;

    // ...
} redisDb;
```
通过watched_keys字典,服务器可以清楚地知道哪些数据库键正在被监视,以及哪些客户端正在监视这些数据库键.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519085206.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519085220.png)

# 监视机制的触发
所有对数据库进行修改的命令,比如SET、LPUSH、SADD、ZREM、DEL、FLUSHDB等等,在执行之后都会调用multi.c/touchWatchKey函数对watched_keys字典进行检查,查看是否有客户端正在监视刚刚被命令修改过的数据库键,如果有的话,那么touchWatchKey函数会将监视被修改键的客户端的REDIS_DIRTY_CAS标识打开,表示该客户端的事务安全性已经被破坏.

touchWatchKey函数的定义可以用以下伪代码来描述:
```python
def touchWatchKey(db, key):
    # 如果键key存在于数据库的watched_keys字典中
    # 那么说明至少有一个客户端在监视这个key
    if key in db.watched_keys:
        # 遍历所有监视键key的客户端
        for client in db.watch_keys[key]:
            # 打开标识
            client.flags |= REDIS_DIRTY_CAS
```
例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519090445.png)

# 判断事务是否安全
当服务器接收到一个客户端发来的EXEC命令时,服务器会根据这个客户端是否打开了REDIS_DIRTY_CAS标识来决定是否执行事务:
* 如果客户端的REDIS_DIRTY_CAS标识已经被打开,那么说明客户端所监视的键当中,至少有一个键已经被修改过了,在这种情况下,客户端提交的事务已经不再安全,所有服务器会拒绝执行客户端提交的事务.
* 如果客户端的REDIS_DIRTY_CAS标识没有被打开,那么说明客户端监视的所有键都没有被修改过(或者客户端没有监视任何键),事务仍然是安全的,服务器将执行客户端提交的这个事务.

判断是否执行事务的过程:

![图19-6 服务器判断是否执行事务的过程](https://github.com/gdufeZLYL/blog/blob/master/images/20180519091104.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519091143.png)

# 一个完整的WATCH事务执行过程

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519091451.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519091521.png)