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

    //2 填充组播信息
    groupcastaddr.sin_family = AF_INET;
    groupcastaddr.sin_addr.s_addr = inet_addr(argv[1]);//224.xxx.xxx.xxx--239.xxx.xxx.xxx
    groupcastaddr.sin_port = htons(atoi(argv[2]));

    //3通信 
    char buf[128] = "";
    while (1)
    {
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1] = '\0';

        if (sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&groupcastaddr,addrlen)<0)
        {
            perror("out");
            exit(1);
        }
    }
    return 0;
}
