#include <iostream>

class A{
private:
    static const int a;
public:
    A(const int& b = a)
    :_a(b)
    {}
private:
    int _a;
};
const int A::a = 10;
int main()
{
    A a;
    return 0;
}
