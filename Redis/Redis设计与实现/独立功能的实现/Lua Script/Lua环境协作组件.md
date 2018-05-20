# 伪客户端
因为执行Redis命令必须有相应的客户端状态,所以为了执行Lua脚本中包含的Redis命令,Redis服务器专门为Lua环境创建了一个伪客户端,并由这个伪客户端负责处理Lua脚本中包含的所有Redis命令.

Lua脚本使用redis.call函数或者redis.pcal函数执行一个Redis命令,需要完成以下步骤:

![步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520083206.png)

图20-2展示了Lua脚本在调用redis.call函数时,Lua环境、伪客户端、命令执行器三者之间的通信过程(调用redis.pcall函数时产生的通信过程也是一样的).

![图20-2 Lua脚本执行Redis命令时的通信步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520083534.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520083618.png)

# lua_scripts字典
除了伪客户端之外,Redis服务器为Lua环境创建的另一个协作组件是lua_scripts字典,这个字典的键为某个Lua脚本的SHA1校验和,而字典的值则是SHA1校验和对应的Lua脚本:
```c++
struct redisServer {
    // ...

    dict *lua_scriptes;

    // ...
};
```
Redis服务器会将所有被EVAL命令执行过的Lua脚本,以及所有被SCRIPT LOAD命令载入过的Lua脚本都保存到lua_scripts字典里面.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520084634.png)