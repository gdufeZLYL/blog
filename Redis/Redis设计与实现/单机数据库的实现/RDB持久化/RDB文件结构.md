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

