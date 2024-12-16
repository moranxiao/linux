#pragma once

#include <pthread.h>


class Mutex{
public:
    Mutex(pthread_mutex_t* pmtx)
    :_pmtx(pmtx)
    {}
    void Lock()
    {
        pthread_mutex_lock(_pmtx);
    }
    void Unlock()
    {
        pthread_mutex_unlock(_pmtx);
    }
private:
    pthread_mutex_t* _pmtx;
};


class LockGuard{
public:
    LockGuard(pthread_mutex_t* pmtx)
    :_mtx(pmtx)
    {
        _mtx.Lock();
    }
    ~LockGuard()
    {
        _mtx.Unlock();
    }
private:
    Mutex _mtx;
};