#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "delays.h"
#include "process.h"

int main() {
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		// Child process: exec ls -l
		execl("/bin/ls", "ls", "-l", (char *)NULL);
		perror("execl failed");
		exit(EXIT_FAILURE);
	} else {
		printf("========================================\n");
		fflush(stdout);
		wait(NULL); // Wait for child to finish
		printf("========================================\n");
		fflush(stdout);
	}
	return 0;
}
