PSYNC命令的调用方法(两种):
* 如果从服务器以前没有复制过任何主服务器,或者之前执行过SLAVEOF no one命令,那么从服务器在开始一次新的复制时将向主服务器发送PSYNC ? -1命令,主动请求主服务器进行完整重同步(因为这时不可能执行部分重同步).
* 相反地,如果从服务器已经复制过某个主服务器,那么从服务器在开始一次新的复制时将向主服务器发送`PSYNC <runid> <offset>`命令: 其中runid是上一次复制的主服务器的运行ID,而offset则是从服务器当前的复制偏移量,接收到这个命令的主服务器会通过这两个参数来判断应该对从服务器执行哪种同步操作.

根据情况,接收到PSYNC命令的主服务器会向从服务器返回以下三种回复的其中一种:

![回复](https://github.com/gdufeZLYL/blog/blob/master/images/20180516134543.png)

![回复](https://github.com/gdufeZLYL/blog/blob/master/images/20180516134623.png)

流程图15-12总结了PSYNC命令执行完整重同步和部分重同步时可能遇上的情况:

![图15-12 PSYNC执行完整重同步和部分重同步时可能遇上的情况](https://github.com/gdufeZLYL/blog/blob/master/images/20180516134656.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180516135101.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180516135126.png)