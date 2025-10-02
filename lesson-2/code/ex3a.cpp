#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return 1;
    }
    pid_t pid = (pid_t)atoi(argv[1]);
    if (kill(pid, SIGINT) == -1) {
        perror("kill failed");
        return 1;
    }
    printf("SIGINT sent to process %d\n", pid);
    return 0;
}
