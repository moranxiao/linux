#include "RingQueue.hpp"

void* product(void* args)
{
    RingQueue<int>* rq = (RingQueue<int>*)args;
    int i = -1;
    while(true)
    {
        //1.产生任务
        i++;
        //2.传递任务
        rq->push(i);
        std::cout << "product pthread:" <<  i << std::endl;
    }
    return nullptr;
}

void* consumer(void* args)
{
    RingQueue<int>* rq = (RingQueue<int>*)args;
    while(true)
    {
        int i;
        rq->pop(&i);
        std::cout << "consumer pthread:" << i << std::endl;
        
    }
    return nullptr;
}

int main()
{
    RingQueue<int> rq(5);
    pthread_t c[2],p[2];
    pthread_create(&c[1],nullptr,consumer,(void*)&rq);
    pthread_create(&c[0],nullptr,consumer,(void*)&rq);
    pthread_create(&p[0],nullptr,product,(void*)&rq);
    pthread_create(&p[1],nullptr,product,(void*)&rq);
    
    pthread_join(c[0],nullptr);
    pthread_join(c[1],nullptr);
    pthread_join(p[0],nullptr);
    pthread_join(p[1],nullptr);
    return 0;
}