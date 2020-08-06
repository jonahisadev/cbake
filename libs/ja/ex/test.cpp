#define JA_IMPL
#include "JA/JA.h"
#include "JA/string.h"
#include "JA/vector.h"
#include "JA/log.h"

struct Person
{
    String name;
    int age;

    Person(String _name, int _age)
    {
        name = _name;
        age = _age;
    }

    void hello() const
    {
        String str = String("Hi, I'm {}{}{}, and I'm {}{}{} years old")
            .arg(Logger::LightGreen)
            .arg(name)
            .arg(Logger::ResetColor)
            .arg(Logger::LightBlue)
            .arg(age)
            .arg(Logger::ResetColor);

        jOut << str << endl;
    }
};

int main(int argc, char** argv)
{
    Vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);

    return 0;
}
