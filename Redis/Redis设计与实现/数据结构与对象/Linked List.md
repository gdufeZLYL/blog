# 链表和链表节点的实现
## 链表节点的实现
```c++
typedef struct listNode {
    // 前置节点
    struct listNode *prev;

    // 后置节点
    struct listNode *next;

    // 节点的值
    void *value;
}listNode;
```

双端链表:

![](https://github.com/gdufeZLYL/blog/blob/master/images/20180508114456.png)

## 链表的实现
```c++
typedef struct list {
    // 表头节点
    listNode *head;

    // 表尾节点
    listNode *tail;

    // 链表所包含的节点数量
    unsigned long len;

    // 节点值复制函数
    void *(*dup)(void *ptr);

    // 节点值释放函数
    void *(*free)(void *ptr);

    // 节点值对比函数
    int (*match)(void *ptr, void *key);
} list;
```
list结构为链表提供了表头指针head、表尾指针tail,以及链表长度计数器len,而dup、free和match成员则是用于实现多态链表所需的类型特定函数：
* dup函数用于复制链表节点所保存的值;
* free函数用于释放链表节点所保存的值;
* match函数则用于对比链表节点锁保存的值和另一个输入值是否相等.

由list结构和listNode结构组成的链表:

![由list结构和listNode结构组成的链表](https://github.com/gdufeZLYL/blog/blob/master/images/20180508150242.png)

## Redis的链表实现的特性:

![Redis的链表实现的特性](https://github.com/gdufeZLYL/blog/blob/master/images/20180508150345.png)

# 链表和链表节点的API

![链表和链表节点的API1](https://github.com/gdufeZLYL/blog/blob/master/images/20180508150751.png)

![链表和链表节点的API2](https://github.com/gdufeZLYL/blog/blob/master/images/20180508150837.png)