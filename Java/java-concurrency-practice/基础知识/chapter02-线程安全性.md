# 前言
## 编写线程安全代码的核心
要编写线程安全的代码,其核心在于要对状态访问操作进行管理,特别是对共享的(Shared)和可变的(Mutable)状态的访问.

## 对象的状态
从非正式的意义上来说,对象的状态是指存储在状态变量(例如实例或静态域)中的数据.对象的状态可能包括其他依赖对象的域.例如,某个HashMap的状态不仅存储在HashMap对象本身,还存储在许多Map.Entry对象中.在对象的状态中包含了任何可能影响其外部可见行为的数据.

### 共享与可变
"共享"意味着变量可以由多个线程同时访问,而"可变"则意味着变量的值在其生命周期内可以发生变化.讨论线程安全性,更侧重于如何防止在数据上发生不受控的并发访问.
* 一个对象是否需要是线程安全的,取决于它是否被多个线程访问.
* 当多个线程访问某个状态变量并且其中有一个线程执行写入操作时,必须采用同步机制来协同这些线程对变量的访问.

如果当多个线程访问同一个可变的状态变量时没有使用合适的同步,那么程序就会出现错误.有三种方式可以修复这个问题:
* 不在线程之间共享该状态变量.
* 将状态变量修改为不可变的变量.
* 在访问状态变量时使用同步.

注意:
完全由线程安全类构造的程序并不一定就是线程安全的,而在线程安全类中也可以包含非线程安全的类.

# 什么是线程安全性
当多个线程访问某个类时,这个类始终都能表现出正确的行为,那么就称这个类是线程安全的.

> 当多个线程访问某个类时,不管运行时环境采用何种调度方式或者这些线程将如何交替执行,并且在主调代码中不需要任何额外的同步或协同,这个类都能表现出正确的行为,那么就称这个类是线程安全的.

示例: 一个无状态的Servlet

