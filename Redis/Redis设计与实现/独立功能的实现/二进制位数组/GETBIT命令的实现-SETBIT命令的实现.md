# GETBIT命令的实现
GETBIT命令用于返回位数组bitarray在offset偏移量上的二进制位的值:
```
GETBIT <bitarray> <offset>
```
GETBIT命令的执行过程如下：

![GETBIT命令的执行过程](https://github.com/gdufeZLYL/blog/blob/master/images/20180520215823.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520220237.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520220257.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520220332.png)

# SETBIT命令的实现
SETBIT用于将位数组bitarray在offset偏移量上的二进制位的值设置为value,并向客户端返回二进制位被设置之前的旧值:
```
SETBIT <bitarray> <offset> <value>
```
以下是SETBIT命令的执行过程:

![SETBIT命令的执行过程](https://github.com/gdufeZLYL/blog/blob/master/images/20180520221128.png)

![SETBIT命令的执行过程](https://github.com/gdufeZLYL/blog/blob/master/images/20180520221154.png)

# SETBIT命令的执行示例
让我们通过观察一些SETBIT命令的执行例子来熟悉SETBIT命令的运行过程.

首先,如果我们对图22-2所示的位数组执行命令:
```
SETBIT <bitarray> 1 1
```
那么服务器将执行以下操作:

![操作](https://github.com/gdufeZLYL/blog/blob/master/images/20180520221714.png)

# 带扩展操作的SETBIT命令示例
前面展示的SETBIT例子无须对位数组进行扩展,现在,让我们来看一个需要对位数组进行扩展的例子.

假设我们对图22-2所示的位数组执行命令:
```
SETBIT <bitarray> 12 1
```
那么服务器将执行以下操作:

![操作](https://github.com/gdufeZLYL/blog/blob/master/images/20180520223709.png)

![操作](https://github.com/gdufeZLYL/blog/blob/master/images/20180520223739.png)

![操作](https://github.com/gdufeZLYL/blog/blob/master/images/20180520223802.png)

![操作](https://github.com/gdufeZLYL/blog/blob/master/images/20180520223829.png)

![操作](https://github.com/gdufeZLYL/blog/blob/master/images/20180520223857.png)