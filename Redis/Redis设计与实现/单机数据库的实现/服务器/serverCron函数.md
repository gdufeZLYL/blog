# 关于serverCron函数
> Redis服务器中的serverCron函数默认每隔1001毫秒执行一次,这个函数负责管理服务器的资源,并保持服务器自身的良好运转.

# 更新服务器时间缓冲
Redis服务器中有不少功能需要获取系统的当前时间,而每次获取系统的当前时间需要执行一次系统调用,为了减少系统调用的执行次数,服务器状态中的unixtime属性和mstime属性被用作当前时间的缓存:
```c++
struct redisServer {
    // ...

    // 保存了秒级精度的系统当前UNIX时间戳
    time_t unixtime;

    // 保存了毫秒级精度的系统当前UNIX时间戳
    long long mstime;

    // ...
};
```
因为serverCron函数默认会以每100毫秒一次的频率更新unixtime属性和mstime,所以这两个属性记录的时间的精确度并不高:
* 服务器状态中的lruclock属性保存了服务器的LRU时钟,这个属性和上面介绍的unixtime属性、mstime属性一样,都是服务器时间缓存的一种:
```c++
struct redisServer {
    // ...

    // 默认每10秒更新一次的时钟缓存,
    // 用于计算键的空转(idle)时长
    unsigned lruclock: 22;

    // ...
};
```
每个Redis对象都会有一个lru属性,这个lru属性保存了对象最后一次被命令访问的时间:
```c++
typedef struct redisObject {
    // ...

    unsigned lru: 22;

    // ...
};
```
当服务器要计算一个数据库键的空转时间(也即是数据库键对应的值对象的空转时间),程序会用服务器的lruclock属性记录的时间减去对象的lru属性记录的时间,得出的计算结果就是这个对象的空转时间:
```
redis> SET msg "hello world"
OK

# 等待一小段时间
redis> OBJECT IDLETIME msg
(integer) 20

# 等待一阵子
redis> OBJECT IDLETIME msg
(integer) 180

# 访问msg键的值
redis> GET msg
"hello world"

# 键处于活跃状态,空转时长为0
redis> OBJECT IDLETIME msg
(integer) 0
```
serverCron函数默认会以每10秒一次的频率更新lruclock属性的值,因为这个时钟不是实时的,所以根据这个属性计算出来的LRU时间实际上只是一个模糊的估算值.

lruclock时钟的当前值可以通过INFO server命令的lru_clock域查看:
```
redis> INFO server
# Server
...
lru_clock: 55923
...
```

# 更新服务器每秒执行命令次数
serverCron函数中的trackOperationsPerSecond函数会以每100毫秒一次的频率执行,这个函数的功能是以抽样计算的方式,估算并记录服务器在最近一秒钟处理的命令请求数量,这个值可以通过INFO status命令的instantaneous ops per sec域查看:
```
redis> INFO stats
# Stats
...
instantaneous_ops_per_sec:6
...
```
上面的命令结构显示,在最近的一秒钟内,服务器处理了大概六个命令.

trackOperationsPerSecond函数和服务器状态中四个ops_sec_开头的属性有关:
```c++
struct redisServer {
    // ...

    // 上一次进行抽样的时间
    long long ops_sec_last_sample_time;

    // 上一次抽样时,服务器已执行命令的数量
    long long ops_sec_last_sample_ops;

    // REDIS_OPS_SEC_SAMPLES大小(默认值为16)的环形数组,
    // 数组中的每个项都记录了一次抽样结果
    long long ops_sec_samples[REDIS_OPS_SEC_SAMPLES];

    // ops_sec_samples数组的索引值,
    // 每次抽样后将值自增一,
    // 在值等于16时重置为0,
    // 让ops_sec_samples数组构成一个环形数组.
    int ops_sec_idx;

    // ...
};
```
分析:

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180515220023.png)

```c++
long long getOperationsPerSecond(void) {
    int j;
    long long sum = 0;

    // 计算所有取样值的总和
    for (j = 0; j < REDIS_OPS_SEC_SAMPLES; j++) 
        sum += server.ops_sec_samples[j];
    
    // 计算取样的平均值
    return sum/REDIS_OPS_SEC_SAMPLES;
}
```

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180515220106.png)

# 更新服务器内存峰值记录
服务器状态中的stat_peak_memory属性记录了服务器的内存峰值大小:
```c++
struct redisServer {
    // ...

    // 已使用内存峰值
    size_t stat_peak_memory;

    // ...
};
```
分析:

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180515224145.png)

# 处理SIGTERM信号
在启动服务器时,Redis会为服务器进程的SIGTERM信号关联处理器sigtermHandler函数,这个信号处理器负责在服务接到SIGTERM信号时,打开服务器状态的shutdown_asap标识:
```c++
// SIGTERM信号的处理器
static void sigtermHandler(int sig) {
    // 打印日志
    redisLogFromHandler(REDIS_WARNING, "Received SIGTERM, scheduling shutdown...");

    // 打开关闭标识
    server.shutdown_asap = 1;
}
```
每次serverCron函数运行时,程序都会对服务器状态的shutdown_asap属性进行检查,并根据属性的值决定是否关闭服务器:
```c++
struct redisServer {
    // ...

    // 关闭服务器的标识
    // 值为1时,关闭服务器,
    // 值为0时,不做动作
    int shutdown_asap;

    // ...
};
```
分析:

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180515225612.png)

# 管理客户端资源

![管理客户端资源](https://github.com/gdufeZLYL/blog/blob/master/images/20180515230439.png)

# 管理数据库资源

![管理数据库资源](https://github.com/gdufeZLYL/blog/blob/master/images/20180515230624.png)

# 执行被延迟的BGREWRITEAOF

![执行被延迟的BGREWRITEAOF](https://github.com/gdufeZLYL/blog/blob/master/images/20180515231001.png)

![执行被延迟的BGREWRITEAOF](https://github.com/gdufeZLYL/blog/blob/master/images/20180515231026.png)

# 检查持久化操作的运行状态

![检查持久化操作的运行状态](https://github.com/gdufeZLYL/blog/blob/master/images/20180515231220.png)

![检查持久化操作的运行状态](https://github.com/gdufeZLYL/blog/blob/master/images/20180515231300.png)

![检查持久化操作的运行状态](https://github.com/gdufeZLYL/blog/blob/master/images/20180515231615.png)

# 将AOF缓冲区中的内容写入AOF文件

![将AOF缓冲区中的内容写入AOF文件](https://github.com/gdufeZLYL/blog/blob/master/images/20180515232906.png)

# 关闭异步客户端
在这一步,服务器会关闭那些输出缓冲区大小超出限制的客户端,第13章对此有详细的说明.

# 增加cronloops计数器的值
服务器状态的cronloops属性记录了serverCron函数执行的次数:
```c++
struct redisServer {
     // ...

     // serverCron函数的运行次数计数器
     // serverCron函数每执行一次,这个属性的值就增一
     int cronloops;

     // ...
};
```
cronloops属性目前在服务器中的唯一作用,就是在复制模块中实现"每执行serverCron函数N次就执行一次指定代码"的功能,方法如以下伪代码所示:
```python
if cronloops % N == 0:
    # 执行指定代码 ...
```