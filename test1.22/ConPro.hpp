#pragma once

#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include "lockGuard.hpp"

#define DEFAULT_CAP 5

template <class T>
class BlockQueue
{
private:
    bool is_empty()
    {
        return _bq.size() == 0; 
    }
    bool is_full()
    {
        return _bq.size() == _capacity;
    }
public:
    BlockQueue(int capacity = DEFAULT_CAP)
    :_capacity(capacity)
    {
        pthread_mutex_init(&_mtx,nullptr);
        pthread_cond_init(&_full,nullptr);
        pthread_cond_init(&_empty,nullptr);
    }
    void push(const T& in)
    {
        lockGuard lg(&_mtx);
        while(is_full()) pthread_cond_wait(&_full,&_mtx);
        _bq.push(in);
        pthread_cond_signal(&_empty);
    }
    void pop(T* out)
    {
        lockGuard lg(&_mtx);
        while(is_empty()) pthread_cond_wait(&_empty,&_mtx);
        *out = _bq.front();
        _bq.pop();
        pthread_cond_signal(&_full);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mtx);
        pthread_cond_destroy(&_full);
        pthread_cond_destroy(&_empty);
    }
private:
    std::queue<T> _bq;
    int _capacity;
    pthread_mutex_t _mtx;
    pthread_cond_t _full;
    pthread_cond_t _empty;
};