# ALPHA选项的实现
通过使用ALPHA选项,SORT命令可以对包含字符串值的键进行排序:
```
SORT <key> ALPHA
```
例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520160623.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520160650.png)

服务器执行SORT fruits ALPHA命令的详细步骤如下:

![服务器执行SORT fruits ALPHA命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520160750.png)

![图21-5 将obj指针指向集合的各个元素](https://github.com/gdufeZLYL/blog/blob/master/images/20180520160813.png)

![图21-6 按集合元素进行排序后的数组](https://github.com/gdufeZLYL/blog/blob/master/images/20180520160854.png)

# ASC选项和DESC选项的实现
在默认情况下,SORT命令执行升序排序,排序后的结果按值的大小从到大排列,以下两个命令是完全等价的:
```
SORT <key>
SORT <key> ASC
```
相反地,在执行SORT命令时使用DESC选项,可以让命令执行降序排列,让排序后的结果按值的大小从大到小排列:
```
SORT <key> DESC
```
例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520162011.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520162038.png)

![图21-7 执行升序排序的数组](https://github.com/gdufeZLYL/blog/blob/master/images/20180520162106.png)

![图21-8 执行降序排列的数组](https://github.com/gdufeZLYL/blog/blob/master/images/20180520162135.png)

# BY选项的实现
在默认情况下,SORT命令使用被排序键包含的元素作为排序的权重,元素本身决定了元素在排序之后所处的位置.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520162954.png)

服务器执行SORT fruits BY *-price命令的详细步骤如下:

![服务器执行SORT fruits BY *-price命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520163418.png)

![服务器执行SORT fruits BY *-price命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520163441.png)

![服务器执行SORT fruits BY *-price命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520163511.png)

![服务器执行SORT fruits BY *-price命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520163543.png)

# 带有ALPHA选项的BY选项的实现
BY选项默认假设权重键保存的值为数字值,如果权重键保存的是字符串值的话,那么就需要在使用BY选项的同时,配合使用ALPHA选项.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520170336.png)

服务器执行SORT fruits BY *-id ALPHA命令的详细步骤如下:

![服务器执行SORT fruits BY *-id ALPHA命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520170431.png)

![服务器执行SORT fruits BY *-id ALPHA命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520170504.png)

![图21-13 将u.cmpobj指针指向权重键](https://github.com/gdufeZLYL/blog/blob/master/images/20180520170533.png)

![图21-14 将u.cmpobj所指向的字符串对象进行排序之后的数组](https://github.com/gdufeZLYL/blog/blob/master/images/20180520170618.png)