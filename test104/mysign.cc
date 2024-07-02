#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

void handler(int sig)
{
    cout << "收到信号：" << sig << endl;
}

void showPend()
{  
    sigset_t show;
    sigemptyset(&show);
    sigpending(&show);
    for(int i = 1; i <= 31; i++)
    {
        if(sigismember(&show,i))
        {
            cout << 1;
        }
        else
        {
            cout << 0;
        }
    }
    cout << endl;
}

int main()
{
    signal(SIGINT,handler);
    sigset_t set,oset;
    sigemptyset(&set);
    sigemptyset(&oset);
    sigaddset(&set,SIGINT);
    sigprocmask(SIG_BLOCK,&set,&oset);
    while(true)
    {
        sleep(1);
        showPend();
    }
    return 0;
}

// static void handler(int sig)
// {
//     cout << "捕获 sig:" << sig << endl;
// }
// int main()
// {
//     for(int sig = 1; sig <= 31; sig++)
//     {
//         signal(sig,handler);
//     }

//     while(true);
//     return 0;
// }