// Simple shell 0.1 + :
//    Handle command lines with arguments
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_COMMAND_LENGTH / 2 + 1];
    int should_run = 1;

    while (should_run) {
        printf("#cisfun$ ");
        fflush(stdout);

        // read command from user
        fgets(command, MAX_COMMAND_LENGTH, stdin);

        // tokenize command into arguments
        int num_args = 0;
        char *token = strtok(command, " \n");
        while (token != NULL) {
            args[num_args++] = token;
            token = strtok(NULL, " \n");
        }
        args[num_args] = NULL;

        // fork child process to execute command
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // child process
            if (execvp(args[0], args) == -1) {
                perror(args[0]);
                exit(EXIT_FAILURE);
            }
        } else {
            // parent process
            wait(NULL);
        }
    }

    return 0;
}

