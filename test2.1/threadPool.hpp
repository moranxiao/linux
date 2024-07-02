#pragma once
#include "thread.hpp"
#include <vector>
#include <queue>
#include "log.hpp"
#include "lockGuard.hpp"

#define DEFAULT_THREADNUM 3

template <class T>
class threadPool{
public:
    bool is_empty()
    {
        return _task_queue.empty();
    }
    T front()
    {
        return _task_queue.front();
    }
    void pop()
    {
        _task_queue.pop();
    }
private:
    threadPool(int num = DEFAULT_THREADNUM)
    :_num(num)
    {
        for(int i = 1; i <= _num; i++)
        {
            _tp.push_back(new Thread(i,routine,this));
            logMessage(DEBUG, "%s:%s",_tp[i-1]->name().c_str(),"创建线程成功");
        }
        pthread_mutex_init(&_mtx,nullptr);
        pthread_cond_init(&_cond,nullptr);
    }
    threadPool(const threadPool<T>& other) = delete;
    const threadPool<T>& operator=(const threadPool<T>& other) = delete;
public:
    static threadPool<T>* get_threadPool(int num = DEFAULT_THREADNUM)
    {
        if(nullptr == thread_pool)
        {
            lockGuard lg(&tpmtx);
            if(nullptr == thread_pool)
                thread_pool = new threadPool<T>(num);
        }
        return thread_pool;
    }
    static void* routine(void* args)
    {
        threadData* td = (threadData*)args;
        logMessage(DEBUG,"%s启动成功", td->_name.c_str());
        threadPool<T>* tp = (threadPool<T>*)td->_args;
        while(true)
        {
            T task;
                {
                    lockGuard ld(&tp->_mtx);
                    while(tp->is_empty())
                        pthread_cond_wait(&tp->_cond,&tp->_mtx);
                    task = tp->front();
                    tp->pop();
                }
            logMessage(NORMAL,"%s处理任务ing 结果：%d",td->_name.c_str(),task);
        }
        return nullptr;
    }
    void pushTask(const T& task)
    {
        lockGuard ld(&_mtx);
        _task_queue.push(task);
        pthread_cond_signal(&_cond);
    }
    void run()
    {
        for(int i = 0; i < _num; i++)
        {
            _tp[i]->start();
        }
    }
    ~threadPool()
    {
        for(int i = 0; i < _num; i++)
        {
            _tp[i]->join();
            delete _tp[i];
            printf("线程释放成功\n");
        }
        pthread_mutex_destroy(&_mtx);
        pthread_cond_destroy(&_cond);
    }
private:
    std::vector<Thread*> _tp;
    int _num;
    std::queue<T> _task_queue;
    pthread_mutex_t _mtx;
    pthread_cond_t _cond;
    static threadPool<T>* thread_pool;
    static pthread_mutex_t tpmtx;
};

template<class T>
threadPool<T>* threadPool<T>::thread_pool = nullptr;

template<class T>
pthread_mutex_t threadPool<T>::tpmtx = PTHREAD_MUTEX_INITIALIZER;