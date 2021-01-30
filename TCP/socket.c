#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char const *argv[])
{
    //通过socket函数创建tcp套接字
    int sockfd;
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0))< 0)
    {
        perror("out");
        exit(1);
    }
    
    return 0;
}
