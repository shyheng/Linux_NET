#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/ether.h>//ETH_P_ALL
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    int sockfd;
    if ((sockfd = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) < 0)
    {
        perror("out");
        exit(1);
    }
    
    printf("sockfd = %d",sockfd);

    //关闭 close
    close(sockfd);
    return 0;
}
