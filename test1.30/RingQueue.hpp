#pragma once
#include <iostream>
#include <vector>
#include "sem.hpp"
#include <unistd.h>
#define DEFAULT_CAP 5

template<class T>
class RingQueue{
public: 
    RingQueue(int capacity = DEFAULT_CAP)
    :_rq(capacity),
     _capacity(capacity),
     space_sem(capacity),
     data_sem(0),
     _p_index(0),
     _c_index(0)
    {
        pthread_mutex_init(&p_mtx,nullptr);
        pthread_mutex_init(&c_mtx,nullptr);
    }
    void push(const T& in)
    {
        space_sem.p();
        pthread_mutex_lock(&p_mtx);
        _rq[_p_index++] = in;
        _p_index %= _capacity;
        data_sem.v(); 
        pthread_mutex_unlock(&p_mtx);
    }
    void pop(T* out)
    {
        data_sem.p();
        pthread_mutex_lock(&c_mtx);
        *out = _rq[_c_index++];
        _c_index %= _capacity;
        space_sem.v();
        pthread_mutex_unlock(&c_mtx);
    }
    ~RingQueue()
    {
        pthread_mutex_destroy(&c_mtx);
        pthread_mutex_destroy(&p_mtx);
    }
private:
    std::vector<T> _rq;
    int _capacity;
    Sem space_sem;
    Sem data_sem;
    int _p_index;
    int _c_index;
    pthread_mutex_t p_mtx;
    pthread_mutex_t c_mtx;
};