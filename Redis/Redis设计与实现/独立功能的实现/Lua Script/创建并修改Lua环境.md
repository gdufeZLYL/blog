# 创建并修改Lua环境
为了在Redis服务器中执行Lua脚本,Redis在服务器内嵌了一个Lua环境,并对这个Lua环境进行了一系列修改,从而确保这个Lua环境可以满足Redis服务器的需要.

Redis服务器创建并修改Lua环境的整个过程由以下步骤组成:

![Redis服务器创建并修改Lua环境的整个过程](https://github.com/gdufeZLYL/blog/blob/master/images/20180519104955.png)

# 创建Lua环境
在最开始的这一步,服务器首先调用Lua的C API函数lua_open,创建一个新的Lua环境.

因为lua_open函数创建的只是一个基本的Lua环境,为了让这个Lua环境可以满足Redis的操作要求,接下来服务器将对这个Lua环境进行一系列修改.

# 载入函数库
Redis修改Lua环境的第一步,就是将以下函数库载入到Lua环境里面:

![函数库](https://github.com/gdufeZLYL/blog/blob/master/images/20180519141218.png)

![函数库](https://github.com/gdufeZLYL/blog/blob/master/images/20180519141255.png)

![函数库](https://github.com/gdufeZLYL/blog/blob/master/images/20180519141321.png)

# 创建redis全局表格
在这一步,服务器将在Lua环境中创建一个redis表格(table),并将它设为全局变量.这个redis表格包含以下函数:
* 用于执行Redis命令的redis.call和redis.pcall函数
* 用于记录Redis日志(log)的redis.log函数,以及相应的日志级别(level)常量: redis.LOG_DEBUG, redis.LOG_VERBOSE, redis.LOG_NOTICE, 以及redis.LOG_WARNING.
* 用于计算SHA1校验和的redis.shalhex函数
* 用于返回错误信息的redis.error_reply函数和redis.status_reply函数

在这些函数里面,最常用也最重要的要数redis.call函数和redis.pcall函数,通过这两个函数,用户可以直接在Lua脚本中执行Redis命令:
```
redis> EVAL "return redis.call('PING')" 0
PONG
```

# 使用Redis自制的随机函数来替换Lua原有的随机函数
为了保证相同的脚本可以在不同的机器上产生相同的结果,Redis要求所有传入服务器的Lua脚本,以及Lua环境中的所有函数,都必须是无副作用的纯函数.

但是,在之前载入Lua环境的math函数库中,用于生成随机数的math.random函数和math.randomseed函数都是带有副作用的,它们不符合Redis对Lua环境的无副作用.

因为这个原因,Redis使用自制的函数替换了math库中原有的math.random函数和math.randomseed函数,替换之后的两个函数有以下特征:
* 对于相同的seed来说,math.random总产生相同的随机数序列,这个函数是一个纯函数.
* 除非在脚本中使用math.randomseed显式地修改seed,否则每次运行脚本时,Lua环境都使用固定的math.randomseed(0)语句来初始化seed.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519214510.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519214750.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519214834.png)

# 创建排序辅助函数
上一个小节说到,为了防止带有副作用的函数令脚本产生不一致的数据,Redis对math库的math.random函数和math.randomseed函数进行了替换.

对于Lua脚本来说,另一个可能产生不一致数据的地方是那些带有不确定性质的命令.比如对于一个集合键来说,因为集合元素的排列是无序的,所以即使两个集合的元素完全相同,它们的输出结果也可能并不相同.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519215451.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519215652.png)

Redis将SMEMBERS这种在相同数据集上可能会产生不同输出的命令称为"带有不确定性的命令",这些命令包括:
* SINTER
* SUNION
* SDIFF
* SMEMBERS
* HKEYS
* HVALS
* KEYS
为了消除这些命令带来的不确定性,服务器会为Lua环境创建一个排序辅助函数__redis__compare_helper,当Lua脚本执行完一个带有不确定性的命令之后,程序会使用__redis__compare_helper作为对比函数,自动调用table.sort函数对命令的返回值做一次排序,以此来保证相同的数据集总是产生相同的输出.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519222531.png)

# 创建redis.pcall函数的错误报告辅助函数
在这一步,服务器将为Lua环境创建一个名为__redis__err__handler的错误处理函数,当脚本调用redis.pcall函数执行Redis命令,并且被执行的命令出现错误时,__redis_err_handler就会打印出错代码的来源和发生错误的行数,为程序的调式提供方便.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519223117.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519223133.png)

# 保护Lua的全局环境
在这一步,服务器将对Lua环境中的全局环境进行保护,确保传入服务器的脚本不会因为忘记使用local关键字而将额外的全局变量添加到Lua环境里面.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519223750.png)

# 将Lua环境保存到服务器状态的lua属性里面

![将Lua环境保存到服务器状态的lua属性里面](https://github.com/gdufeZLYL/blog/blob/master/images/20180519224417.png)