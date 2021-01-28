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
    //./a.out 192.168.56.1 8080
    if (argc < 3)
    {
        fprintf(stderr,"usage %s ip port\n",argv[0]);
        exit(1);
    }
    
    //第一步创建套接字
    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    printf("sockfd =%d\n",sockfd);

    //第二步 填充服务器网络信息结构体 sockaddr_in
    struct sockaddr_in serveraddr;
    socklen_t addrlen = sizeof(serveraddr);
    serveraddr.sin_family = AF_INET;//协议族 AF_INET ipv4网络协议
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);//ip地址
    serveraddr.sin_port = htons(atoi(argv[2]));
    
    //第三步 发送数据
    char buf[N] = "";
    while (1)
    {
        fgets(buf,N,stdin);
        buf[strlen(buf) - 1] = '\0';//把buf字符串中的\n转\0
        if (sendto(sockfd, buf, N, 0, (struct sockaddr *)&serveraddr,addrlen) ==-1)
        {
            perror("out");
            exit(1);
        }
    }
    
    //第四步 关闭套接字文件描述符
    close(sockfd);
    
    return 0;
}
