# 生成RDB文件
![生成RDB文件](https://github.com/gdufeZLYL/blog/blob/master/images/20180512103227.png)

# 载入RDB文件
![载入RDB文件](https://github.com/gdufeZLYL/blog/blob/master/images/20180512103318.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180512103405.png)

# AOF文件写入
![AOF文件写入](https://github.com/gdufeZLYL/blog/blob/master/images/20180512104036.png)

# AOF重写
![AOF重写](https://github.com/gdufeZLYL/blog/blob/master/images/20180512104116.png)

# 复制
当服务器运行在复制模式下时,从服务器的过期键删除动作由主服务器控制:
* 主服务器在删除一个过期键之后,会显示地向所有从服务器发送一个DEL命令,告知从服务器删除这个过期键.
* 从服务器在执行客户端发送的读命令时,即使碰到过期键也不会将过期键删除,而是继续像处理未过期的键一样来处理过期键.
* 从服务器只有在接到主服务器发来的DEL命令之后,才会删除过期键.

通过由主服务器来控制从服务器统一地删除过期键,可以保证主从服务器数据的一致性,也正是因为这个原因,当一个过期键仍然存在于主服务器的数据库时,这个过期键在从服务器里的复制品也会继续存在.

## 例子

有一对主从服务器,它们的数据库中都保存着同样的三个键message、xxx和yyy,其中message为过期键.如图9-17所示:

![主从服务器删除过期键(1)](https://github.com/gdufeZLYL/blog/blob/master/images/20180512110804.png)

如果这时有客户端向从服务器发送命令GET message, 那么从服务器将发现message键已经过期,但从服务器并不会删除message键, 而是继续将message键的值返回给客户端,就好像message键并没有过期一样,如图9-18所示:

![主从服务器删除过期键(2)](https://github.com/gdufeZLYL/blog/blob/master/images/20180512121029.png)

假设在此之后,有客户端向主服务器发送命令GET message, 那么主服务器将发现键message已经过期: 主服务器会删除message键,向客户端返回空回复,并向从服务器发送DEL message命令,如图9-19所示:

![主从服务器删除过期键(3)](https://github.com/gdufeZLYL/blog/blob/master/images/20180512122233.png)

从服务器在接收到主服务器发来的DEL message命令之后,也会从数据库中删除message键,在这之后,主从服务器都不再保存过期键message了,如图9-20所示:

![主从服务器删除过期键(4)](https://github.com/gdufeZLYL/blog/blob/master/images/20180512122426.png)