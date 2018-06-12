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

### 单键索引(官网)
参考[单键索引(官网)](https://docs.mongodb.com/manual/core/index-single/)

MongoDB对文档数据集合中的所有字段的索引建立都提供了支持,默认的情况下集合都有一个索引,即_id字段,但是用户可以增加其他键的索引来支持重要的查询和操作.

#### 在单个键建立一个正向排序的索引
考虑一个叫"records"的集合,集合的文档数据类似以下示例:
```
{
  "_id": ObjectId("570c04a4ad233577f97dc459"),
  "score": 1034,
  "location": { state: "NY", city: "New York" }
}
```
以下操作在集合records的`score`字段创建了一个正向排序的索引:
```
db.records.createIndex( { score: 1 } )
```
解析:
> 键的值1表示正向排序,-1表示逆向排序
基于`score`字段的单键索引将支持以下查询:
```
db.records.find( { score: 2 } )
db.records.find( { score: { $gt: 10 } } )
```

#### 创建一个嵌入式字段的单键索引
考虑一个叫"records"的集合,集合的文档数据类似以下示例:
```
{
  "_id": ObjectId("570c04a4ad233577f97dc459"),
  "score": 1034,
  "location": { state: "NY", city: "New York" }
}
```
以下操作在集合records的`location.state`字段创建了一个正向排序的索引:
```
db.records.createIndex( { "location.state": 1 } )
```
基于`location.state`字段的单键索引将支持以下查询:
```
db.records.find( { "location.state": "CA" } )
db.records.find( { "location.city": "Albany", "location.state": "NY" } )
```
#### 创建一个嵌入式文档的单键索引
考虑一个叫"records"的集合,集合的文档数据类似以下示例:
```
{
  "_id": ObjectId("570c04a4ad233577f97dc459"),
  "score": 1034,
  "location": { state: "NY", city: "New York" }
}
```
`localtion`字段就是一个包含嵌入式字段`city`和`state`的嵌入式文档,以下操作在集合records的`location`字段创建了一个正向排序的索引:
```
db.records.createIndex( { "location.state": 1 } )
```
基于`location.state`字段的单键索引将支持以下查询:
```
db.records.find( { location: { city: "New York", state: "NY" } } )
```

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

### 多键索引(官网)
参考[多键索引(官网)](https://docs.mongodb.com/manual/core/index-multikey/)

TODO::

## 复合索引

* 当我们的查询条件不只有一个时,就需要建立复合索引
```
1. 插入{x:1,y:2,z:3}记录
2. 按照x与y的值查询
3. 创建索引: db.collection.ensureIndex({x:1,y:1})
4. 使用{x:1,y:1}作为条件进行查询
```

```
db.imooc_2.find()
db.imooc_2.ensureIndex({x:1,y:1})
db.imooc_2.find({x:1,y:2})
```

![复合索引](https://github.com/gdufeZLYL/blog/blob/master/images/20180612183806.png)

### 复合索引(官网)
参考[复合索引(官网)](https://docs.mongodb.com/manual/core/index-compound/)

TODO::

## 过期索引简介
* 过期索引：是在一段时间后会过期的索引.
* 在索引过期后,相应的数据会被删除.
* 这适合存储一些在一段时间之后会失效的数据比如用户的登录信息、存储的日志.
* 建立方法: `db.collection.ensureIndex({time:1},{expireAfterSeconds:10})`

## 过期索引案例演示

```
use imooc
db.imooc_2.ensureIndex({time:1},{expireAfterSeconds:30})
db.imooc_2.insert({time:new Date()})    # 30秒后被删除
db.imooc_2.find()
db.imooc_2.insert({time:1}) # 不能被删除
db.imooc_2.insert({time:new Date()})    # 不一定30秒后被删除,存在误差
```

![过期索引案例演示](https://github.com/gdufeZLYL/blog/blob/master/images/20180612190058.png)

![过期索引案例演示](https://github.com/gdufeZLYL/blog/blob/master/images/20180612190130.png)

## 过期索引的限制
1. 存储在过期索引字段的值必须是指定的时间类型.
> 说明:必须是ISODate或者ISODate数组,不能使用时间戳,否则不能被自动删除.

2. 如果指定了ISODate数组,则按照最小的时间进行删除.
3. 过期索引不能是复合索引.
4. 删除时间不是精确.
> 说明：删除过程是由后台程序每60s跑一次,而且删除也需要一些时间,所以存在误差.