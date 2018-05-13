## AOF文件的载入与数据还原
> 因为AOF文件里面包含了重建数据库状态所需的所有写命令,所以服务器只要读入并重新执行一遍AOF文件里面保存的写命令,就可以还原服务器关闭之前的数据库状态.

## Redis读取AOF文件并还原数据库状态的详细步骤:

![Redis读取AOF文件并还原数据库状态的详细步骤](https://github.com/gdufeZLYL/blog/blob/master/images/20180513175900.png)

## 例子
![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180513175935.png)