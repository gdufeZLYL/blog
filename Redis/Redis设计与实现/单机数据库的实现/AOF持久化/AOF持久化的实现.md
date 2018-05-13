# AOF持久化
## AOF持久化和RDB持久化的区别
* RDB持久化通过保存数据库中的键值对来记录数据库状态不同;
* AOF持久化是通过保存Redis服务器所执行的写命令来记录数据库状态的,如图11-1所示:

![AOF持久化](https://github.com/gdufeZLYL/blog/blob/master/images/20180513163519.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180513163703.png)

![AOF文件](https://github.com/gdufeZLYL/blog/blob/master/images/20180513163809.png)

![AOF文件载入](https://github.com/gdufeZLYL/blog/blob/master/images/20180513163853.png)

# 命令追加
当AOF持久化功能处于打开状态时,服务器在执行完一个写命令之后,会以协议格式将被执行的写命令追加到服务器状态的aof_buf缓冲区的末尾:
```
struct redisServer {
    // ...

    // AOF缓冲区
    sds aof_buf;

    // ...
};
```
例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180513165132.png)

另一个例子:

![另一个例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180513165258.png)

# AOF文件的写入与同步
> Redis的服务器进程就是一个事件循环(loop),这个循环中的文件事件负责接收客户端的命令请求,以及向客户端发送命令回复,而时间事件则负责执行像serverCron函数这样需要定时运行的函数.因为服务器在处理文件事件时可能会执行写命令,使得一些内容被追加到aof_buf缓冲区里面,所以在服务器每次结束一个事件循环之前,它都会调用flushAppendOnlyFile函数,考虑是否需要将aof_buf缓冲区中的内容写入和保存到AOF文件里面.

伪代码如下:
```python
def eventLoop():
    while True:

        # 处理文件事件,接收命令请求以及发送命令回复
        # 处理命令请求时可能会有新内容被追加到aof_buf缓冲区中
        processFileEvents()

        # 处理时间事件
        processTimeEvents()

        # 考虑是否要将aof_buf中的内容写入和保存到AOF文件里面
        flushAppendOnlyFile()
```
flushAppendOnlyFile函数的行为由服务器配置的appendfsync选项的值决定,默认为everysec,不同appendfsync值产生不同的持久化行为如下:

![不同appendfsync值产生不同的持久化行为](https://github.com/gdufeZLYL/blog/blob/master/images/20180513173235.png)

文件的写入和同步:

![文件的写入和同步](https://github.com/gdufeZLYL/blog/blob/master/images/20180513173348.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180513174000.png)

AOF持久化的效率和安全性

![AOF持久化的效率和安全性](https://github.com/gdufeZLYL/blog/blob/master/images/20180513174048.png)

![AOF持久化的效率和安全性](https://github.com/gdufeZLYL/blog/blob/master/images/20180513174129.png)