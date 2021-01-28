#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    unsigned char ip_int[] = {192,168,242,128};
    char ip_str[16] = "";

    inet_ntop(AF_INET,&ip_int,ip_str,16);

    printf("ip_s = %s\n",ip_str);
    return 0;
}
