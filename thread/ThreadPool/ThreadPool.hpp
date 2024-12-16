#pragma once

#include "Thread.hpp"
#include <vector>
#include <queue>

template<class T>
class ThreadPool{
    static const size_t defaultThreadNum = 5;
public:
    ThreadPool(size_t threadNum = defaultThreadNum)
    :_threadNum(threadNum),_threads(threadNum)
    {
        pthread_mutex_init(&_mtx,nullptr);
        pthread_cond_init(&_cond,nullptr);
    }
    void Run()
    {
        for(size_t i = 0; i < _threadNum; i++)
        {
            _threads[i] = new Thread(i,Routinue,this);
        }
    }
    static void* Routinue(void* args)
    {
        ThreadData* ptd = (ThreadData*)args;
        ThreadPool<T>* ptp = (ThreadPool<T>*)ptd->_args;
        while(1)
        {
            pthread_mutex_lock(&ptp->_mtx);
            while(ptp->Empty()) pthread_cond_wait(&ptp->_cond,&ptp->_mtx);
            T task = ptp->_tasks.front();
            ptp->_tasks.pop();
            pthread_mutex_unlock(&ptp->_mtx);
            task();
        }
        return nullptr;
    }
    void Push(const T& task)
    {
        pthread_mutex_lock(&_mtx);
        _tasks.push(task);
        pthread_mutex_unlock(&_mtx);
        pthread_cond_signal(&_cond);
    }
    ~ThreadPool()
    {
        for(size_t i = 0; i < _threadNum; i++)
        {
            delete _threads[i];
        }
        pthread_mutex_destroy(&_mtx);
        pthread_cond_destroy(&_cond);
    }
private:
    bool Empty()
    {
        return _tasks.empty();
    }
private:
    std::vector<Thread*> _threads;
    size_t _threadNum;
    std::queue<T> _tasks;
    pthread_mutex_t _mtx;
    pthread_cond_t _cond;
};