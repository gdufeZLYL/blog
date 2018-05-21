# BITCOUNT命令的实现
BITCOUNT命令用于统计给定位数组中,值为1的二进制位的数量

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180521091553.png)

## 二进制位统计算法(1): 遍历算法
图22-17展示了程序使用遍历算法,对一个8位长的位数组进行遍历并计数的整个过程:

![图22-17](https://github.com/gdufeZLYL/blog/blob/master/images/20180521092235.png)
![图22-17](https://github.com/gdufeZLYL/blog/blob/master/images/20180521092319.png)

缺点:

![缺点](https://github.com/gdufeZLYL/blog/blob/master/images/20180521092007.png)

## 二进制位统计算法(2): 查表算法
原理:

![原理](https://github.com/gdufeZLYL/blog/blob/master/images/20180521093224.png)

![原理](https://github.com/gdufeZLYL/blog/blob/master/images/20180521093307.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180521093446.png)

![表22-1 可以快速检查8位长的位数组包含多少个1](https://github.com/gdufeZLYL/blog/blob/master/images/20180521093515.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180521093559.png)

缺点:

![缺点](https://github.com/gdufeZLYL/blog/blob/master/images/20180521093645.png)

![缺点](https://github.com/gdufeZLYL/blog/blob/master/images/20180521093707.png)

## 二进制位统计算法(3): variable-precision SWAR算法
### 计算汉明重量

![计算汉明重量](https://github.com/gdufeZLYL/blog/blob/master/images/20180521094415.png)

### variable-precision SWAR算法实现

![variable-precision SWAR算法实现](https://github.com/gdufeZLYL/blog/blob/master/images/20180521094452.png)

![variable-precision SWAR算法实现](https://github.com/gdufeZLYL/blog/blob/master/images/20180521094630.png)

![variable-precision SWAR算法实现](https://github.com/gdufeZLYL/blog/blob/master/images/20180521094704.png)

### 例子

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180521101135.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180521101207.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180521102223.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180521102249.png)

## 二进制位统计算法(4): Redis的实现

![二进制位统计算法(4): Redis的实现](https://github.com/gdufeZLYL/blog/blob/master/images/20180521102402.png)

![二进制位统计算法(4): Redis的实现](https://github.com/gdufeZLYL/blog/blob/master/images/20180521102445.png)

![二进制位统计算法(4): Redis的实现](https://github.com/gdufeZLYL/blog/blob/master/images/20180521102519.png)

![二进制位统计算法(4): Redis的实现](https://github.com/gdufeZLYL/blog/blob/master/images/20180521102542.png)

![二进制位统计算法(4): Redis的实现](https://github.com/gdufeZLYL/blog/blob/master/images/20180521102556.png)

# BITOP命令的实现

![BITOP命令的实现](https://github.com/gdufeZLYL/blog/blob/master/images/20180521103029.png)

![BITOP命令的实现](https://github.com/gdufeZLYL/blog/blob/master/images/20180521103100.png)

![BITOP命令的实现](https://github.com/gdufeZLYL/blog/blob/master/images/20180521103124.png)