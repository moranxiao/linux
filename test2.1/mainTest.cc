#include "threadPool.hpp"
#include "lockGuard.hpp"

#include <unistd.h>


int main()
{
    threadPool<int>* tp = threadPool<int>::get_threadPool();
    tp->run();
    int i = 0;
    while(true)
    {
        tp->pushTask(i);
        i++;
        logMessage(NORMAL, "pushTask成功：i=%d", i);
        sleep(1);
    }
    return 0;
}
 
