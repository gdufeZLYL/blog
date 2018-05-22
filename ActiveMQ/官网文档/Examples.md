# Prerequisites
启动ActiveMQ:

![启动ActiveMQ](https://github.com/gdufeZLYL/blog/blob/master/images/20180522112329.png)

# Running
From the command line you can run
```
${ACTIVEMQ_HOME}/bin/activemq producer
${ACTIVEMQ_HOME}/bin/activemq consumer
```
If you have activemq-all jar available you can achieve the same with
```
java -jar activemq-all-5.x.x.jar producer
java -jar activemq-all-5.x.x.jar consumer
```
If you run inside Karaf, you can run the commands as
```
activemq:producer
activemq:consumer
```

# Options
```
${ACTIVEMQ_HOME}/bin/activemq -help
```
查看命令详细说明.

# Examples(TODO::)
Here are a couple of examples of more advanced features.

To send a text message with custom text use
```
bin/activemq producer --message "My message" --messageCount 1
```
To send byte message of custom length use
```
bin/activemq producer --messageSize 100 --messageCount 1
```
To send a text message with content obtained from an url
```
bin/activemq producer --payloadUrl http://activemq.apache.org/schema/core/activemq-core.xsd --messageCount 1
```
To consume in transaction use
```
bin/activemq consumer --transacted true
```
To use client acknowledgment use
```
bin/activemq consumer --ackMode CLIENT_ACKNOWLEDGE
```
To use durable topic subscribers use
```
bin/activemq consumer --durable true --clientId example --destination topic://TEST
```
