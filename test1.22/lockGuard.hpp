#pragma once
#include <pthread.h>
#include <iostream>

class Mutex{
public:
    Mutex(pthread_mutex_t* mtx)
    :_mtx(mtx)
    {}
    void lock()
    {
        std::cout << "要进行加锁" << std::endl;
        pthread_mutex_lock(_mtx);
    }
    void unlock()
    {
        std::cout << "要进行解锁" << std::endl;
        pthread_mutex_unlock(_mtx);
    }
private:
    pthread_mutex_t* _mtx;
};

class lockGuard{
public:
    lockGuard(pthread_mutex_t* mtx):_mtx(mtx)
    {
        _mtx.lock();
    }
    ~lockGuard()
    {
        _mtx.unlock();
    }
private:
    Mutex _mtx;
};