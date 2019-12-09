#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

char* code = "\xb9\x00\x10\x00\x00\x90\x90\x90"
             "\x90\xff\xc9\x75\xf8\xc3";

void hack(char* cmd){
    printf("Hacked!\n");
    char* const arg[] = {cmd,NULL};
    char* const env[] = {NULL};
    execve(cmd,arg,env);
}

void* make_page(){
    return mmap(0,0x1000,0b011,0b100010,-1,0);
}

void make_page_rx(void* p){
    if(mprotect(p,0x1000,0b101) == -1){
        printf("error!\n");
        exit(0);
    }
}

void make_page_rw(void* p){
    if(mprotect(p,0x1000,0b011) == -1){
        printf("error!\n");
        exit(0);
    }
}

void copy(char* p){
    for(int i=0;i<14;i++){
        p[i] = code[i];
    }
}

void loop(char* p){
    make_page_rw(p);
    copy(p);
    read(0,p+5,4);
    make_page_rx(p);
    ((void(*)())p)();
}

int main(){
    setvbuf(stdin, 0LL, 2, 0LL);
    setvbuf(stdout, 0LL, 2, 0LL);
    setvbuf(stderr, 0LL, 2, 0LL);

    char* page = make_page();
    while(1){
        loop(page);
    }
    return 0;
}
