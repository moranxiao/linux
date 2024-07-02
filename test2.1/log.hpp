#pragma once
#include <cstdarg>
#include <ctime>
#include <cstdio>

#define DEBUG 0
#define NORMAL 1
#define WARING 2
#define ERROR 3
#define FATAL 4

const char* gLevelMap[5] = {
    "DEBUG",
    "NORMAL",
    "WARING",
    "ERROR",
    "FATAL"
};

void logMessage(int level,const char* format,...)
{
#ifndef DEBUG_SHOW
    if(level == DEBUG)
        return;
#endif
    char stdbuffer[1024];
    snprintf(stdbuffer,1024,"[%s]:[%ld] ", gLevelMap[level], time(nullptr));
    va_list args;
    va_start(args,format);
    char logbuffer[1024];
    vsnprintf(logbuffer,1024,format,args);
    va_end(args);
    printf("%s%s\n", stdbuffer,logbuffer);    
}
