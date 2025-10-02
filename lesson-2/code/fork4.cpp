#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "delays.h"
#include "process.h"

int main(void)
{
  printf("Before the fork: PID = %d, PPID = %d\n", getpid(), getppid());

  pid_t ret = pfork();
  if (ret == 0)
  {
    // o execl substitui o processo atual por outro
    // se execl der "erro", o processo filho deixa de existir
    // e passa a ser o processo "child" com mesmo PID
    // usamos o pexecl, que faz o tratamento de erro para nós
    pexecl("./child", "./child", NULL);
    printf("why doesn't this message show up?\n");
    return EXIT_FAILURE;
  }
  else
  {
    //pwait(NULL);
    printf("I'm the parent: PID = %d, PPID = %d\n", getpid(), getppid());
    usleep(1000);
  }

  return EXIT_SUCCESS;
}
