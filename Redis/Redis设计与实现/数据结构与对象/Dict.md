# 字典的实现
> Redis的字典使用哈希表作为底层实现,一个哈希表里面可以有多个哈希表节点,而每个哈希表节点就保存了字典中的一个键值对.

## 哈希表
### 哈希表结构:
```c++
typedef struct dictht {
    // 哈希表数组
    dictEntry **table;

    // 哈希表大小
    unsigned long size;

    // 哈希表大小掩码,用于计算索引值
    // 总是等于size-1
    unsigned long sizemask;

    // 该哈希表已有节点的数量
    unsigned long used;
} dictht;
```
注：
* table属性是一个数组,数组中的每个元素都是一个指向dict.h/dictEntry结构的指针,每个dictEntry结构保存着一个键值对.
* size属性记录了哈希表的大小,也即是table数组的大小.
* used属性记录了哈希表目前已有节点(键值对)的数量.
* sizemask属性的值总是等于size-1,这个属性和哈希值一起决定一个键应该被放到table数组的哪个索引上面.

一个空的哈希表结构如下:

![一个空的哈希表结构](https://github.com/gdufeZLYL/blog/blob/master/images/20180508155952.png)

## 哈希表节点
### 哈希表节点结构:
```c++
typedef struct dictEntry {
    // 键
    void *key;

    // 值
    union {
        void *val;
        uint64_tu64;
        int64_ts64;
    } v;

    // 指向下个哈希表节点,形成链表
    struct dictEntry *next;
} dictEntry;
```
注:
* key属性保存着键值对中的键,而v属性则保存着键值对的值,其中键值对的值可以是一个指针,或者是一个uint64_t整数,又或者是一个int64_t整数.
* next属性是指向另一个哈希表节点的指针,这个指针可以将多个哈希值相同的键值对连接在一次,以此来解决键冲突的问题.

索引值相同的键k1和k0:

![索引值相同的键k1和k0](https://github.com/gdufeZLYL/blog/blob/master/images/20180508161301.png)

## 字典
### 字典结构
Redis中的字典由dict.h/dict结构表示:
```c++
typedef struct dict {
    // 类型特定函数
    dictType *type;

    // 私有数据
    void *privdata;

    // 哈希表
    dictht ht[2];

    // rehash索引
    // 当rehash不在进行时,值为-1
    int trehashidx; /*rehashing not in progress if rehashidx == -1 */
} dict;
```
注：
* type属性是一个指向dictType结构的指针,每个dictType结构保存了一簇用于操作特定类型键值对的函数,Redis会为用途不同的字典设置不同的类型特定函数.
* 而privdata属性则保存了需要传给那些类型特定函数的可选参数.
```c++
typedef struct dictType {
    // 计算哈希值的函数
    unsigned int (*hashFunction)(const void *key);

    // 复制键的函数
    void *(*keyDup)(void *privdata, const void *key);

    // 复制值的函数
    void *(*valDup)(void *privdata, const void *obj);

    // 对比键的函数
    int (*keyCompare)(void *privdata, const void *key1, const void *key2);

    // 销毁键的函数
    void (*keyDestructor)(void *privdata, void *key);

    // 销毁值的函数
    void (*valDestructor)(void *privdata, void *obj);
} dictType;
```
* ht属性是一个包含两个项的数组,数组中的每个项都是一个dictht哈希表,一般情况下,字典只使用ht[0]哈希表,ht[1]哈希表只会在对ht[0]哈希表进行rehash时使用.
* 除了ht[1]之外,另一个和rehash有关的属性就是rehashidx,它记录了rehash目前的进度,如果目前没有在进行rehash,那么它的值为-1.

普通状态下(没有进行rehash)的字典:

![普通状态下(没有进行rehash)的字典](https://github.com/gdufeZLYL/blog/blob/master/images/20180508165052.png)

# 哈希算法
将一个新的键值对添加到字典:
1. 根据键值对的键计算出哈希值和索引值;
2. 根据索引值,将包含新键值对的哈希表节点放到哈希表数组的指定索引上面.

Redis计算哈希值和索引值的方法:
```python
# 使用字典设置的哈希函数,计算键key的哈希值
hash = dict->type->hashFunction(key);

# 使用哈希表的sizemask属性和哈希值,计算出索引值
# 根据情况不同,ht[x]可以是ht[0]或者ht[1]
index = hash & dict->ht[x].sizemask;
```

例子：

一开始的空字典:

![一开始的空字典](https://github.com/gdufeZLYL/blog/blob/master/images/20180508175224.png)

如果我们要将一个键值对k0和v0添加到字典里面,那么程序会先使用语句:
```
hash = dict->type->hashFunction(k0);
```
计算键k0的哈希值.假设计算得出的哈希值为8,那么程序会继续使用语句:
```
index = hash & dict->ht[0].sizemask = 8 & 3 = 0;
```
计算出键k0的索引值0,这表示包含键值对k0和v0的节点应该被放置到哈希表数组的索引0位置上,如下图所示:

![添加键值对k0和v0之后的字典](https://github.com/gdufeZLYL/blog/blob/master/images/20180508175631.png)

Redis使用MurmurHash2算法来计算键的哈希值.

# 解决键冲突
## 键冲突
> 当有两个或以上数量的键被分配到了哈希表数组的同一个索引上面时,我们称这些键发生了冲突.

## 解决键冲突
> Redis的哈希表使用链地址法来解决键冲突,每个哈希表节点都有一个next指针,多个哈希表节点可以用next指针构成一个单向链表,被分配到同一个索引上的多个节点可以用这个单向链表连接起来,这就解决了键冲突的问题.

例子：

![例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180508213753.png)

![一个包含两个键值对的哈希表](https://github.com/gdufeZLYL/blog/blob/master/images/20180508213832.png)

![使用链表解决k2和k1的冲突](https://github.com/gdufeZLYL/blog/blob/master/images/20180508213919.png)

# rehash
## rehash的目的
> 随着操作的不断执行,哈希表保存的键值对会逐渐地增多或者减少,为了让哈希表的负载因子维持在一个合理的范围之内,当哈希表保存的键值对数量太多或者太少时,程序需要对哈希表的大小进行相应的扩展或者收缩.扩展和收缩哈希表的工作可以通过执行rehash(重新散列)操作来完成.

## rehash步骤
1. 为字典的ht[1]哈希表分配空间,这个哈希表的空间大小取决于要执行的操作,以及ht[0]当前包含的键值对数量(也即是ht[0].used属性的值):
```python
if 执行操作 = 扩展操作:
    ht[1]的大小 = 第一个大于等于ht[0].used*2的2^n(2的n次方幂)
elif 执行操作 = 收缩操作:
    ht[1]的大小 = 第一个大于等于ht[0].used的2^n
```
2. 将保存在ht[0]中的所有键值对rehash到ht[1]上面:rehash指的是重新计算键的哈希值和索引值,然后将键值对放置到ht[1]哈希表的指定位置上.
3. 当ht[0]包含的所有键值对都迁移到了ht[1]之后(ht[0]变为空表),释放ht[0],将ht[1]设置为ht[0],并在ht[1]新创建一个空白哈希表,为下一次rehash做准备.

## 例子

![执行rehash之前的字典](https://github.com/gdufeZLYL/blog/blob/master/images/20180508220222.png)

![为字典的ht[1]哈希表分配空间](https://github.com/gdufeZLYL/blog/blob/master/images/20180508223121.png)

![ht[0]的所有键值对都已经被迁移到ht[1]](https://github.com/gdufeZLYL/blog/blob/master/images/20180508223820.png)

![完成rehash之后的字典](https://github.com/gdufeZLYL/blog/blob/master/images/20180508223941.png)

## 哈希表的扩展与收缩
程序自动开始对哈希表执行扩展操作的条件(满足其一):
1. 服务器目前没有在执行BGSAVE命令或者BGREWRUTEAOF命令,并且哈希表的负载因子大于等于1.
2. 服务器目前正在执行BGSAVE命令或者BGREWRITEAOF命令,并且哈希表的负载因子大于等于5.

原因：
![原因](https://github.com/gdufeZLYL/blog/blob/master/images/20180508225246.png)

哈希表的负载因子公式:
```python
# 负载因子 = 哈希表已保存节点数量 / 哈希表大小
load_factor = ht[0].used / ht[0].size
```
注:
当哈希表的负载因子小于0.1时,程序自动开始对哈希表执行收缩操作.

# 渐进式rehash
> 扩展或收缩哈希表需要将ht[0]里面的所有键值对rehash到ht[1]里面,但是,这个rehash动作并不是一次性、集中式地完成的,而是分多次、渐进式地完成的.

原因:

![原因](https://github.com/gdufeZLYL/blog/blob/master/images/20180508230309.png)

## 哈希表渐进式rehash的详细步骤
1. 为ht[1]分配空间,让字典同时持有ht[0]和ht[1]两个哈希表
2. 在字典中维持一个索引计数器变量rehashidx,并将它的值设置为0,表示rehash工作正式开始.
3. 在rehash进行期间,每次对字典执行添加、删除、查找或者更新操作时,程序除了执行指定的操作以外,还会顺带将ht[0]哈希表在rehashidx索引上的所有键值对rehash到ht[1],当rehash工作完成之后,程序将rehashidx属性的值增一.
4. 随着字典操作的不断执行,最终在某个时间点上,ht[0]的所有键值对都会被rehash至ht[1],这时程序将rehashidx属性的值设为-1,表示rehash操作已完成.

