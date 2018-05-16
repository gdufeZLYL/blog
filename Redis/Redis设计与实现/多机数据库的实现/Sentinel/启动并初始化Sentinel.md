# 启动Sentinel
启动一个Sentinel可以使用命令:
```
$ redis-sentinel /path/to/your/sentinel.conf
或者命令:
$ redis-server /path/to/your/sentinel.conf --sentinel
```
这两个命令的效果完全相同.

当一个Sentinel启动时,它需要执行以下步骤:
1. 初始化服务器
2. 将普通Redis服务器使用的代码替换成Sentinel专用代码
3. 初始化Sentinel状态
4. 根据给定的配置文件,初始化Sentinel的监视主服务器列表
5. 创建连向主服务器的网络连接

# 初始化服务器
Sentinel本质上只是一个运行在特殊模式下的Redis服务器,Sentinel执行的工作和普通Redis服务器执行的工作不同,所以Sentinel的初始化过程和普通Redis服务器初始化过程并不完全相同,Sentinel模式下Redis服务器主要功能的使用情况:

![Sentinel模式下Redis服务器主要功能的使用情](https://github.com/gdufeZLYL/blog/blob/master/images/20180516191239.png)

# 使用Sentinel专用代码
启动Sentinel的第二个步骤就是将部分普通Redis服务器使用的代码替换成Sentinel专用代码.

直接看例子:

端口例子:

![端口例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180516191841.png)

命令表例子:

![命令表例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180516191923.png)

# 初始化Sentinel状态
在应用了Sentinel的专用代码之后,接下来,服务器会初始化一个sentinel.c/sentinelState结构(后面简称"Sentinel状态"),这个结构保存了服务器中所有和Sentinel功能有关的状态(服务器的一般状态仍然由redis.h/redisServer结构保存)：
```c++
struct sentinelState {
    // 当前纪元,用于实现故障转移
    uint64_t current_epoch;

    // 保存了所有被这个sentinel监视的主服务器
    // 字典的键是主服务器的名字
    // 字典的值则是一个指向sentinelRedisInstance结构的指针
    dict *masters;

    // 是否进入了TILT模式?
    int tilt;

    // 目前正在执行的脚本的数量
    int running_scripts;

    // 进入TILT模式的时间
    mstime_t tilt_start_time;

    // 最后一次执行时间处理器的时间
    mstime_t previous_time;

    // 一个FIFO队列,包含了所有需要执行的用户脚本
    list *scripts_queue;
} sentinel;
```

# 初始化Sentinel状态的masters属性
Sentinel状态中的masters字典记录了所有被Sentinel监视的主服务器的相关信息,其中:
* 字典的键是被监视主服务器的名字
* 而字典的值则是被监视主服务器对应的sentinel.c/sentinelRedisInstance结构

关于sentinelRedisInstance结构:
> 每个sentinelRedisInstance结构(后面简称"实例结构")代表一个被Sentinel监视的Redis服务器实例(instance),这个实例可以是主服务器、从服务器,或者另外一个Sentinel.

```c++
typedef struct sentinelRedisInstance {
    // 标识值, 记录了实例的类型, 以及该实例的当前状态
    int flags;

    // 实例的名字
    // 主服务器的名字由用户在配置文件中设置
    // 从服务器以及Sentinel的名字由Sentinel自动设置
    // 格式为ip:port, 例如"127.0.0.1:26379"
    char *name;

    // 实例的运行ID
    char *runid;

    // 配置纪元,用于实现故障转移
    uint64_t config_epoch;

    // 实例的地址
    sentinelAddr *addr;

    // SENTINEL down-after-milliseconds选项设定的值
    // 实例无响应多少毫秒之后才会被判断为主观下线(subjectively down)
    mstime_t down_after_period;

    // SENTINEL monitor <master-name> <IP> <port> <quorum>选项中的quorum参数
    // 判断这个实例为客观下线(objectively down)所需的支持投票数量
    int parallel_syncs;

    // SENTINEL failover-timeout;

    // ...
} sentinelRedisInstance;
```
sentinelRedisInstance.addr属性是一个指向sentinel.c/sentinelAddr结构的指针,这个结构保存着实例的IP地址和端口号:
```c++
typedef struct sentinelAddr {
    char *ip;
    int port;
} sentinelAddr;
```
对Sentinel状态的初始化将引发对masters字典的初始化,而masters字典的初始化是根据被载入的Sentinel配置文件来进行的.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180516203119.png)

![图16-5 master1的实例结构](https://github.com/gdufeZLYL/blog/blob/master/images/20180516203403.png)

![图16-6 master2的实例结构](https://github.com/gdufeZLYL/blog/blob/master/images/20180516203548.png)

![图16-7 Sentinel状态以及masters字典](https://github.com/gdufeZLYL/blog/blob/master/images/20180516203640.png)

# 创建连向主服务器的网络连接
初始化Sentinel的最后一步是创建连向被监视主服务器的网络连接,Sentinel将成为主服务器的客户端,它可以向主服务器发送命令,并从命令回复中获取相关的信息.

对于每个被Sentinel监视的主服务器来说,Sentinel会创建两个连向主服务器的异步网络连接:
* 一个是命令连接,这个连接专门用于向主服务器发送命令,并接收命令回复.
* 另一个是订阅连接,这个连接专门用于订阅主服务器的__sentinel__:hello频道.

为什么有两个连接?

![为什么有两个连接?](https://github.com/gdufeZLYL/blog/blob/master/images/20180516204126.png)

图16-8展示了一个Sentinel向被它监视的两个主服务器master1和master2创建命令连接和订阅连接的例子.

![Sentinel向主服务器创建网络连接](https://github.com/gdufeZLYL/blog/blob/master/images/20180516204308.png)