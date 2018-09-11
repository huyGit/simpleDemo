#include <stdio.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define CLI_PORT 9354
#define CLI_ADDR  "127.0.0.1"
#define SERV_PORT 9355
#define SERV_ADDR "127.0.0.1"
#define MAX_LINE 512


int main() {
    //创建socket
    int clifd = socket(AF_INET, SOCK_STREAM, 0);
    if (clifd < 0)
    {
        printf("socket create err:%d\n", clifd);
        exit(1);
    }

    //绑定套接字
    struct sockaddr_in cli_addr;
    bzero(&cli_addr, sizeof(struct sockaddr_in));
    cli_addr.sin_family = AF_INET;
    inet_pton(AF_INET, CLI_ADDR, &cli_addr.sin_addr);
    cli_addr.sin_port = htons(CLI_PORT);
    if (bind(clifd, (const struct sockaddr*)&cli_addr, sizeof(struct sockaddr_in)) < 0)
    {
        printf("bind error\n");
        exit(1);
    }

    //发起连接
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERV_ADDR, &serv_addr.sin_addr);
    serv_addr.sin_port = htons(SERV_PORT);
    if (connect(clifd, (const struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in)) < 0)
    {
        printf("connet error\n");
        exit(1);
    }


    //传输数据
    char sendline[MAX_LINE], recvline[MAX_LINE];
    while(fgets(sendline, MAX_LINE, stdin) != NULL)
    {
        write(clifd, sendline, strlen(sendline));
        if (read(clifd, recvline, MAX_LINE) == 0)
        {
            printf("server terminated\n");
            exit(1);
        }
        if (fputs(recvline, stdout) == EOF)
        {
            printf("fputs error\n");
            exit(1);
        }
    }
    close(clifd);
    printf("client exit\n");
    return 0;
}
