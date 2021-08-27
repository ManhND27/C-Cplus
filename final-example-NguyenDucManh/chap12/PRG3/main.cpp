#include<iostream>
using namespace std;
class Employee
{
private:
    string name;
    float rate;
    float hour;
    float salary;
public:
    void Input();
    void Output();
};
void Employee::Input()
{
     cout << "Enter Name: ";
     cin >> name;
     cout << "Enter rate: ";
     cin >> rate;
     cout << "Enter hour: ";
     cin >> hour;
     salary = rate * hour;
 }
 void Employee::Output()
 {
     cout << "Name: " << name << endl;
     cout <<"Salary: " << salary << endl;
 }
 int main()
 {
     Employee e;
     cout << "Enter the employee information:" << endl;
         e.Input();
     cout << "The employee information is:" <<endl;
         e.Output();

 }
