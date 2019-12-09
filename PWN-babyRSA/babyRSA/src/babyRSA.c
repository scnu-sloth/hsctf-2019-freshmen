#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define MAX_NUM 10000
char notPrime[MAX_NUM+1] = {1,1,0};

/******************** RSA **********************/

void init_pro(){
    int fd = open ("/dev/urandom", O_RDONLY);
    if(fd>0){
        unsigned int seed = 0;
        read(fd,&seed,sizeof(seed));
        //printf("%u\n",seed);
        srand(seed);
        close(fd);
    }else{
        printf("Something wrong!\nPlease tell administrator.\n");
    }

    for(int i=0;i*i<=MAX_NUM;i++){
        if(!notPrime[i]){
            for(int j=i+i;j<=MAX_NUM;j+=i){
                notPrime[j] = 1;
            }
        }
    }    
}

//[0]->p | [1]->q
int* genTwoPrime(){
    int a=rand()%100+1;
    int b=rand()%100+1;
    while(a<3 || notPrime[a])
        a=rand()%100+1;
    while(b<3 || b==a || notPrime[b])
        b=rand()%100+1;
    //printf("%d %d\n",a,b);
    int* result = (int*)malloc(sizeof(int)*2);
    result[0] = a;
    result[1] = b;
    return result;
}

int gcd(int a,int b){
    if(b==0)
        return a;
    return gcd(b,a%b);
}

// n^a === r % m
int getMod(int n,int a,int m){
    int r = 1;
    for(int i=0;i<a;i++){
        r = (r%m*n%m)%m;
    }
    return r;
}

//ax+by = gcd(a,b)
//[0]->a | [1]->b
int* egcd(int* ab){
    int a=ab[0],b=ab[1];
    free(ab);
    ab = 0;
    //[0]->x | [1]->y
    int* result = (int*)malloc(sizeof(int)*2);
    if(b==0){
        result[0] = 1;
        result[1] = 0;
        return result;
    }else{
        result[0] = b;
        result[1] = a%b;
        result = egcd(result);
        int tmp = result[0]-(a/b)*result[1];
        result[0] = result[1];
        result[1] = tmp;
        return result;
    }
}

//[0]->n | [1]->e | [2]->d
int* genRSA(){
    while(1){
        // get p q
        int* tp = genTwoPrime();
        int p = tp[0];
        int q = tp[1];
        //printf("p->%d\nq->%d\n",p,q);
        int fine = (p-1)*(q-1);
        //printf("fine->%d\n",fine);
        //get n
        int n = p*q;
        if(n<500)  // be simple if too small
            continue;
        //printf("n->%d\n",n);
        free(tp);
        tp = 0;
        //get e
        int e = rand()%(fine-2)+2;
        while(gcd(fine,e)!=1)
            e = rand()%(fine-2)+2;
        //printf("e->%d\n",e);
        //get d
        int* xy = (int*)malloc(sizeof(int)*2); 
        xy[0] = e;
        xy[1] = fine;
        xy = egcd(xy);
        //printf("x->%d\ny->%d\n",xy[0],xy[1]);
        int d = xy[0];
        if(d<=0) continue;
        free(xy);
        xy = 0;
        //printf("d->%d\n",d);
        //return
        int* result = (int*)malloc(sizeof(int)*3);
        result[0] = n;
        result[1] = e;
        result[2] = d;
        return result; 
    }
}

int encrypt(int m,int e,int n){
    return getMod(m,e,n);
}

int decrypt(int c,int d,int n){
    return getMod(c,d,n);
}

/******************* PWN *********************/

void hack(){
    printf("Hello hacker!~ \n");
    system("/bin/sh\x00");
    printf("\n");
}

void (*f)() = & hack;

/******************* MAIN *********************/

int main(){
    setvbuf(stdin, 0LL, 2, 0LL);
    setvbuf(stdout, 0LL, 2, 0LL);
    setvbuf(stderr, 0LL, 2, 0LL);
    init_pro();
    int* ned = genRSA();
    int n = ned[0];
    int e = ned[1];
    int d = ned[2];
    free(ned);
    ned = 0;
    printf("n = %d\ne = %d\nd = %d\n",n,e,d);


    int m = rand()%(n-200)+1;
    printf("\n");
    int enc = encrypt(m,e,n);
    printf("crypted = %d\n",enc);
    printf("message = ");
    char input[100];
    gets(input);
    int check = atoi(input);

    if(check==m){
        printf("You're right!\n");
        printf("\n");
        return 0;
    }

    printf("You failed!\n");
    exit(0);
}
