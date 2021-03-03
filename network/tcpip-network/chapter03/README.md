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

### 网路字节序与地址变换

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

