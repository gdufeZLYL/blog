## 目录
1. 数据写入和查询
2. 数据更新
3. 更新不存在的数据
4. 更新多条的数据
5. 数据删除
6. 创建索引

## 创建数据库
```
use imooc   # 创建数据库,imooc为数据库名称,数据库没有数据时`show dbs`不会显示数据库名称
show dbs    # 显示当前所有数据库
db  # 显示当前数据库
db.dropDatabase()   # 删除当前数据库
```

![创建数据库](https://github.com/gdufeZLYL/blog/blob/master/images/20180612084222.png)

## 数据写入和查询

```
db.imooc_collection.insert({x:1})   # 向imooc_collection集合中插入文档`{x:1}`
show collections    # 显示当前数据库所有集合
db.imooc_collection.find()  # 默认找出该集合中所有文档
db.imooc_collection.insert({x:2,_id:1}) # _id必须唯一,否则出错
db.imooc_collection.insert({x:3,_id:1}) # 报错,因为已经存在_id为1的文档数据了
db.imooc_collection.find()  # 默认找出该集合中所有文档
db.imooc_collection.find({x:1})  # 找出符合该文档的文档数据
for(i=3;i<100;i++)db.imooc_collection.insert({x:i}) # 遍历插入97条数据
db.imooc_collection.find().count()  # 返回当前集合的文档数据数量
db.imooc_collection.find().skip(3).limit(2).sort({x:1}) # 对于集合所有文档数据进行按x排序,过滤前3条，然后返回前2条
```

![数据写入和查询](https://github.com/gdufeZLYL/blog/blob/master/images/20180612105802.png)

![数据写入和查询](https://github.com/gdufeZLYL/blog/blob/master/images/20180612110218.png)

## 数据更新

```
db.imooc_collection.find({x:1}) # 找出符合{x:1}的文档数据
db.imooc_collection.update({x:1},{x:999})   # update(arg1,arg2),arg1表示查询条件,arg2表示更新后的数据(即覆盖满足查询条件的数据),该示例表示查询{x:1}的文档,把符合查询条件的文档数据改为{x:999},这种属于"完全更新"
db.imooc_collection.find({x:1}) # 找出符合{x:1}的文档数据
db.imooc_collection.find({x:999}) # 找出符合{x:999}的文档数据

# 把y更新为99,x和z的数据不变
db.imooc_collection.insert({x:100,y:100,z:100}) # 向imooc_collection集合中插入文档`{x:100,y:100,z:100}`
# db.imooc_collection.update({z:100},{y:99}) # 用文档`{y:99}`覆盖满足查询条件为`{z:100}`,完全更新
db.imooc_collection.update({z:100},{$set:{y:99}}) # 把满足查询条件为`{z:100}`的文档数据中的y改为99,部分更新
db.imooc_collection.find({z:100}) # 找出符合{z:100}的文档数据
```

![数据更新](https://github.com/gdufeZLYL/blog/blob/master/images/20180612111312.png)

## 更新不存在的数据

```
db.imooc_collection.find({y:100})   # 找出符合{y:100}的文档数据,但是该集合中并没有符合{y:100}的文档数据
db.imooc_collection.update({y:100},{y:999}) # 更新不存在符合{y:100}的文档数据时
db.imooc_collection.find({y:999})   # 找出符合{y:100}的文档数据,结果显然不存在

db.imooc_collection.update({y:100},{y:999},true) # 更新不存在符合{y:100}的文档数据时,因为upsert参数为true(这个参数的意思是,如果不存在update的记录,是否插入objNew,true为插入,默认是false,不插入.)
db.imooc_collection.find({y:999})   # 找出符合{y:100}的文档数据,结果存在,因为前一条命令插入了
```

![更新不存在的数据](https://github.com/gdufeZLYL/blog/blob/master/images/20180612111721.png)

## 更新多条的数据

```
db.imooc_collection.insert({c:1})   # 向imooc_collection集合中插入文档`{c:1}`
db.imooc_collection.insert({c:1})   # 向imooc_collection集合中插入文档`{c:1}`
db.imooc_collection.insert({c:1})   # 向imooc_collection集合中插入文档`{c:1}`
db.imooc_collection.find({c:1})     # 找出符合{c:1}的文档数据
db.imooc_collection.update({c:1},{c:2}) # 默认只更新第一条数据
db.imooc_collection.find({c:2})     # 找出符合{c:2}的文档数据,只有一条
db.imooc_collection.update({c:1},{$set:{c:2}},false,true) # 部分更新,upsert参数为false,表示不存在不创建,multi参数为true(mongodb 默认是false,只更新找到的第一条记录，如果这个参数为true,就把按条件查出来多条记录全部更新),显然更新了2条数据
db.imooc_collection.find({c:1})     # 找出符合{c:1}的文档数据,显然不存在
db.imooc_collection.find({c:2})     # 找出符合{c:2}的文档数据,显然存在3条
```

![更新多条的数据](https://github.com/gdufeZLYL/blog/blob/master/images/20180612112017.png)

## 数据删除

```
db.imooc_collection.remove()    # 报错,必须提供查询条件
db.imooc_collection.find({c:2})     # 找出符合{c:2}的文档数据,显然存在3条
db.imooc_collection.remove({c:2})    # 删除满足查询条件的所有文档数据
db.imooc_collection.find({c:2})     # 找出符合{c:2}的文档数据,显然不存在
db.imooc_collection.drop()  # 删除当前集合
db
show tables # 显示当前集合,跟show collections效果一样
```

![数据删除](https://github.com/gdufeZLYL/blog/blob/master/images/20180612112256.png)

## 创建索引

```
use imooc
db.imooc_collection.getIndexes()    # 查看当前所有索引
db.imooc_collection.ensureIndex({x:1})  # 创建索引,x表示索引,1表示正向排序,-1表示逆向排序,该命令创建索引需要消耗一些时间,如果当前集合文档数据过多,不能使用之前该命令创建索引,需要在使用数据库之前就创建索引
```

![创建索引](https://github.com/gdufeZLYL/blog/blob/master/images/20180612112612.png)

