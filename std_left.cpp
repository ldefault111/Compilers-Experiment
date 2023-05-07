// std1使用条件：输入算式形式合法，输出左结合计算结果
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
string s;
bool flag;
void cal1(); // 三个数两个运算符无括号四则运算
void cal2(); // 四个数三个运算符一对括号四则运算
int checkmul(char); // 检查是否是乘除法
void aOpb(int&,int,char); // 两个数四则运算，结果保存在第一个数
void aOpbOpc(int&,int,int,char,char); // 三个数四则运算，结果保存在第一个数
void aOpbOpcOpd(int&,int,int,int,char,char,char); // 四个数四则运算，结果保存在第一个数
int solve(string); // 两or三or四个数的四则运算，返回计算结果
void error() // 除0报error
{
    flag=0;
    cout<<"divide 0:"<<s<<endl;
}
int main()
{
    while(cin>>s)
    {
        if(s.find('(')!=string::npos) cal2();
        else cal1();
    }
}
int checkmul(char x)
{
    return x=='*'||x=='/';
}
void aOpb(int& a,int b,char x) // a op b
{
    //cout<<a<<x<<b<<' ';
    if(x=='*')
        a*=b;
    else if(x=='/')
    {
        if(b==0)
            error();
        else
            a/=b;
    }
    else if(x=='+')
        a+=b;
    else if(x=='-')
        a-=b;
}
void aOpbOpc(int& sum,int b,int c,char x,char y) // sum op b op c
{
    //cout<<sum<<x<<b<<y<<c<<' ';
    if(!checkmul(x)&&checkmul(y)) // sum + b * c
    {
        aOpb(b,c,y);
        aOpb(sum,b,x);
    }
    else // 顺序计算
    {
        aOpb(sum,b,x);
        aOpb(sum,c,y);
    }
}
void aOpbOpcOpd(int& sum,int b,int c,int d,char x,char y,char z)
{   // a op b op c op d
    //cout<<sum<<x<<b<<y<<c<<z<<d<<' ';
    if(checkmul(x)) // sum * b op c op d
    {
        aOpb(sum,b,x); // sum op c op d
        aOpbOpc(sum,c,d,y,z);
    }
    else if(checkmul(y)) // a + b * c op d
    {
        aOpb(b,c,y); // b op c
        aOpbOpc(sum,b,d,x,z); // a op (b*c) op d
    }
    else if(checkmul(z)) // sum + b + c * d
    {
        aOpb(c,d,z);
        aOpbOpc(sum,b,c,x,y);
    }
    else
    {
        aOpb(sum,b,x); // sum op c op d
        aOpbOpc(sum,c,d,y,z);
    }
}
void cal1()
{
    flag=1;
    int sum=solve(s);
    if(flag)
        cout<<s<<"="<<sum<<endl;
}
void cal2()
{
    flag=1;
    auto l=s.find('(');
    auto r=s.find(')');
    string tmp=s;
    int sum=solve(tmp.substr(l+1,r-l-1)); // 取括号内的运算
    if(!flag)
        return;
    tmp.erase(l,r-l+1);
    tmp.insert(l,to_string(sum));
    sum=solve(tmp);
    if(!flag)
        return;
    cout<<s<<"="<<sum<<endl;
}
int solve(string str)
{
    //cout<<str<<':';
    stringstream sstream(str);
    int sum;
    sstream>>sum; // 至少一个数
    vector<int> nums;
    vector<char> ops;
    char x,y,z;
    int a,b,c,d;
    while(sstream>>x>>b)
    {
        //cout<<x<<' '<<b<<' ';
        nums.push_back(b);
        ops.push_back(x);
    }
    sstream.str("");
    sstream.clear();
    if(nums.size()==1) // a op b
        aOpb(sum,nums[0],ops[0]);
    else if(nums.size()==2) // a op b op c
        aOpbOpc(sum,nums[0],nums[1],ops[0],ops[1]);
    else if(nums.size()==3)  // a op b op c op d
        aOpbOpcOpd(sum,nums[0],nums[1],nums[2],ops[0],ops[1],ops[2]);
    //cout<<endl;
    return sum;
}


