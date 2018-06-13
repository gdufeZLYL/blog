## 目录
1. MongoDB安全楔子
2. 开启权限认证
3. 在MongoDB中创建用户
4. MongoDB用户角色详解
5. 在MongoDB中创建用户角色

## MongoDB安全楔子
### MongoDB安全
1. MongoDB安全概览
2. 物理隔离与网络隔离
3. IP白名单隔离
4. 用户名密码鉴权

### MongoDB安全概览
1. 最安全的是物理隔离:不现实
2. 网络隔离其次
3. 防火墙再其次
4. 用户名密码在最后

## 开启权限认证
1. auth开启
2. keyfile开启

### auth开启
* 在mongod.conf加上`auth=true`

mongod.log中鉴权启动的标志:

![mongod.log中鉴权启动的标志](https://github.com/gdufeZLYL/blog/blob/master/images/20180613161553.png)

## 在MongoDB中创建用户
1. 创建语法:`createUser`(2.6之前为addUser)
2. `{user:"<name>",pwd:"<cleartext password",customDara:{<any information>},roles:[{role:"<role>",db:"{database}"}]}`
3. 角色类型:内建类型`(read,readWrite,dbAdmin,dbOwner,userAdmin)`

```
db.createUser({user:"imooc",pwd:"imooc",roles:[{role:"userAdmin",db:"admin"},{role:"read",db:"test"}]})
```

先创建用户,再auth开启,否则一直不能授权

![创建用户](https://github.com/gdufeZLYL/blog/blob/master/images/20180613170720.png)

关闭MongoDB服务器,修改mongod.conf,加上`auth=true`

重启MongoDB服务器

无密码登录,然后`show tables`报错

![无密码登录操作](https://github.com/gdufeZLYL/blog/blob/master/images/20180613184926.png)

使用密码登录,在数据库admin上的角色是userAdmin,在数据库test上的角色是read.

在数据库imooc上没授权,报错:

![数据库imooc上没授权](https://github.com/gdufeZLYL/blog/blob/master/images/20180613194458.png)

在数据库test上只能读:

![在数据库test上只能读](https://github.com/gdufeZLYL/blog/blob/master/images/20180613195019.png)

## MongoDB用户角色详解
1. 数据库角色(read,readWrite,dbAdmin,dbOwner,userAdmin)
2. 集群角色(clusterAdmin,clusterManaer...)
3. 备份角色(backup,restore...)
4. 其他特殊权限(DBAdminAnyDatabase...)

## 在MongoDB中创建用户角色

![在MongoDB中创建用户角色](https://github.com/gdufeZLYL/blog/blob/master/images/5af300e10001060112800720-156-88.jpg)