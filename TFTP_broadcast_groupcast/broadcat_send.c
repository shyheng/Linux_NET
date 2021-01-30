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
    struct sockaddr_in broadcataddr;
    socklen_t addrlen = sizeof(broadcataddr);

    //1创建套接字
    if ((sockfd = socket(AF_INET,SOCK_DGRAM,0))< 0)
    {
        perror("out");
        exit(1);
    }

    //第二步 设置允许发送广播权限
    int on = 1;
    if (setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on))< 0)
    {
        perror("out");
        exit(1);
    }
    
    //第三步 填充广播信息结构体
    broadcataddr.sin_family = AF_INET;
    broadcataddr.sin_addr.s_addr = inet_addr(argv[1]);
    broadcataddr.sin_port = htons(atoi(argv[2]));

    //第四步 进行通信
    char buf[128] = "";
    while (1)
    {
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1] = '\0';

        if (sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&broadcataddr,addrlen)<0)
        {
            perror("out");
            exit(1);
        }
    }
    return 0;
}
