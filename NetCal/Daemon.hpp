#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <iostream>

//改变为守护进程
void Daemon()
{
    //1.屏蔽信号
    signal(SIGPIPE,SIG_IGN);
    signal(SIGCHLD,SIG_IGN);
    
    //2.不让自己成为组长
    int pid = fork();
    if(pid > 0)
        exit(0);

    //3.改变为守护进程
    setsid();

    //4.关闭标准输入输出错误，守护进程不能向终端发送和接受数据，如果发送或者接受会出现错误
    int devnull = open("/dev/null",O_RDWR);
    if(devnull < 0)
    {
        std::cout << "open devnull error\n";
        exit(-1);
    }
    dup2(devnull,0);
    dup2(devnull,1);
    dup2(devnull,2);
    close(devnull);

}
