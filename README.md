# BluePill
## 简介
一个基于MySQL协议的高性能中间层。

## 主要功能
* **连接池**

    支持中间层对连接池的管理，提高短连接使用场景的建连效率，以及对连接数量的合理限制。

* **读写分离**

    支持对应用透明的读写分离，识别SQL类型进行主从分发，并支持强制读主的相关功能。

* **负载均衡**

    多个从库的负载均衡。

* **从库故障屏蔽**

    对发生故障的从库，自动平滑下线，实现对应用无感知。

* **在线扩缩容**

    支持从库的平滑上、下线，实现对应用无感知。

* **多机房架构**

    支持多机房部署架构，同机房优先访问。

* **安全认证**

    账号密码、IP白名单

* **配置热加载**
* **监控**
* **日志**


## 应用场景
![image](https://user-images.githubusercontent.com/21175995/112304342-57469480-8cd8-11eb-8afd-2cb2e7a5a5c9.png)

## 逻辑架构
![image](https://user-images.githubusercontent.com/21175995/112304363-5b72b200-8cd8-11eb-9e0c-a3136de5f998.png)


