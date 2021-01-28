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
#if 0
// 一般不用写系统调用
    struct sockaddr_in clientaddr;
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = inet_addr(argv[1]);
    clientaddr.sin_port = htons(atoi(argv[2]));
    if (bind(sockfd,(struct sockaddr *)&clientaddr,addrlen) < 0 )
    {
        perror("out");
        exit(1);
    }
#endif
    //填充服务器
    serveaddr.sin_family = AF_INET;
    serveaddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveaddr.sin_port = htons(atoi(argv[2]));

    //通信
    char buf[32] = "";
    while (1)
    {
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf) - 1] = '\0';

        if (sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serveaddr,addrlen) ==-1)
        {
            perror("out");
            exit(1);
        }

        char text[32] = "";
        if (recvfrom(sockfd, text,sizeof(text),0,(struct sockaddr *)&serveaddr,&addrlen) == -1)
        {
            perror("out");
            exit(1);
        }
        printf("server %s\n",text);
    }
    //使用完关闭
    close(sockfd);
    return 0;
}
