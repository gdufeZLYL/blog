# 什么是跳跃表
[漫画：什么是跳跃表？](https://mp.weixin.qq.com/s?__biz=MzIxMjE5MTE1Nw==&mid=2653190879&idx=1&sn=1916d0f6e72f34408261d70d13eecf5b&chksm=8c990805bbee81137dd6cadbe7b69cf84020233385cc5d7cee778d10977b6f9b28ea235b93e0&mpshare=1&scene=1&srcid=0509EUqFZD0rZJ0Vn1gbqQZB#rd)

# 跳跃表的实现

![一个跳跃表](https://github.com/gdufeZLYL/blog/blob/master/images/20180509092704.png)

图片最左边的是zskiplist结构,该结构包含以下属性:
* header: 指向跳跃表的表头节点.
* tail: 指向跳跃表的表尾节点.
* level: 记录目前跳跃表内,层数最大的那个节点的层数(表头节点的层数不计算在内).
* length: 记录跳跃表的长度,也即是,跳跃表目前包含节点的数量(表头节点不计算在内).

位于zskiplist结构右方的是四个zskiplistNode结构,该结构包含以下属性:
* 层(level): 节点中用L1、L2、L3等字样标记节点的各个层,L1代表第一层,L2代表第二层,以此类推.每个层都带有两个属性:前进指针和跨度.前进指针用于访问位于表尾方向的其他节点,而跨度则记录了前进指针所指向节点和当前节点的距离.在上面的图片中,连线上带有数字的箭头就代表前进指针,而那个数字就是跨度.当程序从表头向表尾进行遍历时,访问会沿着层的前进指针进行.
* 后退(backward)指针: 节点中用BW字样标记节点的后退指针,它指向位于当前节点的前一个节点.后退指针在程序从表尾向表头遍历时使用.
* 分值(score): 各个节点中的1.0、2.0和3.0是节点所保存的分值.在跳跃表中,节点按各自所保存的分值从小到大排列.
* 成员对象(obj): 各个节点中的o1、o2和o3是节点所保存的成员对象.

## 跳跃表节点
```c++
typedef struct zskiplistNode {
    // 层
    struct zskiplistLevel {
        // 前进指针
        struct zskiplistNode *forward;

        // 跨度
        unsigned int span;
    } level[];

    // 后退指针
    struct zskiplistNode *backward;

    // 分值
    double score;

    // 成员对象
    robj *obj;
} zskiplistNode;
```

### 层
![层](https://github.com/gdufeZLYL/blog/blob/master/images/20180509113152.png)

### 前进指针
![前进指针](https://github.com/gdufeZLYL/blog/blob/master/images/20180509151554.png)

![带有不同层高的节点](https://github.com/gdufeZLYL/blog/blob/master/images/20180509151629.png)

![遍历整个跳跃表](https://github.com/gdufeZLYL/blog/blob/master/images/20180509151716.png)

![遍历节点过程](https://github.com/gdufeZLYL/blog/blob/master/images/20180509152017.png)

### 跨度
![跨度](https://github.com/gdufeZLYL/blog/blob/master/images/20180509152306.png)

![计算节点的排位](https://github.com/gdufeZLYL/blog/blob/master/images/20180509152646.png)

![另一个计算节点排位的例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180509152750.png)

### 后退指针
![后退指针](https://github.com/gdufeZLYL/blog/blob/master/images/20180509153354.png)

### 分值和成员
![分值和成员](https://github.com/gdufeZLYL/blog/blob/master/images/20180509155110.png)

![分值和成员](https://github.com/gdufeZLYL/blog/blob/master/images/20180509155225.png)

![分值和成员](https://github.com/gdufeZLYL/blog/blob/master/images/20180509155759.png)

## 跳跃表
### 跳跃表的数据结构
zskiplist结构的定义如下:
```c++
typedef struct zskiplist {
    // 表头节点和表尾节点
    struct zskiplistNode *header, *tail;

    // 表中节点的数量
    unsigned long length;

    // 表中层数最大的节点的层数
    int level;
} zskiplist;
```

![跳跃表](https://github.com/gdufeZLYL/blog/blob/master/images/20180509161335.png)

![跳跃表](https://github.com/gdufeZLYL/blog/blob/master/images/20180509161423.png)

# 跳跃表API
![跳跃表API](https://github.com/gdufeZLYL/blog/blob/master/images/20180509161532.png)
