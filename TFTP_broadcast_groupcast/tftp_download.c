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

void do_download(int sockfd,struct sockaddr_in servreaddr)
{
    char filename[128] = "";
    printf("输入要下载文件：");
    scanf("%s",filename);

    //给服务器发送消息，告知服务器执行下载
    unsigned char text[1024] = "";
    int text_len;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int fd;
    int flags = 0;
    int num = 0;
    ssize_t bytes;

    text_len = sprintf(text,"%c%c%s%c%s%c",0,1,filename,0,"octet",0);
    if (sendto(sockfd,text,text_len,0,(struct sockaddr *)&servreaddr,addrlen) < 0)
    {
        perror("out");
        exit(1);
    }
    while (1)
    {
        if ((bytes = recvfrom(sockfd,text,sizeof(text),0,(struct sockaddr *)&servreaddr,&addrlen)) <0)
        {
            perror("out");
            exit(1);
        }
        if (text[1] == 5)
        {
            printf("error%s",text+4);
            return ;
        }
        else if(text[1] == 3)
        {
            if (flags == 0)
            {
                if ((fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC,0664))< 0)
                {
                    perror("out");
                    exit(1);
                }
                flags = 1;
            }
            
            if ((num+1 == ntohs(*(unsigned short *)(text+2))) && (bytes == 516))
            {
                num = ntohs(*(unsigned short *)(text+2));
                if (write(fd,text+4,bytes-4) < 0)
                {
                    perror("out");
                    exit(1);
                }
                
                text[1] = 4;
                if (sendto(sockfd,text,4,0,(struct sockaddr *)&servreaddr,addrlen)<0)
                {
                    perror("out");
                    exit(1);
                }
            }
            else if ((num+1 == ntohs(*(unsigned short *)(text+2))) && (bytes < 516))
            {
                if (write(fd,text+4,bytes - 4) < 0)
                {
                    perror("out");
                    exit(1);
                }
                
                text[1] = 4;
                if (sendto(sockfd,text,4,0,(struct sockaddr *)&servreaddr,addrlen) <0)
                {
                    perror("out");
                    exit(1);
                }
                
                printf("文件下载完毕\n");
                return ;
            }   
        }   
    }
}

int main(int argc, char const *argv[])
{
    if(argc < 2){
        fprintf(stderr,"%s",argv[0]);
        exit(1);
    }

    int sockfd;
    struct sockaddr_in serveraddr;

    //创建套接字
    if ((sockfd = socket(AF_INET,SOCK_DGRAM,0))< 0)
    {
        perror("out");
        exit(1);
    }
    
    //填充服务器网络
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);//服务器的ip
    serveraddr.sin_port = htons(69);

    do_download(sockfd,serveraddr);//下载操作

    return 0;
}
