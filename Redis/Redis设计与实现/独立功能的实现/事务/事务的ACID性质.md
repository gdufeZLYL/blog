# 事务的ACID性质
在传统的关系式数据库中,常常用ACID性质来检验事务功能的可靠性和安全性.

在Redis中,事务总是具有原子性(Atomicity)、一致性(Consistency)和隔离性(Isolation),并且当Redis运行在某种特定的持久化模式下时,事务也具有耐久性(Durability).

# 原子性
事务具有原子性指的是,数据库将事务中的多个操作当作一个整体来执行,服务器要么就执行事务中的所有操作,要么就一个操作也不执行.

对于Redis的事务功能来说,事务队列中的命令要么就全部都执行,要么就一个都不执行,因此,Redis的事务是具有原子性的.

一个成功执行的事务的例子:

![一个成功执行的事务的例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519092358.png)

一个失败执行的事务的例子:

![一个执行失败的事务的例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519092430.png)

Redis不支持事务回滚机制:
> Redis的事务和传统的关系型数据库事务的最大区别在于,Redis不支持事务回滚机制(rollback),即使事务队列中的某个命令在执行期间出现了错误,整个事务也会继续执行下去,直到将事务队列中的所有命令都执行完毕为止.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519092821.png)

关于为什么不支持回滚机制:

![关于为什么不支持回滚机制](https://github.com/gdufeZLYL/blog/blob/master/images/20180519092946.png)

# 一致性
事务具有一致性指的是,如果数据库在执行事务之前是一致的,那么在事务执行之后,无论事务是否执行成功,数据库也应该仍然是一致的.

"一致"指的是数据符合数据库本身的定义和要求,没有包含非法或者无效的错误数据.

## 入队错误
如果一个事务在入队命令的过程中,出现了命令不存在,或者命令的格式不正确等情况,那么Redis将拒绝执行这个事务.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519093924.png)

Redis 2.6.5以前的入队错误处理:

![Redis 2.6.5以前的入队错误处理](https://github.com/gdufeZLYL/blog/blob/master/images/20180519094018.png)

![Redis 2.6.5以前的入队错误处理](https://github.com/gdufeZLYL/blog/blob/master/images/20180519094042.png)

## 执行错误
除了入队时可能发生错误以外,事务还可能在执行的过程中发生错误.

关于这种错误有两个需要说明的地方:
* 执行过程中发生的错误都是一些不能再入队时被服务器发现的错误,这些错误只会在命令实际执行时被触发.
* 即使在事务的执行过程中发生了错误,服务器也不会中断事务的执行,它会继续执行事务中余下的其他命令,并且已执行的命令(包括执行命令所产生的结果)不会被出错的命令影响.

对数据库键执行了错误类型的操作是事务执行期间最常见的错误之一.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180519095055.png)

因为在事务执行的过程中,出错的命令会被服务器识别出来,并进行相应的错误处理,所以这些出错命令不会对数据库做任何修改,也不会对事务的一致性产生任何影响.

## 服务器停机

![服务器停机](https://github.com/gdufeZLYL/blog/blob/master/images/20180519095527.png)

![服务器停机](https://github.com/gdufeZLYL/blog/blob/master/images/20180519095552.png)

# 隔离性
事务的隔离性指的是,即使数据库中有多个事务并发地执行,各个事务之间也不会互相影响,并且在并发状态下执行的事务和串行执行的事务产生的结果完全相同.

因为Redis使用单线程的方式来执行事务(以及事务队列中的命令),并且服务器保证,在执行事务期间不会对事务进行中断,因此,Redis的事务总是以串行的方式运行的,并且事务也总是具有隔离性的.

# 耐久性
事务的耐久性指的是,当一个事务执行完毕时,执行这个事务所得的结果已经被保存到永久性存储介质(比如硬盘)里面了,即使服务器在事务执行完毕之后停机,执行事务所得的结果也不会丢失.

![耐久性](https://github.com/gdufeZLYL/blog/blob/master/images/20180519101137.png)

![耐久性](https://github.com/gdufeZLYL/blog/blob/master/images/20180519101203.png)

![耐久性](https://github.com/gdufeZLYL/blog/blob/master/images/20180519101713.png)
