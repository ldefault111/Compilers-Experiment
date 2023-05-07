#include<stdlib.h>
#include<stdio.h>
#include<time.h>

//#define debug
#define filename "test1.in"

static char op[4]={'+','-','*','/'};

static int N=4,M=101,CNT=100;
void rand1();

int main()
{
    srand(time(0));
    for(int i=1;i<=CNT;i++)
        rand1();
    return 0;
}

void rand1()
{
    int a=rand()%M,b=rand()%M,c=rand()%M;
    char x=op[rand()%N],y=op[rand()%N];
    printf("%d%c%d%c%d\n",a,x,b,y,c);
}
