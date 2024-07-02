#pragma once

#include <pthread.h>
#include "log.hpp"

class Mutex
{
public:
    Mutex(pthread_mutex_t* mtx)
    :_mtx(mtx)
    {
        pthread_mutex_init(_mtx,nullptr);
    }
    void lock()
    {
        pthread_mutex_lock(_mtx);
    }
    void unlock()
    {
        pthread_mutex_unlock(_mtx);
    }
    ~Mutex()
    {
        pthread_mutex_destroy(_mtx);
    }
public:
    pthread_mutex_t* _mtx;
};

class lockGuard{
public:
    lockGuard(pthread_mutex_t* mtx)
    :_mtx(mtx)
    {
        _mtx.lock();
        logMessage(DEBUG,"加锁成功");   
    }
    ~lockGuard()
    {
        _mtx.unlock();
        logMessage(DEBUG,"解锁成功");
    }
private:
    Mutex _mtx;
};