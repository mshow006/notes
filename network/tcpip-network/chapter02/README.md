## 第2章：套接字类型与协议设置

介绍套接字创建方法及不同套接字的特性

### 2.1 套接字协议及其数据传输特性

#### 协议（Protocol）

协议就是通信规则，计算机之间的通信规则。

#### 创建套接字

```c
#include <sys/socket.h>

// domain: 套接字中使用的协议族（Protocol Family）信息
// type: 套接字数据传输类型信息
// protocol: 计算机间通信中使用的协议信息
// 成功返回文件描述符，失败返回-1
int socket(int domain, int type, int protocol);
```

#### 协议族（Protocol Family）

**`sys/socket.h` 中声明的协议族**

| 名称      | 协议族               |
| --------- | -------------------- |
| PF_INET   | **IPv4互联网协议**   |
| PF_INET6  | **IPv6互联网协议**   |
| PF_LOCAL  | 本地通信的UNIX协议族 |
| PF_PACKET | 底层套接字协议族     |
| PF_IPX    | IPX Novell协议族     |

#### 套接字类型（Type）

type: 数据的传输方式

一个协议族可能对应着多种传输方式（Type）

- 类型1：面向连接的套接字（SOCK_STREAM）

    **可靠的、按序传递的、基于字节的面向连接的数据传输方式的套接字。**

    - 套接字连接一一对应

    - 传输过程中数据不会消失

    - 按序传输数据

    - 传输的数据不存在数据边界（Boundary）

        `100个糖果是分批传递的,但接收者凑齐100个后才装袋`

        `传输数据的计算机通过3次调用 write函数传递了100字节的数据,但接收数据的计算机仅通过1次read函数调用就接收了全部100个字节`

    套接字缓冲（buffer）填满后数据传输情况：

    > 在调用read函数读取缓冲中的数据时，缓冲是不满的；但当read读取速度比接受数据速度慢，则缓冲可能被填满，此时套接字无法接受数据，传输端套接字停止传输，所以不会发送数据丢失。
    >
    > 面向连接的套接字会根据**接收端的状态**传输数据，如果传输出错会提供**重传服务**。
    >
    > 面向连接的套接字一般不会发送数据丢失。

- 类型2：面向消息的套接字（SOCK_DGRAM）

    **不可靠的、不按序传递的、以数据的高速传输为目的套接字**

    - 强调快速传输而非传输顺序

    - 传输的数据可能丢失也可能损坏

    - 限制每次传输的数据大小

    - 传输的数据有数据边界（接收数据的次数和传输次数相同）

        `使用摩托车发送2件包裹，接收者需要分2次接受`

    面向消息的套接字不存在连接的概念。

#### 协议的选择（Protocol）

大多数情况下，可以向 protool 传递0，除非：`同一种协议族中存在多个数据传输方式相同的协议`

- IPv4协议族中面向连接的套接字

    ```c
    // TCP 套接字
    // PF_INET: IPv4网络协议族
    // SOCK_STREAM: 面向连接的数据传输
    // IPPROTO_TCP: 满足上述两个条件的只有 IPPROTO_TCP
    int tcp_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    ```

- IPv4协议族中面向消息的套接字

    ```c
    // UDP 套接字
    // PF_INET: IPv4网络协议族
    // SOCK_DGRAM: 面向消息的数据传输
    // IPPROTO_TCP: 满足上述两个条件的只有 IPPROTO_TCP
    int tcp_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ```

#### 面向连接的套接字：TCP套接字

特性：传输的数据不存在数据边界

在第一章中的`hello_client.c`代码里，更改read函数的调用方式

以验证TCP套接字的`传输的数据不存在数据边界`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0; // c 中，默认不赋值会赋值为随机数
    int idx = 0, read_len = 0;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    // IPv4协议族中面向连接的套接字 TCP套接字
    // 若前两个参数是 PF_INET, SOCK_STREAM,则可以省略第三个参数
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 调用 connect 函数向服务器端发送连接请求。
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect error");

    // 每次读取1个字节，read返回0，跳出循环
    while (read_len = read(sock, &message[idx++], 1))
    {
        if (read_len == -1)
            error_handling("read() error!");

        // read_len 的值始终为1
        // 跳出循环后，str_len中是读取的总字节数
        str_len += read_len;
    }

    printf("\n");
    printf("Message from server: %s \n", message);
    printf("Function read call count: %d \n", str_len);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
```

执行结果

```
Message from server: Hello World!
Function read call count: 13
```

服务器端发送了13字节的数据，客户端调用了13次read函数进行读取。

### 2.3 习题

1. 什么是协议？在收发数据中定义协议有何意义？

    传输数据的规则

    更有效的完成数据传输

2. 面向连接的TCP套接字传输特性有3点，请分别说明

    - 传输过程中数据不会消失
    - 按序传输数据
    - 传输的数据不存在数据边界（Boundary）

3. 面向消息的套接字的特性

    - 强调快速传输而非传输顺序
    - 传输的数据可能丢失或者损毁
    - 传输的数据有数据边界
    - 限制每次传输的数据大小

4. 套接字的适用情况

    演唱会现场直播的多媒体数据 - 面向消息

    某人压缩过的文本文件 - 面向连接

    网上银行用户与银行之间的数据传递 - 面向连接

5. 何种类型的套接字不存在数据边界？这类套接字接收数据时需要注意什么？

    面向连接的套接字不存在数据边界（Boundary）

    在接受套接字内部，写入buffer的速度要小于读取buffer的速度

6. tcp_server.c 和 tcp_client.c 中多次调用read函数读取，服务器端调用1次write函数传递的字符串，更改程序，使服务器端多次调用（次数自拟）write函数传输数据，客户端要等待服务器端传输所有数据。通过下列代码延迟read或recv函数的调用

    ```c
    for (i = 0; i < 3000; i++)
        printf("Wait time %d \n", i);
    ```

    让CPU执行多余的任务以延迟代码运行的方式称为`Busy Waiting`，使用得当即可推迟函数调用。