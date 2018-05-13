# RDB文件结构前瞻
RDB文件结构:

![RDB文件结构](https://github.com/gdufeZLYL/blog/blob/master/images/20180513100209.png)

说明:
* REDIS

![REDIS](https://github.com/gdufeZLYL/blog/blob/master/images/20180513100251.png)

* db_version

[db_version](https://github.com/gdufeZLYL/blog/blob/master/images/20180513100451.png)

* databases

![databases](https://github.com/gdufeZLYL/blog/blob/master/images/20180513100547.png)

* EOF

![EOF](https://github.com/gdufeZLYL/blog/blob/master/images/20180513100650.png)

* check_num

![check_num](https://github.com/gdufeZLYL/blog/blob/master/images/20180513100734.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180513100820.png)

# databases部分
> 一个RDB文件的databases部分可以保存任意多个非空数据库

例子说明:

![例子说明1](https://github.com/gdufeZLYL/blog/blob/master/images/20180513101534.png)

![例子说明2](https://github.com/gdufeZLYL/blog/blob/master/images/20180513101652.png)

# key_value_pairs部分

TYPE类型:

![key_value_pairs部分](https://github.com/gdufeZLYL/blog/blob/master/images/20180513102828.png)

不带有过期时间的键值对:

![key_value_pairs部分](https://github.com/gdufeZLYL/blog/blob/master/images/20180513102943.png)

带有过期时间的键值对:

![key_value_pairs部分](https://github.com/gdufeZLYL/blog/blob/master/images/20180513103233.png)

# value的编码
> RDB文件中的每个value部分都保存了一个值对象,每个值对象的类型都由与之对应的TYPE记录,根据类型的不同,value部分的结构、长度也会有所不同.

## 字符串对象
TTPE的值为REDIS_RDB_TYPE_STRING, 字符串的对象的编码可以是REDIS_ENCODING_INT或者REDIS_ENCODING_RAW.
### REDIS_ENCODING_INT
> 如果字符串对象的编码为REDIS_ENCODING_INT,那么说明对象中保存的是长度不超过32位的整数,这种编码的对象将以图10-20所示的结构保存.

![INT编码字符串对象的保存结构](https://github.com/gdufeZLYL/blog/blob/master/images/20180513110948.png)

ENCODING的值:

![ENCODING的值](https://github.com/gdufeZLYL/blog/blob/master/images/20180513111132.png)

例子:

![用8位来保存整数的例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180513111213.png)

### REDIS_ENCODING_RAW
如果字符串对象的编码为REDIS_ENCODING_RAW,那么说明对象所保存的是一个字符串值,根据字符串长度的不同,有压缩和不压缩两种方法来保存这个字符串:
* 如果字符串的长度小于等于20字节,那么这个字符串会直接被原样保存.
* 如果字符串的长度大于20字节,那么这个字符串会被压缩之后再保存.

注意:
> 以上两个条件是在假设服务器打开了RDB文件压缩功能的情况下进行的,如果服务器关闭了RDB文件压缩功能,那么RDB程序总以无压缩的方式保存字符串值.具体信息可以参考redis.conf文件中关于rdbcompression选项的说明.

无压缩字符串的保存结构:

![无压缩字符串的保存结构](https://github.com/gdufeZLYL/blog/blob/master/images/20180513111827.png)

例子: 一个保存无压缩字符串,其中字符串的长度为5,字符串的值为"hello"

![无压缩的字符串](https://github.com/gdufeZLYL/blog/blob/master/images/20180513112011.png)

压缩后字符串的保存结构:

![压缩后字符串的保存结构](https://github.com/gdufeZLYL/blog/blob/master/images/20180513112331.png)

结构分析：

![压缩后字符串的保存结构分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180513112413.png)

例子: 一个压缩后的字符串示例,字符串原本的长度为21,压缩之后的长度为6,压缩之后的字符串内容为"?aa???",其中?代表的是无法用字符串形式打印出来的字节.

![压缩后字符串的保存结构分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180513112413.png)

## 列表对象
如果TYPE的值为REDIS_RDB_TYPE_LIST,那么value保存的就是一个REDIS_ENCODING_LINKEDLIST编码的列表对象,RDB文件保存这种对象的结构如图10-26所示:

![LINKEDLIST编码列表对象的保存结构](https://github.com/gdufeZLYL/blog/blob/master/images/20180513113243.png)

分析:

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180513113332.png)

例子: 一个包含三个元素的列表

![一个包含三个元素的列表](https://github.com/gdufeZLYL/blog/blob/master/images/20180513113425.png)

## 集合对象
如果TYPE的值为REDIS_RDB_TYPE_SET,那么value保存的就是一个REDIS_ENCODING_HT编码的集合对象,RDB文件保存这种对象的结构如图10-28所示:

![HT编码集合对象的保存结构](https://github.com/gdufeZLYL/blog/blob/master/images/20180513113722.png)

分析:

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180513113757.png)

例子: 一个包含四个元素的集合

![一个包含四个元素的集合](https://github.com/gdufeZLYL/blog/blob/master/images/20180513114132.png)

## 哈希表对象
如果TYPE的值为REDIS_RDB_TYPE_HASH,那么value保存的就是一个REDIS_ENCODING_HT编码的集合对象,RDB文件保存这种对象的结构如图10-30所示:
* hash_size记录了哈希表的大小,也即是这个哈希表保存了多少键值对,读入程序可以通过这个大小知道自己应该读入多少个键值对.
* 以key_value_pair开头的部分代表哈希表中的键值对,键值对的键和值都是字符串对象,所以程序会以处理字符串对象的方式来保存和读入键值对.

HT编码哈希表对象的保存结构:

![HT编码哈希表对象的保存结构](https://github.com/gdufeZLYL/blog/blob/master/images/20180513114919.png)

例子: 一个包含两个键值对的哈希表

![一个包含两个键值对的哈希表](https://github.com/gdufeZLYL/blog/blob/master/images/20180513115040.png)

## 有序集合对象
如果TYPE的值为REDIS_RDB_TYPE_ZSET,那么value保存的就是一个REDIS_ENCODING_SKIPLIST编码的有序集合对象,RDB文件保存这种对象的结构如图10-34所示:

![SKIPLIST编码有序集合对象的保存结构](https://github.com/gdufeZLYL/blog/blob/master/images/20180513150532.png)

分析:

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180513150706.png)

例子: 一个带有两个元素的有序集合

![一个带有两个元素的有序集合](https://github.com/gdufeZLYL/blog/blob/master/images/20180513150955.png)

## INTSET编码的集合
如果TYPE的值为REDIS_RDB_TYPE_SET_INTSET,那么value保存的就是一个整数集合对象,RDB文件保存这种对象的方法是,先将整数集合转换为字符串对象,然后将这个字符串对象保存到RDB文件里面.

如果程序在读入RDB文件的过程中,碰到由整数集合对象转换成的字符串对象,那么程序会根据TYPE值的指示,先读入字符串对象,将这个字符串对象转换成原来的整数集合对象.

## ZIPLIST编码的列表、哈希表或者有序集合
如果TYPE的值为REDIS_RDB_TYPE_LIST_ZIPLIST、REDIS_RDB_TYPE_HASH_ZIPLIST或者REDIS_RDB_TYPE_ZSET_ZIPLIST,那么value保存的就是一个压缩列表对象,RDB文件保存这种对象的方法是:
1. 将压缩列表转换成一个字符串对象.
2. 将转换所得的字符串对象保存到RDB文件.

如果程序在读入RDB文件的过程中,碰到由压缩列表对象转换成的字符串对象,那么程序会根据TYPE值的指示,执行以下操作:
1. 读入字符串对象,并将它转换成原来的压缩列表对象.
2. 根据TYPE的值,设置压缩列表对象的类型: 如果TYPE的值为REDIS_RDB_TYPE_LIST_ZIPLIST,那么压缩列表对象的类型为列表;如果TYPE的值为REDIS_RDB_TYPE_HASH_ZIPLIST,那么压缩列表对象的类型为哈希表;如果TYPE的值为REDIS_RDB_TYPE_ZSET_ZIPLIST,那么压缩列表对象的类型为有序集合.

由于TYPE的存在,即使列表、哈希表和有序集合三种类型都使用压缩列表来保存,RDB读入程序也总可以将读入并转换之后得出的压缩列表设置成原来的类型.

