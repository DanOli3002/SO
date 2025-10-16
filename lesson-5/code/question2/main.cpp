/*
 * An implementation of the bounded-buffer problem
 *
 * NC producers and NC consumers communicate through a fifo.
 * The fifo has a fixed capacity.
 * NI items will be produced by the producers and consume by the consumers.
 * An item is composed of 2 equal integers, ranging from 1 to NI.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <libgen.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "utils.h"
#include "thread.h"

// return status
int status;
void *threadChild(void *arg)
{
    int *shared = (int *)arg;
    int N2;
    do {
        printf("Insert a number from 10 to 20: ");
        scanf("%d", &N2);
        if (N2 < 10 || N2 > 20) {
            printf("Error: number out of range\n");
        }
    } while (N2 < 10 || N2 > 20);

    while (*shared < N2) {
        printf("Child incremented: %d\n", *shared);
        (*shared)++;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int N1;
    do {
        printf("Insert a number from 1 to 9: ");
        scanf("%d", &N1);
        if (N1 < 1 || N1 > 9) {
            printf("Error: number out of range\n");
        }
    } while (N1 < 1 || N1 > 9);

    int *shared = (int *)malloc(sizeof(int));
    *shared = N1;

    pthread_t thr_child;
    thread_create(&thr_child, NULL, threadChild, shared);
    thread_join(thr_child, NULL);

    // After child thread, decrement shared to 1
    while (*shared >= 1) {
        printf("Main decremented: %d\n", *shared);
        (*shared)--;
        
    }

    free(shared);
    printf("Main thread finished.\n");
    return 0;
}

