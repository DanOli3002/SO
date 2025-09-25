
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "delays.h"
#include "process.h"

int main(void){
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        // Child process prints 1-10
        for (int i = 1; i <= 10; ++i) {
            printf("%d\n", i);
        }
        fflush(stdout);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process waits for child
        wait(NULL);
        // Parent prints 11-20
        for (int i = 11; i <= 20; ++i) {
            printf("%d\n", i);
        }
        fflush(stdout);
    }
}