#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MSGSIZE 16

const char *msg1="hello,#1";
const char *msg2="hello,#2";
const char *msg3="hello,#3";
int main()
{
  char inbuf[MSGSIZE];
  int p[2],j,pid;

  if(pipe(p)<0)
    {  perror("pipe call err");
       exit(1);
    }

  if((pid=fork())<0)
     { perror("fork call err");
      exit(2);
     }

   if (pid>0) //父进程
      {  close(p[0]);     /*关闭父进程读*/
         write(p[1],msg1,MSGSIZE); 
write(p[1],msg2,MSGSIZE);
write(p[1],msg3,MSGSIZE);
          wait(0);
       }
   
   if(pid==0) //子进程
     { close(p[1]);     /*关闭子进程写*/
        for(j=0;j<3;j++)
            { read(p[0],inbuf,MSGSIZE);
              printf("------in child--------%s\n",inbuf);
            }
         exit(0);
      }
  exit(0);
  return 0;
}

