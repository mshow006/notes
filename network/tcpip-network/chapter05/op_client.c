#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 待算数字的字节数
#define BUF_SIZE 1024
// 运算结果的字节数
#define RLT_SIZE 4
#define OPSZ 4
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    // 为收发数据准备的内存空间，需要数据积累到一定程度再收发
    char opmsg[BUF_SIZE];
    int result, opnd_cnt, i;
    struct sockaddr_in serv_addr;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    // 创建套接字，但此时套接字并不马上分为服务器端和客户端。
    // 如果紧接着调用 bind、listen 函数，将成为服务器端套接字；
    // 如果调用 connect 函数，将成为客户端套接字.
    // 创建后为 TCO 套接字
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    // 结构体变量serv_addr 初始化Ip和端口信息
    // 初始化值为目标服务器端套接字IP和端口信息
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 调用 connect 函数向服务器端发送连接请求。
    // 客户端的IP地址和端口号在调用connect函数时自动分配，无需调用bind函数
    // 若调用该函数引起的连接请求被注册到服务器端等待队列，则connect函数完成正常调用
    // 如果服务器端尚未调用accept函数，即使输出了字符串，也不会真正建立连接关系
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect error");
    else
        puts("Connected.........");

    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;

    for (int i = 0; i < opnd_cnt; i++)
    {
        printf("Operand %d: ", i + 1);
        // 从输入中得到待算整数，保存到opmsg
        // 4字节int型数据要保存到char数组，需要转换成int指针类型
        scanf("%d", (int *)&opmsg[i * OPSZ + 1]);
    }
    // 删除缓冲中的字符\n
    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);
    // 调用1次write函数进行传输，也可多次调用
    write(sock, opmsg, opnd_cnt * OPSZ + 2);
    // 保存服务器端传输的运算结构，待接收的数据长度为4字节，调用1次read函数即可接收
    read(sock, &result, RLT_SIZE);

    printf("Operation result: %d\n", result);

    // 关闭套接字，结束与服务器的连接
    // 调用close向相应套接字发送EOF（EOF即意味着中断连接）
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}