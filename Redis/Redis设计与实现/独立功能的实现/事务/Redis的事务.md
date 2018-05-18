# Redis的事务
> Redis通过MULTI、EXEC、WATCH等命令来实现事务(transaction)功能.事务提供了一种将多个命令请求打包,然后一次性、按顺序地执行多个命令的机制,并且在事务执行期间,服务器不会中断事务而改去执行其他客户端的命令请求,它会将事务中的所有命令都执行完毕,然后才去处理其他客户端的命令请求.

# 事务执行过程
一个事务执行过程: 首先以一个MULTI命令为开始,接着将多个命令放入事务当中,最后由EXEC命令将这个事务提交(commit)给服务器执行:
```
redis> MULTI
OK

redis> SET "name" "Practical Common Lisp"
QUEUED

redis> GET "name"
QUEUED

redis> SET "author" "Peter Seibel"
QUEUED

redis> GET "author"
QUEUED

redis> EXEC
OK
"Practical Common Lisp"
OK
"Peter Seibel"
```
