# Hadoop包含模块
* Hadoop Common: The common utilities that support the other Hadoop modules.(基础模块,给其他模块提供支撑)
* Hadoop Distributed File System (HDFS™): A distributed file system that provides high-throughput access to application data.(全称:Hadoop分布式文件系统,提供应用数据的高吞吐访问量)
* Hadoop YARN: A framework for job scheduling and cluster resource management.(一个关于工作调度和集群资源管理的框架)
* Hadoop MapReduce: A YARN-based system for parallel processing of large data sets.(基于YARN的大型数据集并行处理系统)

# Hadoop概述
## 介绍
> 分布式存储+分布式计算平台

## 能做什么
> 搭建大型数据仓库,PB级数据的存储、处理、分析、统计等
例子：
* 搜索引擎
* 日志分析
* 商业智能
* 数据挖掘

# Hadoop核心组件
## 分布式文件系统HDFS
* 源自于Google的GFS论文,论文发表于2003年10月
* HDFS是GFS的克隆版
* HDFS特点：扩展性&容错性&海量数据存储
* 将文件切分成指定大小的数据块并以多副本的存储在多个机器上
* 数据切分、多副本、容错等操作对用户是透明的

![文件系统](https://github.com/gdufeZLYL/blog/blob/master/images/20180503164706.png)

## 资源调度系统YARN
* YARN: Yet Another Resource Negotiator
* 负责整个集群资源的管理和调度
* YARN特点：扩展性&容错性&多框架资源统一调度

![YARN](https://github.com/gdufeZLYL/blog/blob/master/images/20180503165502.png)

## 分布式计算框架MapReduce
* 源自于Google的MapReduce论文,论文发表于2004年12月
* MapReduce是Google MapReduce的克隆版
* 特点：扩展性&容错性&海量数据离线处理

![MapReduce](https://github.com/gdufeZLYL/blog/blob/master/images/20180503165839.png)

# Hadoop优势
## 高可靠性
* 数据存储：数据块多副本
* 数据计算：重新调度作业计算

## 高扩展性
* 存储/计算资源不够时,可以横向的线性扩展机器
* 一个集群中可以包含数以千计的节点

## 其他
* 存储在廉价机器上,降低成本
* 成熟的生态圈

# Hadoop生态
## 狭义Hadoop
> 是一个适合大数据分布式存储(HDFS)、分布式计算(MapReduce)和资源调度(YARN)的平台.
## 广义Hadoop
> 指的是Hadoop生态系统,Hadoop生态系统是一个很庞大的概念,Hadoop是其中最重要最基础的一部分;生态系统中的每一个子系统只解决某一个特定的问题域(甚至可能很窄),不搞统一型的一个全能系统,而是小而精的多个小系统;

![Hadoop生态系统](https://github.com/gdufeZLYL/blog/blob/master/images/20180503173139.png)

## Hadoop生态圈
![Hadoop生态圈](https://github.com/gdufeZLYL/blog/blob/master/images/20180503175740.png)