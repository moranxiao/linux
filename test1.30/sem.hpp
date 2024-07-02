#pragma once
#include <semaphore.h>

class Sem{
public:
    Sem(int value)
    {
        sem_init(&_sem,0,value);
    }
    void p()
    {
        sem_wait(&_sem);
    }
    void v()
    {
       sem_post(&_sem); 
    }
    ~Sem()
    {
        sem_destroy(&_sem);
    }
private:
    sem_t _sem;
};