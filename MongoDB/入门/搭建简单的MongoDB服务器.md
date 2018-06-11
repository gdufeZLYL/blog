# 搭建简单点的MongoDB服务器

## 目录
1. 首先,创建一个叫做mongodb_simple的目录,进入到目录中.
2. 创建文件夹: data,用来存储数据库的数据文件.
3. 创建文件夹: log,用来存储数据库的日志文件.
4. 创建文件夹: bin,用来存储数据库的可执行文件.
5. 创建文件夹: conf,用来存储数据库的配置文件.

## 首先,创建一个叫做mongodb_simple的目录,进入到目录中

![mongodb_simple目录里](https://github.com/gdufeZLYL/blog/blob/master/images/20180611192113.png)

## 创建文件夹: data,用来存储数据库的数据文件

![创建四个文件夹](https://github.com/gdufeZLYL/blog/blob/master/images/20180611192741.png)

## 创建文件夹: log,用来存储数据库的日志文件

![创建四个文件夹](https://github.com/gdufeZLYL/blog/blob/master/images/20180611192741.png)

## 创建文件夹: bin,用来存储数据库的可执行文件

![创建四个文件夹](https://github.com/gdufeZLYL/blog/blob/master/images/20180611192741.png)

## 创建文件夹: conf,用来存储数据库的配置文件

![创建四个文件夹](https://github.com/gdufeZLYL/blog/blob/master/images/20180611192741.png)

![创建mongod.conf文件](https://github.com/gdufeZLYL/blog/blob/master/images/20180611193528.png)

mongod.conf文件内容:
```
port = 12345
dbpath = D:\MongoDB_DataBase\mongodb_simple\data
logpath = D:\MongoDB_DataBase\mongodb_simple\log\mongod.log
fork = true     # windows系统下无效
```

## 运行MongoDB服务器

![运行MongoDB服务器](https://github.com/gdufeZLYL/blog/blob/master/images/20180611212322.png)

![data目录下数据文件](https://github.com/gdufeZLYL/blog/blob/master/images/20180611212353.png)

![mongod.log文件](https://github.com/gdufeZLYL/blog/blob/master/images/20180611212435.png)

![mongod.log文件内容](https://github.com/gdufeZLYL/blog/blob/master/images/20180611212515.png)

## 连接MongoDB服务器

![连接MongoDB服务器](https://github.com/gdufeZLYL/blog/blob/master/images/20180611220851.png)

## 关闭MongoDB服务器

![关闭MongoDB服务器](https://github.com/gdufeZLYL/blog/blob/master/images/20180611221317.png)