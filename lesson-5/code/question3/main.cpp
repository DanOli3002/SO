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
#include <pthread.h>


int shared;
pthread_mutex_t mutex;
pthread_cond_t cond;
int turn = 0; // 0: even thread's turn, 1: odd thread's turn

void *threadChildEven(void *arg)
{
    pid_t tid = gettid();
    while (1) {
        pthread_mutex_lock(&mutex);
        while (turn != 0 && shared > 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        if (shared <= 1) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        shared--;
        printf("Even thread (PID %d) decremented: %d\n", tid, shared);
        turn = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *threadChildOdd(void *arg)
{
    pid_t tid = gettid();
    while (1) {
        pthread_mutex_lock(&mutex);
        while (turn != 1 && shared > 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        if (shared <= 1) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        shared--;
        printf("Odd thread (PID %d) decremented: %d\n", tid, shared);
        turn = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int N1;
    do {
        printf("Insert a number from 10 to 20: ");
        scanf("%d", &N1);
        if (N1 < 10 || N1 > 20) {
            printf("Error: number out of range\n");
        }
    } while (N1 < 10 || N1 > 20);

    shared = N1;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    turn = (shared % 2 == 0) ? 0 : 1; // Even or odd starts

    pthread_t thr_childEven, thr_childOdd;
    pthread_create(&thr_childEven, NULL, threadChildEven, NULL);
    pthread_create(&thr_childOdd, NULL, threadChildOdd, NULL);

    pthread_join(thr_childEven, NULL);
    pthread_join(thr_childOdd, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    printf("Main thread finished.\n");
    return 0;
}

