// Write a UNIX command line interpreter.
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE 80

int main(void) {
    char command[MAX_LINE];
    pid_t pid;

    while (1) {
        printf("#cisfun$ ");
        fflush(stdout);

        // Read command from user
        if (fgets(command, MAX_LINE, stdin) == NULL) {
            continue;
        }

        // Remove newline character from the end of command
        command[strcspn(command, "\n")] = '\0';

        // Fork process
        pid = fork();

        if (pid < 0) { // Error occurred
            perror("fork");
            continue;
        } else if (pid == 0) { // Child process
            char *argv[] = { command, NULL };

            // Execute command
            if (execve(command, argv, NULL) < 0) {
                printf("%s: command not found\n", command);
                exit(1);
            }
        } else { // Parent process
            // Wait for child process to complete
            waitpid(pid, NULL, 0);
        }
    }

    return 0;
}

