# EVAL命令的执行过程
1. 根据客户端给定的Lua脚本,在Lua环境中定义一个Lua函数.
2. 将客户端给定的脚本保存到lua_scripts字典,等待将来进一步使用.
3. 执行刚刚在Lua环境中定义的函数,以此来执行客户端给定的Lua脚本.

以下三个小节将以:
```
redis> EVAL "return 'hello world'" 0
"hello world"
```
命令为示例,分别介绍EVAL命令执行的三个步骤.

# 定义脚本函数
当客户端向服务器发送EVAL命令,要求执行某个Lua脚本的时候,服务器首先要做的就是在Lua环境中,为传入的脚本定义一个与这个脚本相对应的Lua函数,其中,Lua函数的名字由f_前缀加上脚本的SHA1校验和(四十个字符长)组成,而函数的体则是脚本本身.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520090436.png)

使用函数来保存客户端传入的脚本有以下好处:

![使用函数来保存客户端传入的脚本有以下好处](https://github.com/gdufeZLYL/blog/blob/master/images/20180520090549.png)

# 将脚本保存到lua_scripts字典
EVAL命令要做的第二件事是将客户端传入的脚本保存到服务器的lua_scripts字典里面.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520092307.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520092359.png)

# 执行脚本函数
在为脚本定义函数,并且将脚本保存到lua_scripts字典之后,服务器还需要进行一些设置钩子、传入参数之类的准备动作,才能正式开始执行脚本.

整个准备和执行脚本的过程如下:

![整个准备和执行脚本的过程](https://github.com/gdufeZLYL/blog/blob/master/images/20180520094344.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520094430.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520094450.png)