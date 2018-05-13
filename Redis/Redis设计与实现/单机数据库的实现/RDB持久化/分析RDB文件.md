# 不包含任何键值对的RDB文件
创建一个数据库状态为空的RDB文件:
```
redis> FLUSHALL
OK

redis> SAVE
OK
```
然后调用od命令,打印RDB文件:
```
$ od -c dump.rdb
0000000  R E D I S 0 0 0 6 377 334 263 C 360 Z 334
0000020 362 V
0000022
```
当一个RDB文件没有包含任何数据库数据时,这个RDB文件将由以下四个部分组成:
* 五个字节的"REDIS"字符串.
* 四个字节的版本号(db_version).
* 一个字节的EOF常量.
* 八个字节的校验和(check_sum).

分析:
从od命令的输出中可以看出,最开头的是"REDIS"字符串,之后的0006是版本号,再之后的一个字节377代表EOF常量,最后的334 263 C 360 Z 334 362 V八个字节则代表RDB文件的校验和.

# 包含字符串键的RDB文件

![创建与od](https://github.com/gdufeZLYL/blog/blob/master/images/20180513154639.png)

分析:

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180513154741.png)

# 包含带有过期时间的字符串键的RDB文件

![创建与od](https://github.com/gdufeZLYL/blog/blob/master/images/20180513160118.png)

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180513160154.png)

# 包含一个集合键的RDB文件

![创建与od](https://github.com/gdufeZLYL/blog/blob/master/images/20180513161402.png)

分析:

![分析](https://github.com/gdufeZLYL/blog/blob/master/images/20180513161433.png)

# 关于分析RDB文件的说明

![关于分析RDB文件的说明](https://github.com/gdufeZLYL/blog/blob/master/images/20180513162810.png)

