## 目录
1. mongoDB索引简介
2. _id索引
3. 单键索引
4. 多键索引
5. 复合索引
6. 过期索引简介
7. 过期索引案例演示

## mongoDB索引简介
### 索引使用
* 显著地加快查询速度
### 内容简介
* 索引的种类与使用
* 索引的匹配规则
* 如何建立合适的索引
* 索引建立的情况评估

### 索引的种类
1. _id索引
2. 单键索引
3. 多键索引
4. 复合索引
5. 过期索引
6. 全文索引
7. 地理位置索引

## _id索引
* _id索引是绝大多数集合默认建立的索引
* 对于每个插入的数据,MongoDB都会自动生成一条唯一的_id字段

```
show dbs
use imooc
show tables
db.imooc_2.insert({x:1})
db.imooc_2.getIndexes()
db.imooc_2.findOne()
```

![_id索引](https://github.com/gdufeZLYL/blog/blob/master/images/20180612132251.png)

## 单键索引
* 单键索引是最普通的索引
```
例如: 一条记录,形式为:{x:1,y:2,z:3},在x(或y,或z)建立索引
```

```
db.imooc_2.getIndexes()
db.imooc_2.ensureIndex({x:1})
db.imooc_2.find()
db.imooc_2.find({x:1})
```

![单键索引](https://github.com/gdufeZLYL/blog/blob/master/images/20180612133404.png)

## 多键索引
* 多键索引与单键索引创建形式相同,区别在于字段的值
    * 单键索引: 值为一个单一的值,例如字符串,数字或者日期
    * 多键索引: 值具有多个记录,例如数组.

```
db.imooc_2.getIndexes()
db.imooc_2.find()
db.imooc_2.insert({x:[1,2,3,4,5]})
```

![多键索引](https://github.com/gdufeZLYL/blog/blob/master/images/20180612135245.png)

## 复合索引



## 过期索引简介

## 过期索引案例演示