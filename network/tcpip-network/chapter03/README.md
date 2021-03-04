## 第3章：地址族与数据序列

介绍给套接字分配 IP 地址和端口号

### 3.1 分配给套接字的 IP 地址和端口号

IP（Internet Protocol）：网络协议，为收发网络数据而分配给计算机的值。

端口号是为区分程序中创建的套接字而分配给套接字的序号。

#### 网络地址（Internet Address）

- IPv4 (Internet Protocol version 4)   4 字节地址族

- IPv6 (Internet Protocol version 6)   16 字节地址族

##### IPv4

IPv4 标准的 4 字节 IP 地址分为网络地址和主机（指计算机）地址，有 A、B、C、D、E 等类型。

![image-20210303125843332](https://i.loli.net/2021/03/03/C4y7nfBShK5t3vO.png)

向目标网络传输数据，就是向构成网络的路由器（Router）或交换机（Switch）传递数据，由接受数的路由器根据数据中的主机地址向目标主机传递数据。

>**路由器和交换机**
>
>为特殊目的而设计运行的"计算机"；交换机比路由器功能更简单一些。

#### 网络地址分类与主机地址边界

- A类地址的首字节范围：0~127
- B类地址的首字节范围：128~191
- C类地址的首字节范围：192~223



- A类地址的首位以 0 开始
- B类地址的前2位以 10 开始
- C类地址的前3位以 110 开始

#### 用于区分套接字的端口号

每个软件可能会有不同的套接字，区分套接字就用到端口号

计算机中一般都有 NIC（Network Interface Card，网络接口卡）数据传输设备。

通过 NIC 接收的数据内有端口号，操作系统通过端口号吧数据传输到相应端口的套接字。1

![image-20210303131351688](https://i.loli.net/2021/03/03/PWMguU2bc1KvBEH.png)

端口号由 16 位构成，范围：0\~65535，0\~1023（Well-known PORT）已经被分配给默认的应用程序。

TCP套接字和UDP套接字不会共用套接字，允许重复。

### 3.2 地址信息的表示

#### 表示 IPv4 地址的结构体

```
struct sockaddr_in
{
	sa_family_t		sin_family;		//地址族（Address Family)
	uint16_t		sin_port;		//16位 TCP/UDP端口号
	struct int_addr	sin_addr;		//32位IP地址
	char			sin_zero[8];	//不使用
}

struct in_addr
{
	in_addr_t 		s_addr; 		//32位 IPv4 地址
}
```

`uint16_t`、`in_addr_t`等类型是为了考虑扩展性，如果使用`int3_t2`类型的数据，就能保证在任何时候都占用4字节，即使将来用64位表示int类型也是如此。

![image-20210303154153459](https://i.loli.net/2021/03/03/A9gDml3Ze5pUHWd.png)

#### 结构体 `sockaddr_in`的成员分析

##### sin_family

`IPv4`使用4字节地址族，`IPv6`使用16字节地址族

![image-20210303154338106](https://i.loli.net/2021/03/03/S4MQIPDbNGjE8iw.png)

##### sin_port

保存16位端口号，以网络字节序保存

##### sin_addr

保存32位IP地址信息，以网络字节序保存，`in_addr`结构体中的`in_addr`声明为`uint32_t`，因此只需当作32位整数即可。

##### sin_zero

无特殊含义，只是为了使结构体sockaddr_in的大小与sockaddr的结构体保持一致而插入的。

必须填充为0，否则无法得到预期结果。



```c
struct sockaddr_in serv_addr;
...
// sockaddr_in 需要转换成 sockaddr
if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind() error");

// sockaddr 结构
struct sockaddr
{
    sa_family_t		sin_family; 	// 地址族（Address Family)
    char 			sa_data[14];	// 地址信息 // 包含IP地址和端口号，其余部分填充0
}
```

### 3.3网路字节序与地址变换

不同CPU，在内存空间中保存4字节整数型值1的方式不同。

- 正序保存 `00000000 00000000 00000000 00000001`

- 倒序保存 `00000001 00000000 00000000 00000000`

这就需要考虑到对接收数据的解析顺序

#### 字节序（Order）与网络字节序

CPU向内存保存数据由2种方式，解析数据同样时2种

- 大端序（Big Endian）：高位字节存放到低位地址
- 小端序（Little Endian）：高位字节存放到高位地址

示例：

假设在 0x20 的地址中保存4字节int类型数 0x12345678

0x12 是最高位字节，0x78是最低位字节

- 大端序

    ![image-20210303163847903](https://i.loli.net/2021/03/03/Dr3iOxX1jlkKMdf.png)

- 小端序

    ![image-20210303163924970](https://i.loli.net/2021/03/03/MSXU1aJFWknLbfZ.png)

网络字节序（Network Byte Order）：统一使用大端序进行网络数据传输。

#### 字节序转换（Endian Conversions）

在填充sockaddr_in结构体前需将数据转换成网络字节序

转换字节序函数

```c
unsigned short htons(unsigned short);
unsigned short ntohs(unsigned short);
unsigned long htonl(unsigned long);
unsigned long ntohl(unsigned long);
```

h:host n:network s:short l:long

ntohs:把short型数据从网络字节序转化为主机字节序

### 3.4 网络地址的初始化与分配

#### 将字符串信息转换为网络字节序的整数型

sockaddr_in 中保存地址信息的成员为32位整数型，所有需要将IP地址转换为32位整数型数据

##### inet_addr函数

将字符串形式的IP地址转换为32位整数型

```c
#include <arpa/inet.h>

// 成功返回32位大端序整数型值，失败返回 INADDR_NONE
int_addr_t inet_addr(const char *string);
```

```c
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    char *addr1 = "1.2.3.4";
    char *addr2 = "1.2.3.256"; // 1个字节表示的最大整数为255，该IP地址错误

    unsigned long conv_addr = inet_addr(addr1);
    if (conv_addr == INADDR_NONE)
        printf("Error occured! \n");
    else
        // 0x4030201
        printf("Network ordered integer addr: %#lx \n", conv_addr);

    conv_addr = inet_addr(addr2);
    if (conv_addr == INADDR_NONE)
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr: %#lx \\nn", conv_addr);
    return 0;
}
```

输出 0x4030201

从输出结果可以验证确实转换为网络字节序

##### inet_aton函数

与 inet_addr函数功能完全相同，只不过利用了in_addr结构体，使用频率最高

```c
#include <arpa/inet.h>

// string: IP地址字符串地址
// addr: 保存转换结构的in_addr结构体变量的地址值
// 成功返回 1，失败返回 0
int_addr_t inet_aton(const char *string, struct in_addr *addr);
```

 实际编程中若要调用 inet_addr函数,需将转换后的IP地址信息代入sockaddr_in结构体中声明的 in_addr结构体变量。而 Inet_aton函数则不需此过程。原因在于,若传递 in_addr结构体变量地址值,函数会自动把结果填入该结构体变量。通过示例了解 inet_aton函数调用过程。

```c
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void error_handling(char *message);

int main(int argc, char *argv)
{
    char *addr = "127.233.124.79";
    struct sockaddr_in addr_inet;

    // 转换后的IP地址信息需保存到sockaddr_in的in_addr型变量才有意义
    // inet_aton函数的第二个参数呀求得到in_addr型的变量地址值，省去了手动保存IP地址信息的过程
    if (!inet_aton(addr, &addr_inet.sin_addr))
        error_handling("Conversion error");
    else
        printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);
        // Network ordered integer addr: 0x4f7ce97f
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
```

##### inet_ntoa函数

与inet_aton 函数正好相反，是将网络字节序整数型IP地址转换成我们熟悉的字符串形式

```c
#include <arpa/inet.h>

// 成功返回转换的字符串地址值，失败返回 -1
char * inet_ntoa(struct in_addr addr);
```

#### 网络地址初始化（服务器端）

套接字创建过程中常见的网络地址信息初始化方法

```c
struct sockaddr_in addr;
char *serv_ip = "211.217.168.13";			// 声明IP地址字符串
cahr *serv_port = "9190";					// 声明端口号字符串
memset(&addr, 0, sizeof(addr));				// 就够提变量addr的所有成员初始化为0
addr.sin_family = AF_INET;					// 指定地址族
addr.sin_addr.s_addr = inet_addr(serv_ip);	// 基于字符串的IP地址初始化
addr.sin_port = htons(atoi(serv_port));		// 基于字符串的端口号初始化 字符串转换为整型
```

利用字符串格式的IP地址和端口号初始化了sockaddr_in结构体变量

#### 客户端地址信息初始化

声明sockaddr_in结构体，并初始化要与之连接的服务器端套接字的IP和端口号

#### INADDR_ANY

初始化地址信息

```c
struct sockaddr_in addr;
char *serv_port = "9190";
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = htonl(INADDR_ANY);
addr.sin_port = htons(atoi(serv_port));
```

利用常数INADDR_ANY分配服务器端的IP地址，自动获取运行服务器短地计算机IP地址

若同一计算机中已分配多个IP地址（多宿主（Multi-homed）计算机，路由器之类）则只要端口一致，就可以从不同IP地址接收数据

服务端优先考虑



创建服务器端套接字时需要IP地址的原因：

> 同一计算机可以分配多个IP地址，数量与NIC相等，服务器端套接字接受那个IP（NIC传来的）的数据。
>
> 若只有一个NIC，直接使用INADDR_ANY

#### 向套接字分配网络地址

已经有了 sockaddr_in 结构体的初始化方法，下面就把初始化的地址信息分配给套接字，操作bind函数

```c
#include <sys/socket.h>

// sockfd: 要分配地址信息（IP地址和端口号）的套接字文件描述符
// myaddr: 存有地址信息的结构体变量地址值
// addrlen： 第二个结构体变量的长度
// 成功返回0，失败返回-1
int bind(int sockfd, struct sockadr *myaddr, socklen_t addrlen);
```

调用成功会把第二个参数指定的地址信息分配给第一个参数中的相应套接字

服务器端常见套接字初始化过程

```c
int serv_sock;
struct sockaddr_in serv_addr;
char *serv_port = "9109";

// 创建服务器端套接字（监听套接字）
serv_sock = socket(PF_INET, SOCK_STREAM, 0);

// 地址信息初始化
memset(&serv_addr, 0, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
serv_addr.sin_port = htons(atoi(serv_port));

// 分配地址信息
bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
......
```

