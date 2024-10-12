#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main()
{
	pid_t i = fork();
	if(i == 0) //子进程1
	{
		cout << 'b' << endl;
	}
	else if(i > 0) //父进程
	{
		pid_t j = fork();
		if(j > 0) //父进程
		{
			cout << 'a' << endl;
			waitpid(i,nullptr,0);	
			waitpid(j,nullptr,0);	
		}
		else if(j == 0) //子进程2
		{
			cout << 'c' << endl;
		}
	}
	return 0;
}
