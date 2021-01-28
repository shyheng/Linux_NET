#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    char ip_str[]="192.168.242.128";
    unsigned int ip_int = 0;
    unsigned char *ip_p = NULL;

    //将点分十进制ip地址转32位无符号整型数据
    inet_pton(AF_INET,ip_str,&ip_int);
    printf("in_uint = %d\n",ip_int);

    ip_p = (char *)&ip_int;
    printf("in_uint = %d,%d,%d,%d",*ip_p,*(ip_p+1),*(ip_p+2),*(ip_p+3));

    return 0;
}
