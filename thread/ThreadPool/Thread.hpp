#pragma once

#include <pthread.h>
#include <string>
#include <cstdio>

class ThreadData{
public:
    std::string _name;
    void* _args;    
};

typedef void*(*func_t)(void*);

class Thread{
public:
    Thread(size_t num,func_t callback,void* args)
    :_func(callback)
    {
        _data._args = args;
        char buffer[100] = {0};
        snprintf(buffer,sizeof(buffer),"thread%lu",num);
        _data._name = buffer; 
        pthread_create(&_tid,nullptr,_func,&_data);       
    }
    ~Thread()
    {
        pthread_join(_tid,nullptr);
    }
private:
    ThreadData _data;
    pthread_t _tid;
    func_t  _func;
};