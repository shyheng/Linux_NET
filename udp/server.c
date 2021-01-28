#include <stdio.h>//print
#include <stdlib.h>//exit
#include <sys/types.h>
#include <sys/socket.h>//socket
#include <netinet/in.h>//sockaddr_in
#include <arpa/inet.h>//htons inet_addr
#include <unistd.h>//close
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr,"%s ip port\n",argv[0]);
        exit(1);
    }

    int sockfd;//文件描述符
    struct sockaddr_in serveaddr;//服务器网络信息结构体
    socklen_t addrlen = sizeof(serveaddr);

    //创建套接字
    if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
        perror("socket");
        exit(1);
    }   

    //填充服务器
    serveaddr.sin_family = AF_INET;
    serveaddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveaddr.sin_port = htons(atoi(argv[2]));
    if (bind(sockfd,(struct sockaddr *)&serveaddr,addrlen) < 0 )
    {
        perror("out");
        exit(1);
    }

    //通信
    while (1)
    {
        char text[32] = "";
        struct sockaddr_in clientaddr;
        if (recvfrom(sockfd, text,sizeof(text),0,(struct sockaddr *)&clientaddr,&addrlen) == -1)
        {
            perror("out");
            exit(1);
        }
        printf("ip %s port %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
        
        strcat(text,"%s");

        if (sendto(sockfd,text, sizeof(text), 0, (struct sockaddr *)&clientaddr,addrlen) ==-1)
        {
            perror("out");
            exit(1);
        }

    }
    //使用完关闭
    close(sockfd);
    return 0;
}
