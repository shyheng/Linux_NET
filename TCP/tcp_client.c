#include <stdio.h>//print
#include <stdlib.h>//exit
#include <sys/types.h>
#include <sys/socket.h>//socket
#include <netinet/in.h>//sockaddr_in
#include <arpa/inet.h>//htons inet_addr
#include <unistd.h>//close
#include <string.h>

#define N 128

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr,"%s ip port\n",argv[0]);
        exit(1);
    }

    //1创建套接字
    int sockfd;
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    //2 发送客服端请求
    struct sockaddr_in serveraddr;
    socklen_t addrlen = sizeof(serveraddr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    if (connect(sockfd,(struct sockaddr *)&serveraddr,addrlen) < 0)
    {
        perror("socket");
        exit(1);
    }
    
    //3 进行通信
    //发送数据
    char buf[N] = "";
    fgets(buf,N,stdin);
    buf[strlen(buf)-1] = '\0';
    if (send(sockfd,buf,N,0) < 0)
    {
        perror("socket");
        exit(1);
    }
    
    //接受数据
    char text[N] = "";
    if (recv(sockfd,text,N,0) < 0)
    {
        perror("socket");
        exit(1);
    }
    printf("接收：%s",text);

    //4关闭套接字文件描述符
    close(sockfd);
    return 0;
}
