#pragma once

#include <pthread.h>
#include <queue>

#include "../LockGuard.hpp"

template<class T>
class BlockQueue{
    static const size_t defaultSize = 10;
public:
    BlockQueue(size_t size = defaultSize)
    :_size(size)
    {
        pthread_mutex_init(&_mtx,nullptr);
        pthread_cond_init(&_full,nullptr);
        pthread_cond_init(&_empty,nullptr);
    }
    void Push(const T& task)
    {
        pthread_mutex_lock(&_mtx);
        while(Full()) pthread_cond_wait(&_full,&_mtx);
        _tasks.push(task);
        pthread_mutex_unlock(&_mtx);
        pthread_cond_signal(&_empty);        
    }
    T Pop()
    {
        pthread_mutex_lock(&_mtx);
        while(Empty()) pthread_cond_wait(&_empty,&_mtx);
        T task = _tasks.front();
        _tasks.pop();   
        pthread_mutex_unlock(&_mtx);
        pthread_cond_signal(&_full);
        return task;
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mtx);
        pthread_cond_destroy(&_full);
        pthread_cond_destroy(&_empty);
    }
private:
    bool Full()
    {
        return _size == _tasks.size();
    }
    bool Empty()
    {
        return _tasks.size() == 0;
    }
private:
    std::queue<T> _tasks;    
    size_t _size;
    pthread_mutex_t _mtx;
    pthread_cond_t _full;
    pthread_cond_t _empty;
};