## 第1章：理解网络编程和套接字

### 1.1 理解网络编程和套接字

网络编程就是编写程序使两台连网的计算机相互交换数据。

套接字(socket)：网络数据传输用的软件设备。

####  构建接电话套接字和接电话套接字

电话机通过固定电话网（telephone network）完成语音数据交换。基于这一点，固定电话与套接字并无太大区别。

电话机可以同时拨打和接听，但是套接字的拨打和接听是有区别的。

网络编程中接受连接请求的套接字创建过程：

1. 调用 socket 函数创建套接字
2. 调用 bind 函数分配 IP 地址和端口号
3. 嗲用 listen 函数转为可接收请求状态
4. 调用 accept 函数受理连接请求

请求连接的套接字创建过程：

1. 调用 socket 函数创建套接字
2. 调用 connect 函数向服务器端发送连接请求

#### Hello world 服务器端

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
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hellow World!";

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 调用 socket 函数创建套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
    {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 调用 bind 函数 分配 ip 地址和端口号
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("bind() error");
    }

    // 调用 listen 函数将套接字转为接受连接状态
    if (listen(serv_sock, 5) == -1)
    {
        error_handling("listen() error");
    }

    clnt_addr_size = sizeof(clnt_addr);
    // 调用 accept 函数受理连接请求
    // 如果再没有连接请求的情况下调用该函数，则不会返回，直到有连接请求为止
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
    {
        error_handling("accept() error");
    }

    write(clnt_sock, message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
```

#### Hello world 客户端

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
    int str_len;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    // 创建套接字，但此时套接字并不马上分为服务器端和客户端。
    // 如果紧接着调用 bind、listen 函数，将成为服务器端套接字；
    // 如果调用 connect 函数，将成为客户端套接字.
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 调用 connect 函数向服务器端发送连接请求。
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect error");
    }

    str_len = read(sock, message, sizeof(message) - 1);
    if (str_len == -1)
        error_handling("read() error!");

    printf("Message from server : %s \n", message);
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

#### 在 Linux 平台下运行

```bash
sudo apt-get update
sudo apt-get install build-essential gdb
```

安装之后确认是否安装成功

```bash
whereis g++
whereis gdb
```

编译 （`-o`指定文件名）:

```bash
gcc hello_server.c -o hserver
gcc hello_client.c -o hclient
```

运行

```bash
./hserver
./hclient
```

> 先执行服务器端，然后在执行客户端，运行成功的会在控制台输出 `Hello world!`

### 1.2 基于 Linux 的文件操作

Linux 中所有内容都是以文件的形式保存和管理的，即一切皆文件，普通文件是文件，目录（Windows 下称为文件夹）是文件，硬件设备（键盘、监视器、硬盘、打印机）是文件，包括套接字（socket）、网络通信等资源也都是文件。

#### 底层文件访问（Low-Level File Access）文件描述符（File Descriptor）

#### 打开文件操作

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// path: 文件名的字符串地址
// flag: 文件打开模式
// 成功返回文件描述符，失败返回-1
int open(const char *path, int flag);
```

#### 关闭文件操作

```c
#include <unistd.h>

// fd: 需要关闭的文件或套接字的文件描述符
// 成功返回0，失败返回-1
int close(int fd);
```

#### 写入文件操作

```c
#include <unistd.h>

// fd: 文件描述符
// buf: 保存要传输数据的缓冲地址
// nbytes: 需要传输数据的字节数
// 成功返回写入的字节数，失败返回-1
ssize_t write(int fd, const void * buf, size_t nbytes);
```

#### 写入文件示例

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(char *message);

int main()
{
    int fd;
    char buf[] = "Let's go~";

    // 如果 data.txt 不存在，直接创建，并只能写入
    // 否则，清空文件的全部数据
    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
    if (fd == -1)
        error_handling("open() error");

    // 输出：file descriptor: 3 
    printf("file descriptor: %d \n", fd);

    if (write(fd, buf, sizeof(buf)) == -1)
        error_handling("write() error");
    close(fd);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
```

#### 读取文件操作

```c
#include <unistd.h>

// 参数同写入操作
// 成功返回接受的字节数（遇到文件结尾时返回0），失败返回-1
ssize_t read(int fd, void * buf, size_t nbytes);
```

#### 读取文件示例

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(void)
{
    int fd;
    char buf[BUF_SIZE];

    fd = open("data.txt", O_RDONLY);
    if (fd == -1)
        error_handling("open() error");

    printf("file descriptor: %d \n", fd);

    if (read(fd, buf, sizeof(buf)) == -1)
        error_handling("read() error");

    printf("file data: %s", buf);
    close(fd);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
```

#### 文件描述符与套接字

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
    int fd1, fd2, fd3;
    fd1 = socket(PF_INET, SOCK_STREAM, 0);
    fd2 = open("test.dat", O_CREAT | O_WRONLY | O_TRUNC);
    fd3 = socket(PF_INET, SOCK_DGRAM, 0);

    printf("file descriptor 1: %d\n", fd1);
    printf("file descriptor 2: %d\n", fd2);
    printf("file descriptor 3: %d\n", fd3);
    // file descriptor 1: 3
    // file descriptor 2: 4
    // file descriptor 3: 5

    close(fd1);
    close(fd2);
    close(fd3);
    return 0;
}
```

描述符是从3开始，由小到大编号，0、1、2是分配给了标准 I/O 的描述符.

### 1.5 习题

1. 套接字在网络编程中的作用是什么？为何称它为套接字？

    套接字(socket)是网络数据传输用的软件设备，作用是进行网络数据传输。

2. 在服务器端创建套接字以后，会依次调用 listen 函数和 accept 函数。请比较并说明二者作用。

    调用 listen 函数后，套接字转变为可接受连接的状态

    调用 accept 函数是受理本次连接请求

3. Linux 中，对套接字数据进行 I/O 时可以直接使用文件 I/O 相关函数；而在 Windows 中则不可以。原因为何？

    Linux 中，一切皆为文件，因而可以通过文件I/O函数进行数据传输，返回文件描述符；

    Windows 中，对套接字和文件需要使用不同的操作函数，分别返回套接字句柄和文件句柄。

    有一种说法是 Windows 是以 UNIX、LInux 系列的 BSD 套接字为原型设计的，但是为了防止 UNIX、Linux 服务器端直接移植到 Windows ，故意为之。

4. 创建套接字后一般会给他分配地址，为什么？为了完成地址分配需要调用哪个函数？

    确定连接连接对象。bind 函数。

5. Linux 中的文件描述符与 Windows 的句柄实际上非常类似。请以套接字为对象说明它们的含义。

    区分不同的套接字。

6. 底层 I/O 函数与 ANSI 标准定义的文件 I/O 函数有何区别？

    文件 I/O 又称为低级磁盘 I/O，遵循 POSIX 相关标准。任何兼容 POSIX 标准的操作系统上都支持文件I/O。标准 I/O 被称为高级磁盘 I/O，遵循 ANSI C 相关标准。只要开发环境中有标准 I/O 库，标准 I/O 就可以使用。

    Linux 中使用的是 GLIBC，它是标准C库的超集。不仅包含 ANSI C 中定义的函数，还包括 POSIX 标准中定义的函数。因此，Linux 下既可以使用标准 I/O，也可以使用文件 I/O。

7. 参考本书给出的示例`low_open.c`和`low_read.c`，分别利用底层文件 I/O 和 ANSI 标准 I/O 编写文件复制程序。可任意指定复制程序的使用方法。 