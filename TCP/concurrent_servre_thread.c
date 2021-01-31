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
#include <pthread.h>

#define N 128
#define ERR_LOG(errmsg) do{\
                                                    perror(errmsg);\
                                                    exit(1);\
                                            }while(0)

typedef struct 
{
    struct sockaddr_in addr;
    int acceptfd;
}MSG;

void *pthread_fun(void *arg)
{
    char buf[N] = "";
    ssize_t bytes;
    MSG msg = *(MSG *)arg;
    while (1)
    {
        if ((bytes = recv(msg.acceptfd,buf,N,0)) < 0)
        {
            ERR_LOG("out");
        }
        else if (bytes == 0)
        {
            printf("quited\n");
            pthread_exit(NULL);
        }
        if (strncmp(buf,"quit",4) == 0)
        {
            printf("quited\n");
            pthread_exit(NULL);
        }
        
        printf("ip %s port %d  %s\n",inet_ntoa(msg.addr.sin_addr),ntohs(msg.addr.sin_port),buf);

        strcat(buf,"yes");
        if (send(msg.acceptfd,buf,N,0)<0)
        {
            ERR_LOG("out");
        }
    }
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

    while (1)
    {
        //5 阻塞等待客服端
        if ((acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addrlen)) < 0)
        {
            ERR_LOG("out");
        }
        // 打印客服端
        // printf("ip %s port %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

        // 创建子线程与客服端进行通信
        MSG msg;
        msg.addr= clientaddr;
        msg.acceptfd = acceptfd;
        pthread_t thread;
        if (pthread_create(&thread,NULL,pthread_fun,&msg) != 0)
        {
            ERR_LOG("out");
        }
        pthread_detach(thread);
    }
    return 0;
}
