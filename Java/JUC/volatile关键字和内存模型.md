# volatile关键字和内存模型

## 物理计算机的并发问题
### 原因
计算机的运算速度与它的存储和通信子系统速度的差距太大，大量的时间都花费在磁盘I/O,网络通信或数据库访问上。
### 如何衡量一个服务性能的高低好坏
每秒事务处理数(Transactions Per Second, TPS)是最重要的指标之一,它代表着一秒内服务端平均能响应的请求总数,而TPS值与程序的并发能力又有非常密切的关系。
### 高速缓存的出现
#### 原因
“让计算机并发执行若干个运算任务”与“更充分地利用计算机处理器”的一个复杂性来源：绝大多数的运算任务都不可能只靠处理器“计算”就能完成,处理器至少要与内存交互,如读取运算数据、存储运算结果等,这个I/O操作是很难消除的(无法紧靠寄存器来完成所有运算任务).
#### 高速缓存
由于计算机的存储设备与处理器的运算速度有几个数量级的差距,所有现代计算机系统都不得不加入一层读写速度尽可能接近处理器运算速度的高速缓存(Cache)来作为内存与处理器之间的缓冲:将运算需要使用到的数据复制到缓存中,让运算能快速进行,当运算结束后再从缓存同步回内存之中,这样处理器就无须等待缓慢的内存读写了.
### 缓存一致性(Cache Coherence)问题
在多处理器系统中,每个处理器都有自己的高速缓存,而它们又共享同一主内存(Main Memory),当多个处理器的运算任务都涉及同一块主内存区域时,将可能导致各自的缓存数据不一致.

### 内存模型
> 在特定的操作协议下,对特定的内存或高速缓存进行读写访问的过程抽象.

