#include <vector>
#include "Person.h"

constexpr int MAX_STUDENTS = 10;

int main()
{
    std::vector<Person> people;

    for( int i = 0 ; i < MAX_STUDENTS ; i++ ) {
        Person student{};
        student.setName();
        student.setAge();

        people.push_back( student );
    }
}
