#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

void waiting(int),stop(int);
int wait_mark;
int main()
{ int p1,p2;
 while ((p1=fork())==-1);
 if (p1>0)
   { while ((p2=fork())==-1) ;
if (p2>0)
  { wait_mark=1;
    alarm(1);signal(SIGALRM,stop);
    waiting(0);
    kill(p1,16);
    kill(p2,17);
    wait(0);
    wait(0);
    printf("parent process is killed!\n");
    exit(0);
  }
else 
  {wait_mark=1;
   signal(17,stop);
   waiting(0);
   lockf(1,1,0);
printf("child process 2 is killed by parent!\n");
lockf(1,0,0);
exit(0);
}
   }
else
{wait_mark=1;
   signal(16,stop);
   waiting(0);
   lockf(1,1,0);
printf("child process 1 is killed by parent!\n");
lockf(1,0,0);
exit(0);
}
return 0;
}
void waiting(int)
{
  while (wait_mark !=0);
}
void stop(int)
{
  wait_mark=0;
}

