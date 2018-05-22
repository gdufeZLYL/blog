# Windows Binary Installation
## 安装
1. [下载apache-activemq-x.x.x-bin.zip](http://activemq.apache.org/download.html)

2. 解压到指定目录上,例如:

![指定目录](https://github.com/gdufeZLYL/blog/blob/master/images/20180522085752.png)

## 启动ActiveMQ

![启动ActiveMQ](https://github.com/gdufeZLYL/blog/blob/master/images/20180522090227.png)

![启动ActiveMQ](https://github.com/gdufeZLYL/blog/blob/master/images/20180522090307.png)

## 测试安装是否成功
### Using the administrative interface

![Using the administrative interface](https://github.com/gdufeZLYL/blog/blob/master/images/20180522090711.png)

![Using the administrative interface](https://github.com/gdufeZLYL/blog/blob/master/images/20180522090750.png)

![Using the administrative interface](https://github.com/gdufeZLYL/blog/blob/master/images/20180522090841.png)

![Using the administrative interface](https://github.com/gdufeZLYL/blog/blob/master/images/20180522090957.png)

![Using the administrative interface](https://github.com/gdufeZLYL/blog/blob/master/images/20180522091040.png)

![Using the administrative interface](https://github.com/gdufeZLYL/blog/blob/master/images/20180522091151.png)

![Using the administrative interface](https://github.com/gdufeZLYL/blog/blob/master/images/20180522091315.png)

![Using the administrative interface](https://github.com/gdufeZLYL/blog/blob/master/images/20180522091349.png)

### Logfile and console output

![Logfile and console output](https://github.com/gdufeZLYL/blog/blob/master/images/20180522091512.png)

![Logfile and console output](https://github.com/gdufeZLYL/blog/blob/master/images/20180522091544.png)

### Listen port

![Listen port](https://github.com/gdufeZLYL/blog/blob/master/images/20180522091613.png)

![Listen port](https://github.com/gdufeZLYL/blog/blob/master/images/20180522092913.png)

## Monitoring ActiveMQ

![Monitoring ActiveMQ](https://github.com/gdufeZLYL/blog/blob/master/images/20180522094928.png)

## Stopping ActiveMQ

![Stopping ActiveMQ](https://github.com/gdufeZLYL/blog/blob/master/images/20180522095305.png)

## Configuring ActiveMQ
> The ActiveMQ broker should now run. You can configure the broker by specifying an [Xml Configuration](http://activemq.apache.org/xml-configuration.html) file as a parameter to the activemq command. An alternative is to use the [Broker Configuration](http://activemq.apache.org/broker-configuration-uri.html) URI to configure things on the command line in a concise format (though the configuration options are not as extensive as if you use Java or XML code). You can also

> Also see [Configuring Transports](http://activemq.apache.org/configuring-transports.html) to see how you can configure the various connection, transport and broker options using the connection URL in the ActiveMQConnectionFactory.

> See the [Initial Configuration](http://activemq.apache.org/initial-configuration.html) for details of which jars you need to add to your classpath to start using ActiveMQ in your Java code

> If you want to use JNDI to connect to your JMS provider then please view the [JNDI Support](http://activemq.apache.org/jndi-support.html). If you are a Spring user you should read about [Spring Support](http://activemq.apache.org/spring-support.html)

> After the installation, ActiveMQ is running with a basic configuration. For details on configuring options, please see refer to the [Configuration](http://activemq.apache.org/configuration.html) section.