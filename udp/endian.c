#include <stdio.h>

//判断当前的字节序

union un
{
    int a;
    char b;
};

int main(int argc, char const *argv[])
{
    union un myun;
    myun.a = 0x12345678;

    printf("a = %#x\n",myun.a);
    printf("b = %#x\n",myun.b);

    if(myun.b == 0x78)
    {
        printf("小端地址\n");
    }
    else
    {
        printf("大端地址\n");
    }
    return 0;
}
