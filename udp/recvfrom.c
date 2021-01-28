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
    
    //第一步 创建套接字
    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
        perror("out");
        exit(1);
    }

    //第二步 将服务器的网络信息结构体绑定前进行填充
    struct sockaddr_in sevreaddr;
    sevreaddr.sin_family = AF_INET;
    sevreaddr.sin_addr.s_addr = inet_addr(argv[1]);
    sevreaddr.sin_port = htons(atoi(argv[2]));

    //第三步 将网络信息结构体与套接字绑定
    if (bind(sockfd,(struct sockaddr *)&sevreaddr,sizeof(sevreaddr)) == -1)
    {
        perror("out");
        exit(1);
    }
    
    //接收数据
    char buf[N] = ""; 
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if (recvfrom(sockfd, buf,N,0,(struct sockaddr *)&clientaddr,&addrlen) == -1)
    {
        perror("out");
        exit(1);
    }
    
    //打印数据
    // 打印客服端的ip和端口
    printf("ip %s port %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
    printf("%s",buf);
    return 0;
}
