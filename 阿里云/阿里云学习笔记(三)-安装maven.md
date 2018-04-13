## 下载maven 3.3.9
1. [apache-maven-3.3.9-bin.tar.gz](http://mirrors.sonic.net/apache/maven/maven-3/3.3.9/binaries/)
![image](https://github.com/gdufeZLYL/blog/blob/master/images/20180413105152.png)

2. 把maven安装包拖动到服务器`/usr/java/`目录下
![image](https://github.com/gdufeZLYL/blog/blob/master/images/20180413105642.png)

3. 把`apache-maven-3.3.9-bin.tar.gz`解压到`/usr/java/maven/`目录下：

```
tar -xvf apache-maven-3.3.9-bin.tar.gz -C /usr/java/maven/
```
![image](https://github.com/gdufeZLYL/blog/blob/master/images/20180413110127.png)

## 配置maven 3.3.9
1. 在`PATH`设置路径：

```
$ vim /etc/profile

MAVEN_HOME=/usr/java/maven/apache-maven-3.3.9
export MAVEN_HOME
export PATH=${PATH}:${MAVEN_HOME}/bin
```
路径生效命令:

```
source /etc/profile
```
![image](https://github.com/gdufeZLYL/blog/blob/master/images/20180413111239.png)
