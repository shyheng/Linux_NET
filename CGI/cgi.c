#include <stdio.h>

int main(int argc, char const *argv[])
{
    //cgi必须有
    printf("content-type:text/html\n\n");
    printf("<html>\n<title>CGI</title>");
    printf("<conter>shy你好</conter>");    
    return 0;
}
