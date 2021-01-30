#include <stdio.h>//print
#include <stdlib.h>//exit
#include <sys/types.h>
#include <sys/socket.h>//socket
#include <netinet/in.h>//sockaddr_in
#include <arpa/inet.h>//htons inet_addr
#include <unistd.h>//close
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    if(argc < 3){
        fprintf(stderr,"%s",argv[0]);
        exit(1);
    }

    int sockfd;
    struct sockaddr_in groupcastaddr;
    socklen_t addrlen = sizeof(groupcastaddr);

    //1创建套接字
    if ((sockfd = socket(AF_INET,SOCK_DGRAM,0))< 0)
    {
        perror("out");
        exit(1);
    }

    //2 设置添加多组
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(argv[1]);
    mreq.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))< 0)
    {
        perror("out");
        exit(1);
    }

    //2 填充广播信息结构体
    groupcastaddr.sin_family = AF_INET;
    groupcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
    groupcastaddr.sin_port = htons(atoi(argv[2]));

    //3 套接字与广播信息绑定
    if (bind(sockfd,(struct sockaddr *)&groupcastaddr,addrlen)<0)
    {
        perror("out");
        exit(1);
    }
    
    //4 通信
    char text[32] = "";
    struct sockaddr_in sendaddr;

    while (1)
    {
        if (recvfrom(sockfd,text,sizeof(text),0,(struct sockaddr *)&sendaddr,&addrlen)<0)
        {
            perror("out");
            exit(1);
        }
        printf("%s - %d : %s\n",inet_ntoa(sendaddr.sin_addr),ntohs(sendaddr.sin_port),text);
    }
    
    return 0;
}
