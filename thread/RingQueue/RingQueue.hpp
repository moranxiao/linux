#pragma once

#include <pthread.h>
#include <vector>
#include <semaphore.h>

template<class T>
class RingQueue{
    static const size_t defaultCapacity = 5;
public:
    RingQueue(size_t capacity = defaultCapacity)
    :_capacity(capacity),_tasks(capacity)
    {
        pthread_mutex_init(&_conMtx,nullptr);
        pthread_mutex_init(&_proMtx,nullptr);
        sem_init(&_capSem,0,capacity);
        sem_init(&_dataSem,0,0);
    }
    void Push(const T& task)
    {
        sem_wait(&_capSem);
        pthread_mutex_lock(&_proMtx);
        _tasks[_proIndex] = task;
        _proIndex = (_proIndex + 1) % _capacity; 
        pthread_mutex_unlock(&_proMtx);
        sem_post(&_dataSem);
    }
    T Pop()
    {
        sem_wait(&_dataSem);
        pthread_mutex_lock(&_conMtx);
        T task = _tasks[_conIndex];
        _conIndex = (_conIndex + 1) % _capacity;
        pthread_mutex_unlock(&_conMtx);
        sem_post(&_capSem);
        return task;
    }
    ~RingQueue()
    {
        pthread_mutex_destroy(&_conMtx);
        pthread_mutex_destroy(&_proMtx);
        sem_destroy(&_dataSem);
        sem_destroy(&_capSem);
    }

private:
    std::vector<T> _tasks;
    size_t _capacity;
    size_t _conIndex = 0;
    size_t _proIndex = 0;
    pthread_mutex_t _conMtx;
    pthread_mutex_t _proMtx;
    sem_t _dataSem;
    sem_t _capSem;
};