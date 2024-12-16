#include "RingQueue.hpp"

#include <iostream>
#include <ctime>
#include <unistd.h>

using namespace std;

void* Consumer(void* args)
{
    RingQueue<int>* prq = (RingQueue<int>*)args;
    while(1)
    {
        int x = prq->Pop();
        cout << "消费者线程[" << pthread_self() << "]正在运行:拿出"  << x << endl;
    }
    return nullptr;
}

void* Producer(void* args)
{
    RingQueue<int>* prq = (RingQueue<int>*)args;
    
    while(1)
    {
        int x = (rand() % 100) + 1;
        prq->Push(x);
        cout << "生产者线程[" << pthread_self() << "]正在运行:放入" << x << endl;
        sleep(1);
    }
    return nullptr;
}

static const size_t N = 4;
int main()
{
    srand(time(nullptr));
    pthread_t ptid[N];
    pthread_t ctid[N];
    RingQueue<int> bq;
    for(int i = 0; i < N; i++)
    {
        pthread_create(ptid+i,nullptr,Producer,&bq);
        pthread_create(ctid+i,nullptr,Consumer,&bq);
    }
    for(int i = 0; i < N; i++)
    {
        pthread_join(ptid[i],nullptr);
        pthread_join(ctid[i],nullptr);
    }
    return 0;
}