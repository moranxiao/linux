#pragma once
#include <pthread.h>
#include <string>

typedef void* (*func_t)(void*);

class threadData{
public:
    std::string _name;
    void* _args;
};

class Thread{
public:
    Thread(int num,func_t func,void* args)
    :_func(func)
    {
        char buffer[1024];
        snprintf(buffer,1024,"thread%d:",num);
        _data._name = buffer;
        _name = buffer;
        _data._args = args;      
    }
    void start()
    {
        pthread_create(&_tid,nullptr,_func,(void*)&_data);
    }
    void join()
    {
        pthread_join(_tid,nullptr);
    }
    std::string name()
    {
        return _name;
    }
private:
    pthread_t _tid;
    std::string _name;
    func_t _func;
    threadData _data;
};