## 简介

### 概念

> Microservice architecture – a variant of the service-oriented architecture (SOA) structural style – arranges an application as a collection of loosely coupled services. In a microservices architecture, services are fine-grained and the protocols are lightweight. (Source Wikipedia)

### 目的
解决单体应用（Monolithic Application）规模增加时所带来的问题


Monolithic、粒度更小的SOA、分布式系统、架构（语言无关）

### 单体应用(Monolithic) VS 微服务

- 优势

    - 开发（Development）

    - 稳定（Stability）

        单一进程的方法调用优于远程调用（反序列化，网络稳定）

    - 性能（Performance）

    - 部署（Deployment）

- 不足

    - 中心化（Centralization）

    - 耦合（Coupling）

    - 学习成本（Learning Cost）

    - 伸缩（Scale）

        水桶效应

    - 持续交付（Continuous Delivery）

#### 面向服务架构（SOA）VS 微服务

- 类同
    - 面向服务（Service-Oriented）
    - 松耦合（Loose-Coupling）
    - 模块化（Modular）
    - 分布式计算（Distributed Computing）
    - 平台无关性（Independent Platform）
- 差异
    - ”原子性“（Atomic）
    - 领域驱动设计（DDD）
    - 开发运维体系（DevOps）

### 微服务发展的历程

- 进程间通讯（Inter-Process Communication）
    - 近端通讯（Local）
        - 文件（File）
        - 消息队列（Message）
        - 共享内存（Shared Memory）
        - 其他（Pipe、MMF）
    - 远端通讯（Remote）
        - 套接字（Socket）
        - 远程过程调用（RPC）

- 分布式计算（Distributed Computing）
    - 模型（Model）
        - 客户端-服务端（Client Server）
        - 点对点（Peer To Peer）
    - 分类（Type）
        - 同步（）
        - 异步（）
    - 通讯协议（）
        - HTTP（）
        - 自定义协议（）
    - 传输介质（）
        - 文本（）
        - 二进制（）

- SOA（）
    - 层次划分（）
        - 消费接口（）
        - 业务处理（）
        - 服务组件（）
    - 技术
        - SOAP
        - CORBA
        - RMI
        - REST
        - ESB

### 微服务面临的挑战

- 技术挑战

    - 注册与发现（）

    - 路由（）

        - 负载均衡

        - 版本

    - 可靠性（）

    - 延迟（）

    - 热点（）

    - 短路（）

    - 伸缩（）

    - 异步（）

    - 监控（）

    - 配置（）

    - 数据同步（）

    - 安全（）

### Java微服务实践

- 元编程（Meta Programming）

    再Java编程语言中，元编程是一种新型的编程模式，目的是减少代码行数，得到事半功倍的效果

    - 主要模式
        - 注解驱动（）
        - 反射驱动（）
        - 表达式驱动（）
        - Lambda（）
        - Script On JVM

- 接口编程（）

    又称为契约编程，在OOP语言中，其契约范围包括方法名称、方法入参（类型和顺序）、方法返回值（类型）以及异常情况等元数据。

