#include<stdlib.h>
#include<stdio.h>
#include<time.h>

static char op[4]={'+','-','*','/'};
static char pr[10][20]={
    "(%d)%c%d%c%d%c%d","%d%c(%d)%c%d%c%d","%d%c%d%c(%d)%c%d","%d%c%d%c%d%c(%d)",
    "(%d%c%d)%c%d%c%d","%d%c(%d%c%d)%c%d","%d%c%d%c(%d%c%d)",
    "(%d%c%d%c%d)%c%d","%d%c(%d%c%d%c%d)",
    "(%d%c%d%c%d%c%d)"};

static int N=4,M=101,P=10,CNT=100;
void rand2();

int main()
{
    srand(time(0));
    for(int i=1;i<=CNT;i++)
        rand2();
    return 0;
}

void rand2()
{
    int a=rand()%M,b=rand()%M,c=rand()%M,d=rand()%M;
    char x=op[rand()%N],y=op[rand()%N],z=op[rand()%N];
    char str[20];
    sprintf(str,pr[rand()%P],a,x,b,y,c,z,d);
    puts(str);
}
