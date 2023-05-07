// std1ʹ��������������ʽ��ʽ�Ϸ���������ϼ�����
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
string s;
bool flag;
void cal1(); // �����������������������������
void cal2(); // �ĸ������������һ��������������
int checkmul(char); // ����Ƿ��ǳ˳���
void aOpb(int&,int,char); // �������������㣬��������ڵ�һ����
void aOpbOpc(int&,int,int,char,char); // �������������㣬��������ڵ�һ����
void aOpbOpcOpd(int&,int,int,int,char,char,char); // �ĸ����������㣬��������ڵ�һ����
int solve(string); // ��or��or�ĸ������������㣬���ؼ�����
void error() // ��0��error
{
    flag=0;
    cout<<"divide 0:"<<s<<endl;
}
int main()
{
    while(cin>>s)
    {
        //cout<<s<<endl;
        if(s.find('(')!=string::npos) // ѡ��
            cal2();
        else // ����
            cal1();
        //cout<<endl;
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
    if(checkmul(x)&&!checkmul(y)) // sum * b + c
    {
        aOpb(sum,b,x);
        aOpb(sum,c,y);
    }
    else // �������
    {
        aOpb(b,c,y);
        aOpb(sum,b,x);
    }
}
void aOpbOpcOpd(int& sum,int b,int c,int d,char x,char y,char z)
{   // a op b op c op d
    //cout<<sum<<x<<b<<y<<c<<z<<d<<' ';
    if(checkmul(z)) // sum op b op c * d
    {
        aOpb(c,d,z); // c op d
        aOpbOpc(sum,b,c,x,y); // sum op b op (c*d)
    }
    else if(checkmul(y)) // sum op b * c + d
    {
        aOpb(b,c,y); // b op c
        aOpbOpc(sum,b,d,x,z); // sum op (b*c) op d
    }
    else if(checkmul(x)) // sum * b + c + d
    {
        aOpb(sum,b,x); // a op b
        aOpbOpc(sum,c,d,y,z);
    }
    else // a + b + c + d
    {
        aOpb(c,d,z); // c op d
        aOpbOpc(sum,b,c,x,y); // sum op b op (c*d)
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
    int sum=solve(tmp.substr(l+1,r-l-1)); // ȡ�����ڵ�����
    if(!flag)
        return;
    tmp.erase(l,r-l+1); // ȥ�����ż�������
    tmp.insert(l,to_string(sum)); // �����ŵ�����������ԭ�ַ���
    sum=solve(tmp);
    if(!flag)
        return;
    cout<<s<<"="<<sum<<endl;
}
int solve(string str)
{
    stringstream sstream(str);
    int sum;
    sstream>>sum; // ����һ����
    vector<int> nums; // ����
    vector<char> ops; // ����
    char x; int b;
    while(sstream>>x>>b)
    {
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
    return sum;
}


