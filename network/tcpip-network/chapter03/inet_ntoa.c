#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argcp[])
{
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);

    // 向 inet_ntoa 函数传递结构体变量 addr1 中的地址信息并调用该函数，返回字符串形式的IP地址
    str_ptr = inet_ntoa(addr1.sin_addr);
    // 复制 str_ptr 信息到 str_arr
    strcpy(str_arr, str_ptr);
    printf("Dotted-Decimal notation: %s \n", str_ptr);

    // 再次调用，str_ptr 被覆盖
    inet_ntoa(addr2.sin_addr);
    printf("Dotted-Decimal notaion2: %s \n", str_ptr);
    printf("Dotted-Decimal notaion3: %s \n", str_arr);
    // Dotted-Decimal notation: 1.2.3.4 
    // Dotted-Decimal notaion2: 1.1.1.1 
    // Dotted-Decimal notaion3: 1.2.3.4
    return 0;
}