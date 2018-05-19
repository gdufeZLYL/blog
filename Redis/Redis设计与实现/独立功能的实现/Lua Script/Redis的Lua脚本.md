Redis从2.6版本开始引入对Lua脚本的支持,通过在服务器中嵌入Lua环境,Redis客户端可以使用Lua脚本,直接在服务器端原子地执行多个Redis命令.

其中,使用EVAL命令可以直接对输入的脚本进行求值:
```
redis> EVAL "return 'hello world'" 0
"hello world"
```
而使用EVALSHA命令则可以根据脚本的SHA1校验和来对脚本进行求值,但这个命令要求校验和对应的脚本必须至少被EVAL命令执行过一次:
```
redis> EVAL "return 1+1" 0
(integer) 2

redis> EVALSHA "a27e7e8a43702b7046d4f6a7ccf5b60cef6b9bd9" 0 // 上一个脚本的校验和
(integer) 2
```
或者这个校验和对应的脚本曾经被SCRIPT LOAD命令载入过:
```
redis> SCRIPT LOAD "return 2*2"
"a27e7e8a43702b7046d4f6a7ccf5b60cef6b9bd9"

redis> EVALSHA "a27e7e8a43702b7046d4f6a7ccf5b60cef6b9bd9"
(integer) 4
```