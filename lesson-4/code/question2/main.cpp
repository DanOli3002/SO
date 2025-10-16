#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <libgen.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "process.h"

int main (void)
{

    int shmid = pshmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    int *counter = (int *)pshmat(shmid, NULL, 0);

    *counter = 1;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        pshmdt(counter);
        pshmctl(shmid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);

    }
    if (pid == 0) {
        // Child Process
        int N;
        do {
            printf("Enter a value between 10 and 20: ");
            fflush(stdout);
            if (scanf("%d", &N) != 1) {
                while (getchar() != '\n'); // clear invalid input
                N = 0;
            }
        } while(N < 10 || N > 20);

        while (*counter <= N) {
            printf("%d\n", *counter);
            (*counter)++;
            fflush(stdout);
            sleep(200000);
        }

        pshmdt(counter);
        pshmctl(shmid, IPC_RMID, NULL);
    } else {
        // Parent Process

        wait(NULL); // Wait for child to finish

        while (*counter >= 1)  {
        
            printf("%d\n", *counter);
            (*counter)--;
            fflush(stdout);
            sleep(200000);

        }

        pshmdt(counter);
        pshmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}

