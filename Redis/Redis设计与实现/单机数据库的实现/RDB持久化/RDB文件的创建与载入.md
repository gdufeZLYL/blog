# 前言

数据库状态
> Redis是一个键值对数据库服务器,服务器中通常包含着任意个非空数据库,而每个非空数据库中又可以包含任意个键值对,我们将服务器中的非空数据库以及它们的键值对统称为数据库状态.

例子: 图10-1展示了一个包含三个非空数据库的Redis服务器,这三个数据库以及数据库中的键值对就是该服务器的数据库状态.

![数据库状态示例](https://github.com/gdufeZLYL/blog/blob/master/images/20180512173748.png)

为什么需要持久化?
> 因为Redis是内存数据库,它将自己的数据库状态储存在内存里面,所以如果不想办法将储存在内存中的数据库状态保存到磁盘里面,那么一旦服务器进程退出,服务器中的数据库状态也会消失不见.

RDB持久化功能
> 将Redis在内存中的数据库状态保存到磁盘里面,避免数据意外丢失.

RDB持久化功能怎么工作?

RDB持久化既可以手动执行,也可以根据服务器配置选项定期执行,该功能可以将某个时间点上的数据库装套保存到一个RDB文件中,如图10-2所示:

![将数据库状态保存为RDB文件](https://github.com/gdufeZLYL/blog/blob/master/images/20180512175006.png)

RDB持久化功能所生成的RDB文件是一个经过压缩的二进制文件,通过该文件可以还原生成RDB文件时的数据库状态,如图10-3所示:

![用RDB文件来还原数据库状态](https://github.com/gdufeZLYL/blog/blob/master/images/20180512175141.png)

因为RDB文件是保存在硬盘里面的,所以即使Redis服务器进程退出,甚至运行Redis服务器的计算机停机,但只要RDB文件仍然存在,Redis服务器就可以用它来还原数据库状态.

# RDB文件的创建
## SAVE命令
SAVE命令会阻塞Redis服务器进程,直到RDB文件创建完毕为止,在服务器进程阻塞期间,服务器不能处理任何命令请求:
```
redis> SAVE //等待直到RDB文件创建完毕
OK
```
## BGSAVE命令
BGSAVE命令会派生出一个子进程,然后由子进程负责创建RDB文件,服务器进程(父进程)继续处理命令请求:
```
redis> BGSAVE   //派生子进程,并由子进程创建RDB文件
Background saving started
```

## SAVE命令和BGSAVE命令的实现
创建RDB文件的实际工作由rdb.c/rdbSave函数完成,SAVE命令和BGSAVE命令会以不同的方式调用这个函数,通过以下伪代码可以明显地看出这两个命令之间的区别:
```python
def SAVE():
    # 创建RDB文件
    rdbSave()

def BGSAVE():

    # 创建子进程
    pid = fork()

    if pid == 0:

        # 子进程负责创建RDB文件
        rdbSave()

        # 完成之后向父进程发送信号
        signal_parent()
    elif pid > 0:

        # 父进程继续处理命令请求,并通过轮询等待子进程的信号
        handle_request_and_wait_signal()
    else:

        # 处理出错情况
        handle_fork_error()
```

# RDB文件的载入

![RDB文件的载入](https://github.com/gdufeZLYL/blog/blob/master/images/20180512203610.png)

## 服务器还原数据库状态的文件选择
因为AOF文件的更新频率通常比RDB文件的更新频率高,所以:
* 如果服务器开启了AOF持久化功能,那么服务器会优先使用AOF文件来还原数据库状态.
* 只有在AOF持久化功能处于关闭状态时,服务器才会使用RDB文件来还原数据库状态.

服务器还原数据库状态选择文件的流程:

![服务器还原数据库状态选择文件的流程](https://github.com/gdufeZLYL/blog/blob/master/images/20180512204420.png)

# SAVE命令执行时的服务器状态

![SAVE命令执行时的服务器状态](https://github.com/gdufeZLYL/blog/blob/master/images/20180512204703.png)

# BGSAVE命令执行时的服务器状态
* 在BGSAVE命令执行期间,客户端发送的SAVE命令会被服务器拒绝,服务器禁止SAVE命令和BGSAVE命令同时执行是为了避免父进程(服务器进程)和子进程同时执行两个rdbSave调用,防止产生竞争条件.
* 在BGSAVE命令执行期间,客户端发送的BGSAVE命令会被服务器拒绝,因为同时执行两个BGSAVE命令也会产生竞争条件.
* BGREWRITEAOF和BGSAVE两个命令不能同时执行:
1. 如果BGSAVE命令正在执行,那么客户端发送的BGREWRITEAOF命令会被延迟到BGSAVE命令执行完毕之后执行.
2. 如果BGREWRITEAOF命令正在执行,那么客户端发送的BGSAVE命令会被服务器拒绝.

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180512221815.png)

# RDB文件载入时的服务器状态
服务器在载入RDB文件期间,会一直处于阻塞状态,直到载入工作完成为止.