![处理器与高速缓存与主内存关系](https://github.com/gdufeZLYL/blog/blob/master/images/20180416221716.png)

### 乱序执行优化
处理器可能会对输入代码进行乱序执行(Out-Of-Order Execution)优化,处理器会在计算之后将乱序执行的结果重组,保证该结果与顺序执行的结果是一致的,但并不保证程序中各个语句计算的先后顺序与输入代码中的顺序一致.
***


## Java内存模型
> Java内存模型的主要目标是定义程序中各个变量的访问规则,即在虚拟机中将变量存储到内存和从内存中取出变量这样的底层细节.

*注意：
此处变量包括：
- [x] 实例字段、静态字段和构成数组对象的元素;
- [ ] 局部变量和方法参数(因为是线程私有的,不会被共享,自然就不会存在竞争)

### 主内存
* Java内存模型规定了所有的变量都存储在主内存(Main Memory)中.
* 线程间变量值的传递均需要通过主内存来完成.
* 主内存主要对应于Java堆中的对象实例数据部分.
### 工作内存
* 每条线程有自己的工作内存(Working Memory),线程的工作内存中保存了被该线程使用到的变量的主内存副本拷贝,线程对变量的所有操作(读取、赋值)都必须在工作内存中进行,而不能直接读写主内存中的变量.
* 工作内存对应于虚拟机栈中的部分区域.

![线程与主内存与工作内存关系](https://github.com/gdufeZLYL/blog/blob/master/images/20180416232405.png)

### 内存间交互操作
#### 8种操作：lock,unlock,read,load,use,assign,store,write
关于主内存与工作内存之间具体的交互协议,即一个变量如何从主内存拷贝到工作内存、如何从工作内存同步回主内存之类的实现细节,Java内存模型定义了一下8种操作(每一种操作都是原子的、不可再分的)来完成:
* **lock**(锁定)：作用于主内存的变量,它把一个变量标识为一个线程独占的状态.
* **unlock**(解锁)：作用于主内存的变量,它吧一个处于锁定状态的变量释放出来,释放后的变量才可以被其他线程锁定.
* **read**(读取)：作用于主内存的变量,它把一个变量的值从主内存传输到线程的工作内存中,以便随后的load动作使用.
* **load**(载入)：作用于工作内存的变量,它把read操作从主内存中得到的变量值放入工作内存的变量副本中.
* **use**(使用): 作用于工作内存的变量,它把工作内存中一个变量的值传递给执行引擎,每当虚拟机遇到一个需要使用到变量的值的字节码指令时将会执行这个操作.
* **assign**(赋值)：作用于工作内存的变量,它把一个从执行引擎接收到的值赋给工作内存的变量,每当虚拟机遇到一个给变量赋值的字节码指令时执行这个操作.
* **store**(存储)：作用于工作内存的变量,它把工作内存中一个变量的值传送到主内存中,以便随后的write操作使用.
* **write**(写入)：作用于主内存的变量,它把store操作从工作内存中得到的变量的值放入主内存的变量中.

*注意：
1. 一个变量从主内存复制到工作内存,必须顺序执行read和load操作.
2. 一个变量从工作内存同步回主内存,必须顺序执行store和write操作.
3. Java内存模型只要求上述两个操作必须按顺序执行,而没有保证是连续执行(即read与load之间、store与write之间是可插入其他指令).例如对主内存的变量a、b进行访问时,可能出现read a,read b,load b,load a.

#### 8种操作的执行规则：
* 不允许read和load、store和write操作之一单独出现：不允许一个变量从主内存读取了但工作内存不接受,或者从工作内存发起回写了但主内存不接受的情况出现.
* 不允许一个线程丢弃它的最近的assign操作：变量在工作内存中改变了之后必须把该变化同步回主内存.
* 不允许一个线程无原因地(没有发生过任何assign操作)把数据从线程的工作内存同步回主内存中.
* 对一个变量实施use、store操作之前,必须先执行过了assign和load操作：一个新的变量只能在主内存中“诞生”,不允许在工作内存中直接使用一个未被初始化(load或assign)的变量.
* 一个变量在同一个时刻只允许一条线程对其进行lock操作(lock操作可以被同一条线程执行多次,相应地只有执行相同次数的unlock操作,变量才会解锁).
* 如果对一个变量执行lock操作,那将会清空工作内存中此变量的值(在执行引擎使用这个变量前,需要重新执行load或assign操作初始化变量的值).
* 如果一个变量事先没有被lock操作锁定,那就不允许对它执行unlock操作,也不允许去unlock一个被其他线程锁定住的变量.
* 对一个变量执行unlock操作之前,必须先把此变量同步回主内存中(执行store、write操作).
***


## volatile变量
关键字volatile可以说是Java虚拟机提供的最轻量级的同步机制.
### 特性:
#### 可见性
> 保证此变量对所有线程的可见性(这里的“可见性”是指当一条线程修改了这个变量的值,新值对于其他线程来说是可以立即得知的).
##### 普通变量可见
线程A修改一个普通变量的值,然后向主内存进行回写,另外一条线程B在线程A回写完成了之后再从主内存进行读取操作,新变量值才会对线程B可见.如下图：

![普通变量可见](https://github.com/gdufeZLYL/blog/blob/master/images/20180417111533.png)

代码说明：
```Java
package com.qexz;

public class TestVolatile {

    public static void main(String[] args) {
        ThreadDemo td = new ThreadDemo();
        new Thread(td).start();

        while(true){
            if(td.isFlag()){
               System.out.println("------------------");
               break;
            }
        }

    }

}

class ThreadDemo implements Runnable {

    private boolean flag = false;

    public void run() {
        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
        }

        flag = true;

        System.out.println("flag=" + isFlag());
    }

    public boolean isFlag() {
        return flag;
    }

    public void setFlag(boolean flag) {
        this.flag = flag;
    }

}
```
运行结果：
```
flag=true
<没有退出>
```
![程序分析图](https://github.com/gdufeZLYL/blog/blob/master/images/20180417114020.png)

##### volatile变量保证可见性
代码说明：
```Java
package com.qexz;

public class TestVolatile {
    public static void main(String[] args) {
        ThreadDemo td = new ThreadDemo();
        new Thread(td).start();

        while(true){
            if(td.isFlag()){
                System.out.println("------------------");
                break;
            }
        }

    }

}

class ThreadDemo implements Runnable {

    private volatile boolean flag = false;

    public void run() {
        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
        }

        flag = true;

        System.out.println("flag=" + isFlag());
    }

    public boolean isFlag() {
        return flag;
    }

    public void setFlag(boolean flag) {
        this.flag = flag;
    }
}
```
运行结果：
```
------------------
flag=true
<运行完毕>
```
程序分析图：
![程序分析图](https://github.com/gdufeZLYL/blog/blob/master/images/20180417134904.png)
##### 使用synchronized关键字保证可见性
代码：
```Java
package com.qexz;

public class TestVolatile {

    public static void main(String[] args) {
        ThreadDemo td = new ThreadDemo();
        new Thread(td).start();

        while(true){
            synchronized (td) {
                if(td.isFlag()){
                    System.out.println("------------------");
                    break;
                }
            }
        }

    }

}

class ThreadDemo implements Runnable {

    private boolean flag = false;

    public void run() {
        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
        }

        flag = true;

        System.out.println("flag=" + isFlag());
    }

    public boolean isFlag() {
        return flag;
    }

    public void setFlag(boolean flag) {
        this.flag = flag;
    }

}
```
运行结果:
```
------------------
flag=true
<运行结束>
```
程序分析：
参考8种操作执行规则的第6、8条
##### volatile关键字不保证原子性
代码说明:
```java
package com.qexz;

/**
 * volatile变量自增运算测试
 *
 * @author zzqnxx@foxmail.com
 * @date 2018/4/17 13:38
 */
public class VolatileTest {

    public static volatile int race = 0;

    public static void increase() {
        race++;
    }

    private static final int THREADS_COUNT = 20;

    public static void main(String[] args) {
        Thread[] threads = new Thread[THREADS_COUNT];
        for (int i = 0; i < THREADS_COUNT; i++) {
            threads[i] = new Thread(new Runnable() {
                public void run() {
                    for (int i = 0; i < 10000; i++) {
                        increase();
                    }
                }
            });
            threads[i].start();
        }
        //等待所有累加线程都结束
        while (Thread.activeCount() > 1) {
            Thread.yield();
        }
        System.out.println(race);
    }
}
```
运行结果：(都是小于200000)
```
188777
```
程序分析：
increase()方法字节码如下：
```
0 getstatic #2 <com/qexz/VolatileTest.race>
3 iconst_1
4 iadd
5 putstatic #2 <com/qexz/VolatileTest.race>
8 return
```
当getstatic指令吧race的值取到操作栈顶时,volatile关键字保证了race的值在此时是正确的,但是在执行iconst_1、iadd这些指令的时候,其他线程可能已经把race的值加大了,而在操作栈顶的值就变成了过期的数据,所以putstatic指令执行后就可能把较小的race的值同步回主内存之中.

![程序分析图](https://github.com/gdufeZLYL/blog/blob/master/images/20180417135422.png)

##### volatile使用场景
由于volatile变量只能保证可见性,在不符合以下两条规则的运算场景中,我们仍然要加锁(使用synchronized或java.util.concurrent中的原子类)来保证原子性.
* 运算结果并不依赖变量的当前值,或者能够确保只有单一的线程修改变量的值.
* 变量不需要与其他的状态变量共同参与不变约束.

volatile使用场景：
```Java
volatile boolean shutdownRequested;

public void shutdown() {
   shutdownRequested = true;
}

public void doWork() {
   while (!shutdownRequested) {
      //do stuff
   }
}
```

#### 禁止指令重排序优化
##### 线程内表现为串行的语义
普通的变量仅仅会保证在该方法的执行过程中所有依赖赋值结果的地方都能获取到正确的结果,而不能保证变量赋值操作的顺序与程序代码中的执行顺序一致.因为在一个线程的方法执行过程中无法感知到这点.
##### 指令重排序
```Java
Map configOptions;
char[] configText;
//此变量必须定义为volatile
volatile boolean initialized = false;

//假设以下代码在线程A中执行
//模拟读取配置信息,当读取完成后将initialized设置为true以通知其他线程配置可用
configOptions = new HashMap();
configText = readConfigFile(fileName);
processConfigOptions(configText, configOptions);
initialized = true;

//假设以下代码在线程B中执行
//等待initialized为true,代表线程A已经把配置信息初始化完成
while (!initialized) {
   sleep();
}
//使用线程A中初始化好的配置信息
doSomethingWithConfig();
```
分析：
如果定义initialized变量是没有使用volatile修饰,就可能会由于指令重排序的优化,导致位于线程A中最后一句的代码"initialized = true"被提前执行(这里虽然使用Java作为伪代码,但所指的重排序优化是机器级的优化操作,提前执行是指这句话对应的汇编代码被提前执行),这样在线程B中使用配置信息的代码就可能出现错误,而volatile关键字可以避免这类情况发生.
##### volatile关键字性能
> volatile变量读操作的性能消耗与普通变量几乎没有什么差别,但是写操作则可能会慢一些,因为它需要在本地代码中插入许多内存屏障指令来保证处理器不发生乱序执行.
***


## 对于long和double型变量的特殊规则
> Java内存模型虽然允许虚拟机不把long和double变量的读写实现成原子操作,但允许虚拟机选择吧这些操作实现为具有原子性的操作,而且还“强烈建议”虚拟机这样实现.一般不需要把用到的long和double变量专门声明为volatile.
***


## 原子性、可见性与有序性
### 原子性
* lock和unlock
* synchronized
### 可见性
> 可见性是指当一个线程修改了共享变量的值,其他线程能给立即得知这个修改.Java内存模型是通过在变量修改后将新值同步回主内存,在变量读取前从主内存刷新变量值这种依赖主内存作为传递媒介的方式来实现可见性的,无论是普通变量还是volatile变量都是如此.区别是：volatile的特殊规则保证了新值能立即同步到主内存,以及每次使用前立即从主内存刷新(volatile保证了多线程操作时变量的可见性).
* volatile
* synchronized
* final
### 有序性
> 如果在本线程内观察,所有的操作都是有序的;如果在一个线程中观察另一个线程,所有的操作都是无序的.前半句是指“线程内表现为串行的语义”,后半句是指“指令重排序”现象和“工作内存与主内存同步延迟”现象.
* volatile
* synchronized
***


## 先行发生原则(happens-before)
* 程序次序规则：
> 在一个线程内,按照程序代码顺序(控制流顺序,分支、循环等结构),书写在前面的操作先行发生于书写在后面的操作.
* 管程锁定规则：
> 一个unlock操作先行发生于后面(时间上的先后顺序)对同一个锁的lock操作.
* volatile变量规则:
> 对一个volatile变量的写操作先行发生于后面(时间上的先后顺序)对这个变量的读操作.
* 线程启动规则：
> Thread对象的start()方法先行发生于此线程的每一个动作.
* 线程终止规则：
> 线程中的所有操作都先行发生于对此线程的终止检测(Thread.join(),Thread.isAlive()).
* 线程中断规则：
> 对线程interrupt()方法的调用先行发生于被中断线程的代码检测到中断事件的发生,可以通过Thread.interrupted()方法检测到是否有中断发生.
* 对象终结规则
> 一个对象的初始化完成(构造函数执行结束)先行发生于它的finalize()方法的开始.
* 传递性
> 如果操作A先行发生于操作B,操作B先行发生于操作C,那就可以得出操作A先行发生于操作C的结论.

***

### 参考
1. 《深入理解Java虚拟机》（第2版）—— 第12章 Java内存模型与线程

















