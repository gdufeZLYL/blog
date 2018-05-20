# LIMIT选项的实现
在默认情况下,SORT命令总会将排序后的所有元素都返回给客户端:
```
redis> SADD alphabet a b c d e f
(integer) 6

# 集合中的元素是乱序存放的
redis> SMEMBERS alphabet
"d"
"c"
"a"
"b"
"f"
"e"

# 对集合进行排序,并返回所有排序后的元素
redis> SORT alphabet ALPHA
"a"
"b"
"c"
"d"
"e"
"f"
```
但是,通过LIMIT选项,我们可以让SORT命令只返回其中一部分已排序的元素.

LIMIT选项的格式为`LIMIT <offset> <count>`:
* offset参数表示要跳过的已排序元素数量.
* count参数表示跳过给定数量的已排序元素之后,要返回的已排序元素数量.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520172339.png)

服务器执行SORT alphabet ALPHA LIMIT 0 4命令的详细步骤如下:

![服务器执行SORT alphabet ALPHA LIMIT 0 4命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520172454.png)

![图21-15 将obj指针指向集合的各个元素](https://github.com/gdufeZLYL/blog/blob/master/images/20180520173642.png)

![图21-16 排序后的数组](https://github.com/gdufeZLYL/blog/blob/master/images/20180520173730.png)

# GET选项的实现
在默认情况下,SORT命令在对键进行排序之后,总是返回被排序键本身所包含的元素.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520174130.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520174201.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180520174232.png)

服务器执行SORT students ALPHA GET *-name命令的详细步骤如下:

![服务器执行SORT students ALPHA GET *-name命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520181319.png)

![图21-17 排序之前的数组](https://github.com/gdufeZLYL/blog/blob/master/images/20180520181346.png)

![图21-18 排序之后的数组](https://github.com/gdufeZLYL/blog/blob/master/images/20180520181422.png)

![服务器执行SORT students ALPHA GET *-name命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520181454.png)

![服务器执行SORT students ALPHA GET *-name命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520181554.png)

![服务器执行SORT students ALPHA GET *-name命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520181639.png)

![服务器执行SORT students ALPHA GET *-name命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520181705.png)

# STORE选项的实现
在默认情况下,SORT命令只向客户端返回排序结果,而不保存排序结果:
```
redis> SADD students "peter" "jack" "tom"
(integer) 3

redis> SORT students ALPHA
"jack"
"peter"
"tom"
```
但是,通过使用STORE选项,我们可以将排序结果保存在指定的键里面,并在有需要时重用这个排序结果:
```
redis> SORT students ALPHA STORE sorted_students
(integer) 3

redis> LRANGE sorted_students 0 -1
"jack"
"peter"
"tom"
```
服务器执行SORT students ALPHA STORE sorted_students命令的详细步骤如下:

![服务器执行SORT students ALPHA STORE sorted_students命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520191209.png)

![服务器执行SORT students ALPHA STORE sorted_students命令的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180520191232.png)