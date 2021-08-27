#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
using namespace std;

class Person
{
    public:
        Person();
        Person(string pname, int page);
        string getName() const;
        void setName();
        void setAge();
        int getAge() const;
        void print();

    private:
        string name;
        int age; //If 0 is unknown.
};

#endif // PERSON_H
