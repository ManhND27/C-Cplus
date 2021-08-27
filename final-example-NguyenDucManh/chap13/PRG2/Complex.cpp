#include<stdlib.h>
#include<iostream>
#include<conio.h>
using namespace std;
class Complex{
private:
float pt,pa;
public:
Complex()
{
pt=0; pa=0;
}
void add()
{
cout<<"Moi nhap phan thuc: ";cin>>pt;
cout<<"\nMoi nhap phan ao: ";cin>>pa;
}
void display()
{
if (pa<0) cout<<pt<<pa<<"*";
else cout<<pt<<"+"<<pa<<"*i";
}
friend Complex operator +(Complex a,Complex b);
friend Complex operator -(Complex a,Complex b);
};
Complex operator +(Complex a,Complex b)
{ Complex c;
c.pt=a.pt+b.pt;
c.pa=a.pa+b.pa;
return c;
}
Complex operator -(Complex a,Complex b)
{ Complex c;
c.pt=a.pt-b.pt;
c.pa=a.pa-b.pa;
return c;
}
