#include <iostream>

using namespace std;
class A{
public:
    int print_a()
    {
        return _a;
    }
private:
    static int _a;
};

int A::_a = 1;

int main()
{
    A a;
    cout << a.print_a() << endl; 
    return 0;
}