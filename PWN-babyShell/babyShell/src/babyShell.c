#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void handler()
{
    printf("Bye ~\n");
    exit(0);
}

int filter(char* code){
    char* banList[7] = {"s","h","f","g","*","|","`"};
    int r = 0;
    for(int i=0;i<7;i++){
        r += strstr(code,banList[i])!=0;
    }
    return r;
}

char sc[256] = {0};
int main(){
    setvbuf(stdin, 0LL, 2, 0LL);
    setvbuf(stdout, 0LL, 2, 0LL);
    setvbuf(stderr, 0LL, 2, 0LL);

    signal(SIGALRM, handler);
    alarm(30);
    printf("What is shellcode?\n");
    read(0,sc,256);
    printf("Here is your code:\n%s\n",sc);
    if(filter(sc)==0)
        ((void (*) (void)) sc) ();
    else
        printf("You can't do that!\n");
    return 0;
}
