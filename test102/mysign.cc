#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>

using namespace std;
void sig_fuc(int sigid)
{
    cout << "进程收到信号：[" << sigid << "]进程id:["<< getpid() <<"]" <<endl;
}

int main()
{
    // signal(SIGALRM,sig_fuc);
    // alarm(1);
    // int count = 0;
    while(1)
    {
        cout << "hello sign" << endl;
        kill(getpid(),SIGFPE);
        // cout << count++ << endl;
    }
    // int pip[2];
    // pipe(pip);
    // pid_t pid = fork();
    // if(pid == 0)
    // {
    //     close(pip[0]);
    //     while(1)
    //     {
    //         sleep(1);
    //         write(pip[1],"hello pipe\n",strlen("hello pipe\n"));
    //     }
    //     exit(1);
    // }
    // else
    // {
    //     close(pip[1]);
    //     char buff[1024];
    //     for(int i = 0; i < 5; i++)
    //     {
    //         ssize_t sz = read(pip[0],buff,1024);
    //         buff[sz] = 0;
    //         cout << buff;
    //     }
    //     close(pip[0]);
    //     waitpid(-1,nullptr,0);
    // }
    return 0;
}