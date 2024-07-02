#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

// void handler(int sig)
// {
//     cout << "收到信号：" << sig << endl;
// }
// int main()
// {
//     struct sigaction act,oact;
//     act.sa_flags = 0;
//     act.sa_handler = handler;
//     sigemptyset(&act.sa_mask);
//     sigaddset(&act.sa_mask,SIGINT);
//     sigaction(SIGINT,&act,&oact);
//     while(1);
//     return 0;
// }

// void handler(int sig)
// {
//     while(waitpid(-1,nullptr,WNOHANG) > 0)
//     {
//         cout << "成功释放一个子进程" << endl;
//     }
       
// }

int main()
{
    signal(SIGCHLD,SIG_IGN);
    for(int i = 0; i < 10; i++)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            sleep(5);
            exit(0);
        }
        sleep(1);
    }
    while(1);
    return 0;
}