![一个无状态的Servlet](https://github.com/gdufeZLYL/blog/blob/master/images/20180527113806.png)

![一个无状态的Servlet](https://github.com/gdufeZLYL/blog/blob/master/images/20180527113915.png)

# 原子性

![错误的示例](https://github.com/gdufeZLYL/blog/blob/master/images/20180527114957.png)

![错误的示例](https://github.com/gdufeZLYL/blog/blob/master/images/20180527115029.png)

![错误的示例](https://github.com/gdufeZLYL/blog/blob/master/images/20180527115059.png)

## 竞态条件
> 在并发编程中,这种由于不恰当的执行时序而出现不正确的结果是一种非常重要的情况.

### "先检查后执行"操作
最常见的竞态条件类型就是"先检查后执行"操作,即通过一个可能失效的观测结果来决定下一步的动作.

> 首先观察到某个条件为真(例如文件X不存在),然后根据这个观察结果采用相应的动作(创建文件X),但事实上,在你观察到这个结果以及开始创建文件之间,观察结果可能变得无效(另一个线程在这期间创建了文件X),从而导致各种问题(未预期的异常、数据被覆盖、文件被破坏等)

## 示例: 延迟初始化中的竞态条件

![延迟初始化中的竞态条件](https://github.com/gdufeZLYL/blog/blob/master/images/20180527150212.png)

![延迟初始化中的竞态条件](https://github.com/gdufeZLYL/blog/blob/master/images/20180527150424.png)

![延迟初始化中的竞态条件](https://github.com/gdufeZLYL/blog/blob/master/images/20180527150611.png)

## 复合操作
LazyInitRace和UnsafeCountingFactorizer都包含一组需要以原子方式执行(或者说不可分割)的操作.要避免竞态条件问题,就必须在某个线程修改该变量时,通过某种方式防止其他线程使用这个变量,从而确保其他线程只能在修改操作完成之前或之后读取和修改状态,而不是在修改状态的过程中.

原子方式
> 假定有两个操作A和B,如果从执行A的线程来看,当另一个线程执行B时,要么将B全部执行完,要么完全不执行B,那么A和B对彼此来说是原子的.原子操作是指,对于访问同一个状态的所有操作(包括该操作本身)来说,这个操作是一个以原子方式执行的操作.

使用AtomicLong类型的变量来统计已处理请求的数量:

![使用AtomicLong类型的变量来统计已处理请求的数量](https://github.com/gdufeZLYL/blog/blob/master/images/20180527151645.png)

![使用AtomicLong类型的变量来统计已处理请求的数量](https://github.com/gdufeZLYL/blog/blob/master/images/20180527151805.png)

![使用AtomicLong类型的变量来统计已处理请求的数量](https://github.com/gdufeZLYL/blog/blob/master/images/20180527151826.png)

# 加锁机制
一个错误的例子:

![一个错误的例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180527154209.png)

![一个错误的例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180527154230.png)

![一个错误的例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180527154249.png)

![一个错误的例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180527154334.png)

## 内置锁
Java提供了一种内置的锁机制支持原子性: 同步代码块(Synchronized Block).同步代码块包括两部分:
* 一个作为锁的对象引用: 以关键字synchronized来修饰的方法就是一种横跨整个方法体的同步代码块,其中该同步代码块的锁就是方法调用所在的对象.静态的synchronized方法以Class对象作为锁.
* 一个作为由这个锁保护的代码块: 
```java
synchronized (lock) {
    // 访问或修改由锁保护的共享状态
}
```
每个Java对象都可以用做一个实现同步的锁,这些锁被称为内置锁(Intrinsic)或监视器锁(Monitor Lock).线程在进入同步代码块之前会自动获得锁,并且在退出同步代码块时自动释放锁,而无论是通过正常的控制路径退出,还是通过从代码块中抛出异常退出.活得内置锁的唯一途径就是进入由这个锁保护的同步代码块或方法.

Java的内置锁相当于一种互斥体(或互斥锁),这意味着最多只有一个线程能持有这种锁.当线程A尝试获取一个由线程B持有的锁时,线程A必须等待或者阻塞,直到线程B释放这个锁.如果B永远不释放锁,那么A也将永远地等下去.

由于每次只能有一个线程执行内置锁保护的代码块,因此,由这个锁保护的同步代码块会以原子方式执行,多个线程在执行该代码块时也不会相互干扰.并发环境中的原子性与事务应用程序中的原子性有着相同的含义——一组语句作为一个不可分割的单元被执行.任何一个执行同步代码块的线程,都不可能看到有其他线程正在执行由同一个锁保护的同步代码块.

一个并发性非常槽糕的例子:

![一个并发性非常槽糕的例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180527160954.png)

![一个并发性非常槽糕的例子](https://github.com/gdufeZLYL/blog/blob/master/images/20180527161010.png)

## 重入

![重入](https://github.com/gdufeZLYL/blog/blob/master/images/20180527161411.png)

![重入](https://github.com/gdufeZLYL/blog/blob/master/images/20180527161441.png)

# 用锁来保护状态

![用锁来保护状态](https://github.com/gdufeZLYL/blog/blob/master/images/20180527163344.png)

![用锁来保护状态](https://github.com/gdufeZLYL/blog/blob/master/images/20180527163408.png)

![用锁来保护状态](https://github.com/gdufeZLYL/blog/blob/master/images/20180527163434.png)

![用锁来保护状态](https://github.com/gdufeZLYL/blog/blob/master/images/20180527163457.png)

![用锁来保护状态](https://github.com/gdufeZLYL/blog/blob/master/images/20180527163515.png)

![用锁来保护状态](https://github.com/gdufeZLYL/blog/blob/master/images/20180527163530.png)


# 活跃性与性能

![活跃性与性能](https://github.com/gdufeZLYL/blog/blob/master/images/20180527172526.png)

![活跃性与性能](https://github.com/gdufeZLYL/blog/blob/master/images/20180527172545.png)

![活跃性与性能](https://github.com/gdufeZLYL/blog/blob/master/images/20180527172615.png)

![活跃性与性能](https://github.com/gdufeZLYL/blog/blob/master/images/20180527172728.png)

![活跃性与性能](https://github.com/gdufeZLYL/blog/blob/master/images/20180527172816.png)

![活跃性与性能](https://github.com/gdufeZLYL/blog/blob/master/images/20180527172839.png)

![活跃性与性能](https://github.com/gdufeZLYL/blog/blob/master/images/20180527172855.png)

![活跃性与性能](https://github.com/gdufeZLYL/blog/blob/master/images/20180527172908.png)