#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;                      // 多余变量
    struct sockaddr_in serv_adr, from_adr; // 不在需要from_adr

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    // 创建套接字，但此时套接字并不马上分为服务器端和客户端。
    // 如果紧接着调用 bind、listen 函数，将成为服务器端套接字；
    // 如果调用 connect 函数，将成为客户端套接字.
    // 创建后为 TCO 套接字
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    // 结构体变量serv_addr 初始化Ip和端口信息
    // 初始化值为目标服务器端套接字IP和端口信息
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));

    while (1)
    {
        fputs("insert message(q to quie): ", stdout);
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        // 向服务器端传输数据
        // 自动分配IP和端口号
        /*
        sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
        */
        write(sock, message, strlen(message));
        /*
        adr_sz = sizeof(from_adr);
        */
        // 接收数据
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&from_adr, &adr_sz);
        message[str_len] = 0;
        printf("Message from server: %s", message);
    }

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}