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
    - 持续交付（Continuous Delivery）

