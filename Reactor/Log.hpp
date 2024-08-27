#pragma once 
#include <cstdarg>
#include <ctime>
#include <cstdio>

#define DEBUG 0
#define NORMAL 1
#define WARING 2
#define ERROR 3
#define FATAL 4

#define SHOW_DEBUG

const char* gLevelMap[5] = {
    "DEBUG",
    "NORMAL",
    "WARING",
    "ERROR",
    "FATAL"
};

void logMessage(int level,const char* format, ...)
{
#ifndef SHOW_DEBUG
    if(DEBUG == level)
        return;
#endif
    char stdBuffer[1024];
    snprintf(stdBuffer,sizeof(stdBuffer),"%s",gLevelMap[level]);
    char msgBUffer[1024];
    va_list args;
    va_start(args,format);
    vsnprintf(msgBUffer,sizeof(msgBUffer),format,args);
    va_end(args);
    printf("%s:%s\n",stdBuffer,msgBUffer);
}