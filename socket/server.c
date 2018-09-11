#include <stdio.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>


int main() {
	//创建套接字
    struct sockaddr_in serv_addr, cli_addr;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
	{
        printf("socket create error:%d", fd);
		exit(1);
	}
	//绑定套接字
    bzero(&serv_addr, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(9355);
    if (bind(fd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in)) < 0)
	{
        printf("bind error\n");
		exit(1);
	}
	//主动套接字转为被动套接字
	if (listen(fd, 10) < 0)
	{
		printf("listen error\n");
		exit(1);
	}
    while(1) {
		socklen_t clilen = sizeof(struct sockaddr);
        int connfd = accept(fd, (struct sockaddr*)&cli_addr, &clilen);
        if (connfd < 0) {
            printf("accept error:%d\n", connfd);
            continue;
        }
        pid_t pid = fork();
        if (pid == 0) {
            close(fd);
            printf("cli addr:%d, port:%d\n", cli_addr.sin_addr.s_addr, cli_addr.sin_port);
            char recv[512];
            int readNum = read(connfd, recv, 512);
            if (readNum < 0)
                printf("read error:%d\n", readNum);
            int writeNum = write(connfd, recv, readNum);
            if (writeNum < 0)
                printf("write error:%d\n", writeNum);
            close(connfd);
        } else {
            close(connfd);
        }
    }
    return 0;
}
