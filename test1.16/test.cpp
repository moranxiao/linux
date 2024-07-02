#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;
void* threadRun(void* args)
{
    while(1)
    {
        sleep(1);
        cout << (char*)args << endl;
    }
    return (void*)0;
}

int main()
{
    pthread_t tid;    
    char name[64];
    snprintf(name,sizeof(name),"%s-%d","son thread",1);
    pthread_create(&tid,nullptr,threadRun,(void*)name);
    while(1)
    {
        cout << "main thread" <<endl;
        sleep(1);
    }

    return 0;
}