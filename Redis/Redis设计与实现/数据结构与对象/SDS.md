# 前言
> Redis没有直接使用C语言传统的字符串表示(以空字符结尾的字符数组,以下简称C字符串),而是自己构建了一种名为简单动态字符串(simple dynamic string, SDS)的抽象类型,并将SDS用作Redis的默认字符串表示.

## 动态字符串
> 当Redis需要的不仅仅是一个字符串字面量,而是一个可以被修改的字符串值时,Redis就会使用SDS来表示字符串值,比如在Redis的数据库里面,包含字符串值的键值对在底层都是由SDS实现的.

## 例子
![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180507222656.png)

# SDS的定义
## SDS的结构
```C
struct sdshdr {
    // 记录buf数组中已使用字节的数量
    // 等于SDS所保存字符串的长度
    int len;

    // 记录buf数组中未使用字节的数量
    int free;

    // 字节数组,用于保存字符串
    char buf[];
}
```
## SDS示例

![SDS示例](https://github.com/gdufeZLYL/blog/blob/master/images/20180507223350.png)

![SDS示例解析](https://github.com/gdufeZLYL/blog/blob/master/images/20180507223517.png)

# SDS与C字符串的区别
C字符串:
> C语言使用长度为N+1的字符数组来表示长度为N的字符串,并且字符数组的最后一个元素总是空字符'\0'

![C字符串](https://github.com/gdufeZLYL/blog/blob/master/images/20180507223852.png)

## 常数复杂度获取字符串长度
### C字符串: O(n)
C字符串没有记录自身的长度信息,所以每次都必须遍历整个字符串.
![演示](https://github.com/gdufeZLYL/blog/blob/master/images/20180507224253.png)


### SDS: O(1)
SDS在len属性中记录了SDS本身的长度,所以获取长度的复杂度为O(1).

![演示](https://github.com/gdufeZLYL/blog/blob/master/images/20180507224405.png)

![演示](https://github.com/gdufeZLYL/blog/blob/master/images/20180507224426.png)

## 杜绝缓冲区溢出
### C字符串容易造成缓冲溢出
C语言中的strcat函数可以将src字符串中的内容拼接到dest字符串的末尾.因为C字符串不记录自身的长度,所以strcat假定用户在执行这个函数时,已经为dest分配了足够多的内存,可以容纳src字符串中的所有内容,而一旦这个假定不成立时,就会产生缓冲区溢出.

#### 例子
![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180508084917.png)

### SDS的空间策略杜绝缓冲区溢出
当SDS API需要对SDS进行修改时,API会先检查SDS的空间是否满足修改所需的要求,如果不满足的话,API会自动将SDS的空间扩展至执行修改所需的大小,然后才执行实际的修改操作,所以使用SDS既不需要手动修改SDS的空间大小,也不会出现前面所说的缓冲区溢出问题.

#### 例子
![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180508090351.png)

### 减少修改字符串时带来的内存重分配次数
#### C字符串不利于字符串的动态变化

![C字符串不利于字符串的动态变化](https://github.com/gdufeZLYL/blog/blob/master/images/20180508091226.png)

#### 内存重分配操作是一个比较耗时的操作
![内存重分配操作是一个比较耗时的操作](https://github.com/gdufeZLYL/blog/blob/master/images/20180508091507.png)

#### SDS中的空间预分配和惰性空间释放策略
> 为了避免C字符串的这种缺陷,SDS通过未使用空间解除了字符串长度和底层数组长度之间的关联: 在SDS中,buf数组的长度不一定就是字符数量加一,数组里面可以包含未使用的字节,而这些字节的数量就由SDS的free属性记录.通过未使用空间,SDS实现了空间预分配和惰性空间释放两种优化策略.

##### 空间预分配
> 空间预分配用于优化SDS的字符串增长操作:当SDS的API对一个SDS进行修改,并且需要对SDS进行空间扩展的时候,程序不仅会为SDS分配修改所必须要的空间,还会为分配额外的未使用空间.

额外分配的未使用空间数量的公式:
```Python
if 新SDS的len < 1MB:
    新SDS的free = 新SDS的len
elif 新SDS的len >= 1MB:
    新SDS的free = 1MB
新SDS的buf数组的len = 新SDS的free + 新SDS的len + 1
```
例子：

![例子1](https://github.com/gdufeZLYL/blog/blob/master/images/20180508094203.png)

![例子2](https://github.com/gdufeZLYL/blog/blob/master/images/20180508094240.png)

例子：

原来:

![原来](https://github.com/gdufeZLYL/blog/blob/master/images/20180508094424.png)

sdscat(s, " Cluster")后:

![sdscat(s, " Cluster")后](https://github.com/gdufeZLYL/blog/blob/master/images/20180508103308.png)

sdscat(s, " Tutorial")后:

![sdscat(s, " Tutorial")后](https://github.com/gdufeZLYL/blog/blob/master/images/20180508103401.png)

在扩展SDS空间之前,SDS API会先检查未使用空间是否足够,如果足够的话,API就会直接使用未使用空间,而无须执行内存重分配.
通过这种预分配策略,SDS将连续增长N次字符串所需的内存重分配次数从必定N次降低为最多N次.

##### 惰性空间释放
> 惰性空间释放用于优化SDS的字符串缩短操作:当SDS的API需要缩短SDS保存的字符串时,程序并不立即使用内存重分配来回收缩短后多出来的字节,而是使用free属性将这些字节的数量记录起来,并等待将来使用.

例子:

原来:

![原来](https://github.com/gdufeZLYL/blog/blob/master/images/20180508105259.png)

sdstrim(s, "XY")后:

![sdstrim(s, "XY")后](https://github.com/gdufeZLYL/blog/blob/master/images/20180508105516.png)

sdstrim(s, "XY")后:

![sdstrim(s, "XY")后](https://github.com/gdufeZLYL/blog/blob/master/images/20180508105516.png)

sdscat(s, " Redis")后:

![sdscat(s, " Redis")后](https://github.com/gdufeZLYL/blog/blob/master/images/20180508105842.png)

### 二进制安全

#### C字符串的缺陷

![C字符串的缺陷](https://github.com/gdufeZLYL/blog/blob/master/images/20180508110916.png)

如下图:C字符串所用的函数只会识别出其中的"Redis",忽略之后的"Cluster"

![如下图](https://github.com/gdufeZLYL/blog/blob/master/images/20180508111137.png)

#### SDS的buf属性为字节数组
> SDS的buf属性称为字节数组——Redis不是用这个数组来保存字符,而是用它来保存一系列二进制数据

例子：

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180508111606.png)

### 兼容部分C字符串函数
### 总结

![SDS和C字符串的区别](https://github.com/gdufeZLYL/blog/blob/master/images/20180508111935.png)

## SDS的API

![SDS的API1](https://github.com/gdufeZLYL/blog/blob/master/images/20180508112109.png)

![SDS的API2](https://github.com/gdufeZLYL/blog/blob/master/images/20180508112140.png)