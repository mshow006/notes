#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(char *message);

int main(void)
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