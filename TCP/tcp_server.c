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

    //2 套接字与服务器绑定
    struct sockaddr_in serveraddr;
    socklen_t addrlen = sizeof(serveraddr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    if (bind(sockfd,(struct sockaddr *)&serveraddr,addrlen) < 0)
    {
        perror("socket");
        exit(1);
    }
    
    //3 将套接字设置为监听状态
    if (listen(sockfd,10)<0)
    {
        perror("socket");
        exit(1);
    }
    
    //4 阻塞等待客服端
    int acceptfd;
    struct sockaddr_in clientaddr;
    if ((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addrlen)) < 0)
    {
        perror("socket");
        exit(1);
    }
    
    //打印客服端信息
    printf("ip %s port %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

    //5 通信
    char buf[N] = "";
    if (recv(acceptfd,buf,N,0) < 0)
    {
        perror("socket");
        exit(1);
    }
    printf("client %s",buf);

    strcat(buf,"gui");

    if (send(acceptfd,buf,N,0)<0)
    {
        perror("socket");
        exit(1);
    }
    
    //关闭文件描述符
    close(acceptfd);
    close(sockfd);
    return 0;
}
