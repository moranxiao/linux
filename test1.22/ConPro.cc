#include "ConPro.hpp"
#include "lockGuard.hpp"

void* consumer(void* args)
{
    BlockQueue<int>* bq =(BlockQueue<int>*)args;

    while(true)
    {
        int a;
        bq->pop(&a);
        std::cout << "consumer thread:" << "a = " << a << std::endl;     
        sleep(1);
    }
    return nullptr;
}

void* productor(void* args)
{
    BlockQueue<int>* bq =(BlockQueue<int>*)args;
    int a = 0;
    while(true)
    {
        bq->push(a);
        std::cout << "productor thread:" << "a = " << a << std::endl;     
        sleep(1);
        a++;
    }

    return nullptr;
}

int main()
{
    BlockQueue<int> bq = BlockQueue<int>();
    pthread_t con_tid;
    pthread_t pro_tid;
    pthread_create(&con_tid,nullptr,consumer,&bq);
    pthread_create(&pro_tid,nullptr,productor,&bq);

    pthread_join(con_tid,nullptr);
    pthread_join(pro_tid,nullptr);
    return 0;
}