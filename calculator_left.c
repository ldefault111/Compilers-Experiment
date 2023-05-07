#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char token; /* global token variable */
/*function prototype for recursive calls*/
int exp(void);
int term(void);
int factor(void);
int calculator(void);
char str[20];
int pt=0;
int flag=1;
void error(int x)
{
    if(!x)
    {
        puts("error");
        exit(1);
    }
    flag=0;
    while(token!='\n')
        str[pt++]=(token=getchar());
    str[--pt]='\0';
    printf("divide 0:%s\n",str);
}

int main()
{
    while(calculator());
    return 0;
}

int calculator()
{
    pt=0;
    flag=1;
    int result;
    /*load token with first character for lookahead*/
    token=getchar();
    if(token==EOF)
        return 0;
    str[pt++]=token;
    result=exp();
    if(!flag)
        return 1;
    if (token=='\n') /*check for end of line*/
    {
        str[--pt]='\0';
        printf("%s=%d\n", str,result);
        return 1;
    }
    error(0); /*extraneous chars on line*/
    return 1;
}
void match(char expectedToken)
{
    if (token==expectedToken)
    {
        token=getchar();
        str[pt++]=token;
    }
    else error(0);
}
int exp(void)
{
    int temp=term();
    if(!flag)
        return 0;
    while ((token=='+') || (token=='-'))
        switch (token)
        {
            case '+':
                match ('+');
                temp+=term();
                break;
            case '-':
                match ('-');
                temp-=term();
                break;
        }
    return temp;
}
int term(void)
{
    int temp=factor();
    if(!flag)
        return 0;
    while ((token=='*') || (token=='/'))
        switch (token)
        {
            case '*':
                match('*');
                temp*=factor();
                break;
            case '/':
                match('/');
                int t=factor();
                if(!t)
                {
                    error(1);
                    return 0;
                }
                temp/=t;
                break;
        }
    return temp;
}
int factor(void)
{
    int temp;
    if (token=='(')
    {
        match('(');
        temp = exp();
        if(!flag)
            return 0;
        match(')');
    }
    else if (isdigit(token))
    {
        ungetc(token,stdin);
        pt--;
        scanf("%d",&temp);
        pt+=sprintf(&str[pt],"%d",temp);
        token=getchar();
        str[pt++]=token;
    }
    else error(0);
    return temp;
}
