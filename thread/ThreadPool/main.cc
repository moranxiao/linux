#include "ThreadPool.hpp"
#include <unistd.h>
#include <iostream>

using namespace std;

class Task{
public:
    void operator()()
    {
        cout << "you can see me!" << endl;
    }
};


int main()
{
    ThreadPool<Task> tp;
    tp.Run();
    while(1)
    {
        tp.Push(Task());
        sleep(1);
    }
    return 0;
}