### 目录

---


#### chapter-1-exercise-1
控制台输出、命名空间、运算符
#### chapter-1-exercise-2
控制台输出

---

#### chapter-2-2
控制台输出

---

#### chapter-3-4
变量数据溢出输出
#### chapter-3-5
数据类型占用字节、类型转换
#### chapter-3-8
常量函数,关键字constexpr
#### chapter-3-9
枚举,关键字enum

---

#### chapter-4-4
动态数组,vector
#### chapter-4-5
字符数组(或字符串),char[],sizeof()
#### chapter-4-6
字符数组(或字符串),char[],strlen()
#### chapter-4-7
string,字符串连接

---

#### chapter-5-7
bitset,二进制运算

---

#### chapter-6-2
string,strcpy,c_str,if-else结构
#### chapter-6-5
switc结构,enum
#### chapter-6-12
for循环,for(:)

---

#### chapter-7-1
函数function
#### chapter-7-4
函数默认参数值
#### chapter-7-7
函数重载:返回类型和函数名一致，但是参数类型、个人不一样
#### chapter-7-9
引用:&
#### chapter-7-10
inline关键字,替代C中的#define
#### chapter-7-11
常量const
#### chapter-7-12
lambda表达式

---

#### chapter-8-1
取地址,&变量
#### chapter-8-2
指针变量初始化
#### chapter-8-3
指针变量输出:地址
#### chapter-8-4
指针变量相互赋值
#### chapter-8-6
指针变量占用字节：4字节
#### chapter-8-7
内存动态分配:new与delete
#### chapter-8-8
内存动态分配:new Type[]与delete[]
#### chapter-8-9
指针变量加减,for循环取值
#### chapter-8-10
- const Type* variable:
可以改变指针指向,不可改变指针指向的数据
- Type* const variable:
不可以改变指针指向,可改变指针指向的数据
- const Type* const variable:
不可以改变指针指向,不可改变指针指向的数据

#### chapter-8-11
指针与数组:地址
#### chapter-8-12
指针与数组:取值
#### chapter-8-13
指针未初始化的后果
#### chapter-8-14
指针未初始化的后果
#### chapter-8-15
内存动态分配失败,异常处理
#### chapter-8-16
内存动态分配失败,返回NULL,关键字nothrow
#### chapter-8-17
引用与地址
#### chapter-8-18
引用
#### chapter-8-19
引用
---

#### chapter-9-1
面向对象,类,public,成员数据、方法,实例初始化
#### chapter-9-2
private关键字,setXX(),getXX()
#### chapter-9-3
无参构造函数
#### chapter-9-4
构造函数重载
#### chapter-9-5
构造函数重载
#### chapter-9-6
构造函数传值列表话
#### chapter-9-7
析构函数:对象被delete时调用
#### chapter-9-8
浅拷贝：成员指针指向同一个地址,delete成员指针带来严重问题
#### chapter-9-9
深拷贝,解决浅拷贝带来的问题
#### chapter-9-10
单例类,单例模式
#### chapter-9-11
析构函数私有化：禁止在栈创建对象
- MonsterDB* myDB = new MonsterDB(); // on heap
- MonsterDB myDB; // on stack

#### chapter-9-12
类似Java的自动拆箱装箱问题

#### chapter-9-13
类的sizeof()

#### chapter-9-14
关键字friend调用private成员

#### chapter-9-15
关键字friend,friend类

#### chapter-9-16
关键字union,enum运用

#### chapter-9-17
类初始化方式:{}

#### chapter-9-18
关键字constexpr修饰类成员

---
#### chapter-10-1
面向对象:继承

#### chapter-10-2
关键字protect

#### chapter-10-3
继承,父类有参构造函数

#### chapter-10-4
子类函数重写父类函数

#### chapter-10-5
派生类调用父类函数

#### chapter-10-6
父类函数重载

#### chapter-10-7
构造函数、析构函数执行顺序

#### chapter-10-8
private继承

#### chapter-10-9
protected继承

#### chapter-10-10
多继承

---
#### chapter-11-1
派生类作为参数传给父类,调用重写方法(其实是调用父类方法)

#### chapter-11-2
关键字virtual,虚函数,派生类作为参数传给父类,调用重写方法(其实是调用子类方法)

#### chapter-11-3
- 指针指向的派生类作为函数的父类参数传值时,delete父类对象,只调用父类的析构函数;(堆)
- 普通对象实例会分别调用子类析构函数和父类析构函数(栈)

#### chapter-11-4
指针指向的派生类作为函数的父类参数传值时,delete父类对象,只调用父类的析构函数;(堆)(用关键字virtual修饰父类析构函数解决)

#### chapter-11-5
sizeof()与virtual








