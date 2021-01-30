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

    //2 填充广播信息结构体
    broadcataddr.sin_family = AF_INET;
    broadcataddr.sin_addr.s_addr = inet_addr(argv[1]);
    broadcataddr.sin_port = htons(atoi(argv[2]));

    //3 套接字与广播信息绑定
    if (bind(sockfd,(struct sockaddr *)&broadcataddr,addrlen)<0)
    {
        perror("out");
        exit(1);
    }
    
    //4 通信
    char text[32] = "";
    struct sockaddr_in sendaddr;

    while (1)
    {
        //centerOS无法接受进行阻塞，但是Ubuntu可以接受，我的Ubuntu无法编译
        // 所以在centerOS进行编译之后，然后传到ubuntu进行运行
        if (recvfrom(sockfd,text,sizeof(text),0,(struct sockaddr *)&sendaddr,&addrlen)<0)
        {
            perror("out");
            exit(1);
        }
        printf("%s - %d : %s\n",inet_ntoa(sendaddr.sin_addr),ntohs(sendaddr.sin_port),text);
    }
    
    return 0;
}
