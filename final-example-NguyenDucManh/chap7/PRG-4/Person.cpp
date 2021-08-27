#include "Person.h"
#include <iostream>

using namespace std;

Person::Person()
{
    name = "";
    age = 0;
}

Person::Person (string pname, int page)
{
    pname = name;
    page = age;
}

string Person::getName() const
{
    return name;
}

int Person::getAge() const
{
    return age;
}

void Person::print()
{
    cout << "Name : " << name << endl;
    cout << "Age : " << age << endl;
}

void Person::setName()
{
    cout << "What is the name" << endl;
    cin >> name;
}

void Person::setAge()
{
    cout << "What is the age" << endl;
    cin >> age;
}
