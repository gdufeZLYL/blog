SORT命令的最简单执行形式为:
```
SORT <key>
```
这个命令可以对一个包含数字值的键key进行排序.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520154819.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520154850.png)

服务器执行SORT numbers命令的详细步骤如下:

![服务器执行SORT numbers命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520154936.png)

![服务器执行SORT numbers命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520155013.png)

![服务器执行SORT numbers命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520155031.png)

以下是redisSortObject结构的完整定义:
```c++
typedef struct _redisSortObject {
    // 被排序键的值
    robj *obj;

    // 权重
    union {
        // 排序数字值时使用
        double score;

        // 排序带有BY选项的字符串值时使用
        robj *cmpobj;
    } u;
} redisSortObject;
```
SORT命令为每个被排序的键都创建一个与键长度相同的数组,数组的每个项都是一个redisSortObject结构,根据SORT命令使用的选项不同,程序使用redisSortObject结构的方式也不同.