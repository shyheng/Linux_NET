#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define N 128
#define ERR_LOG(errmsg) do{\
                                                    perror(errmsg);\
                                                    exit(1);\
                                            }while(0)

void handler(int sig)
{
    wait(NULL);
}

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr,"%s ip port\n",argv[0]);
        exit(1);
    }

    int sockfd,acceptfd;
    struct sockaddr_in serveraddr,clientaddr;
    socklen_t addrlen = sizeof(serveraddr);
//1 创建套接字
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        ERR_LOG("out");
    }
    
    // 将套接字设置为可以重复使用
    int on = 1;
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
    {
        ERR_LOG("out");
    }
    
    // 2 填充服务器网络信息
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    //3 将套接字和服务器网络进行绑定
    if (bind(sockfd,(struct sockaddr *)&serveraddr,addrlen) < 0)
    {
        ERR_LOG("out");
    }
    
    //4 设置监听状态
    if (listen(sockfd,5)< 0)
    {
        ERR_LOG("out");
    }
    
    //使用型号 异步的方式处理僵尸进程
    signal(SIGCHLD,handler);

    while (1)
    {
        //5 阻塞等待客服端
        if ((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addrlen)) < 0)
        {
            ERR_LOG("out");
        }
        printf("ip %s port %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

        //使用fork创建子进程,父进程进行连接，子进程负责与客服端通信
        pid_t pid;
        if ((pid = fork()) <0)
        {
            ERR_LOG("out");
        }
        else if(pid > 0)//父进程负责执行accept
        {

        }
        else//子进程负责跟指定客服端
        {
            char buf[N] = "";
            ssize_t bytes;
            while (1)
            {
                if ((bytes = recv(acceptfd,buf,N,0)) < 0)
                {
                    ERR_LOG("out");
                }
                else if (bytes == 0)
                {
                    printf("client\n");
                    exit(0);
                }
                printf("client %s\n",buf);
                if (send(acceptfd,buf,N,0)<0)
                {
                    perror("socket");
                    exit(1);
                }
            }
            
        }
    }
    return 0;
}
