# ASK错误
在进行重新分片期间,源节点向目标节点迁移一个槽的过程中,可能会出现这样一种情况: 属于被迁移槽的一部分键值对保存在源节点里面,而另一部分键值对则保存在目标节点里面.

当客户端向源节点发送一个与数据库键有关的命令,并且命令要处理的数据库键恰好就属于正在被迁移的槽时:
* 源节点会现在自己的数据库里面查找指定键,如果找到的话,就直接执行客户端发送的命令.
* 相反地,如果源节点没能在自己的数据库里面找到指定的键,那么这个键有可能已经被迁移到了目标节点,源节点将向客户端返回一个ASK错误,指引客户端转向正在导入槽的目标节点,并再次发送之前想要执行的命令.

图17-26展示了源节点判断是否需要向客户端发送ASK错误的整个过程.

![图17-26 判断是否发送ASK错误的过程](https://github.com/gdufeZLYL/blog/blob/master/images/20180517213627.png)

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180517214106.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180517214128.png)

被隐藏的ASK错误:

![被隐藏的ASK错误](https://github.com/gdufeZLYL/blog/blob/master/images/20180517214218.png)

# CLUSTER SETSLOT IMPORTING命令的实现
clusterState结构的importing_slots_from数组记录了当前节点正在从其他节点导入的槽:
```c++
typedef struct clusterState {
    // ...

    clusterNode *importing_slots_from[16384];

    // ...
} clusterState;
```
如果importing_slots_from[i]的值不为NULL,而是指向一个clusterNode结构,那么表示当前节点正在从clusterNode所代表的节点导入槽i.

在对集群进行重新分片的时候,向目标节点发送命令:
```
CLUSTER SETSLOT <i> IMPORTING <source_id>
```
可以将目标节点clusterState.importing_slots_from[i]的值设置为source_id所代表节点的clusterNode结构.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180517222406.png)

![图17-27 节点7003的importing_slots_from数组](https://github.com/gdufeZLYL/blog/blob/master/images/20180517222431.png)

# CLUSTER SETSLOT MIGRATING命令的实现
clusterState结构的migrating_slots_to数组记录了当前节点正在迁移至其他节点的槽:
```c++
typedef struct clusterState {
    // ...

    clusterNode *migrating_slots_to[16384];

    // ...
} clusterState;
```
如果migrating_slots_to[i]的值不为NULL,而是指向一个clusterNode结构,那么表示当前节点正在将槽i迁移至clusterNode所代表的节点.

在对集群进行重新分片的时候,向源节点发送命令:
```
CLUSTER SETSLOT <i> MIGRATING <target_id>
```
可以将源节点clusterState.migrating_slots_to[i]的值设置为target_id所代表节点的clusterNode结构.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180517225824.png)

![图17-28 节点7002的migrating_slots_to数组](https://github.com/gdufeZLYL/blog/blob/master/images/20180517225903.png)

# ASK错误
如果节点收到一个关于键key的命令请求,并且键key所属的槽i正好就指派给了这个节点,那么节点会尝试在自己的数据库里查找键key,如果找到了的话,节点就直接执行客户端发送的命令.

与此相反,如果节点没有在自己的数据库里找到键key,那么节点会检查自己的clusterState.migrating_slots_to[i],看键key所属的槽i是否正在进行迁移,如果槽i的确在进行迁移的话,那么节点会向客户端发送一个ASK错误,引导客户端到正在导入槽i的节点去查找键key.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180517231741.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180517231921.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180517231953.png)

# ASKING命令
ASKING命令唯一要做的就是打开发送该命令的客户端的REDIS_ASKING标识,以下是该命令的伪代码实现:
```python
def ASKING():
    # 打开标识
    client.flags |= REDIS_ASKING

    # 向客户端返回OK回复
    reply("OK")
```
在一般情况下,如果客户端向节点发送一个关于槽i的命令,而槽i又没有指派这个节点的话,那么节点将向客户端返回一个MOVED错误;但是,如果节点的clusterState.importing_slots_from[i]显示节点正在导入槽i,并且发送命令的客户端带有REDIS_ASKING标识,那么节点将被破例执行这个槽i的命令一次,图17-31展示了这个判断过程.

![判断过程](https://github.com/gdufeZLYL/blog/blob/master/images/20180517234254.png)

当客户端接收到ASK错误并转向至正在导入槽的节点时,客户端会先向节点发送一个ASKING命令,然后才重新发送想要执行的命令,这是因为如果客户端不发送ASKING命令,而直接发送想要执行的命令的话,那么客户端发送的命令将被节点拒绝执行,并返回MOVED错误.

例子:

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180517235332.png)

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180517235402.png)

# ASK错误和MOVED错误的区别

![ASK错误和MOVED错误的区别](https://github.com/gdufeZLYL/blog/blob/master/images/20180518000001.png)