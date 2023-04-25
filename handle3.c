// Simple shell 0.2 + :
//    Handle the PATH
//    fork must not be called if the command doesn’t exist

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

        // search for command in PATH
        char *path = getenv("PATH");
        char *path_token = strtok(path, ":");
        char command_path[MAX_COMMAND_LENGTH];
        int found_command = 0;

        while (path_token != NULL) {
            snprintf(command_path, MAX_COMMAND_LENGTH, "%s/%s", path_token, args[0]);
            if (access(command_path, F_OK) != -1) {
                found_command = 1;
                break;
            }
            path_token = strtok(NULL, ":");
        }

        if (!found_command) {
            printf("%s: command not found\n", args[0]);
            continue;
        }

        // fork child process to execute command
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // child process
            if (execvp(command_path, args) == -1) {
                perror(command_path);
                exit(EXIT_FAILURE);
            }
        } else {
            // parent process
            wait(NULL);
        }
    }

    return 0;
}

