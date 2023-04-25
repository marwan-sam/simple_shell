// Simple shell 0.3 + :
//    Implement the exit built-in, that exits the shell
//    -[ Usage: exit ]-
//    You don’t have to handle any argument to the built-in exit

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10
#define MAX_PATH_LENGTH 100

char *path[MAX_PATH_LENGTH];
int num_paths = 0;

void read_paths_from_env() {
    char *env_path = getenv("PATH");
    char *path_str = strtok(env_path, ":");
    while (path_str != NULL) {
        path[num_paths++] = path_str;
        path_str = strtok(NULL, ":");
    }
}

int execute_command(char *command, char **args) {
    int i;
    char command_path[MAX_PATH_LENGTH];
    for (i = 0; i < num_paths; i++) {
        strcpy(command_path, path[i]);
        strcat(command_path, "/");
        strcat(command_path, command);
        if (access(command_path, X_OK) == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                execv(command_path, args);
                fprintf(stderr, "Error: Unable to execute command\n");
                exit(1);
            } else {
                wait(NULL);
                return 0;
            }
        }
    }
    fprintf(stderr, "Error: Command not found\n");
    return 1;
}

int parse_command(char *input, char **command, char ***args) {
    char *token = strtok(input, " \n");
    if (token == NULL) {
        return 1;
    }
    *command = token;
    *args = malloc(MAX_ARGUMENTS * sizeof(char *));
    int i = 0;
    while (token != NULL && i < MAX_ARGUMENTS) {
        (*args)[i++] = token;
        token = strtok(NULL, " \n");
    }
    (*args)[i] = NULL;
    return 0;
}

int main(int argc, char **argv) {
    read_paths_from_env();

    while (1) {
        printf("simple_shell$ ");
        char command_line[MAX_COMMAND_LENGTH];
        if (fgets(command_line, MAX_COMMAND_LENGTH, stdin) == NULL) {
            printf("\n");
            exit(0);
        }

        char *command;
        char **args;
        if (parse_command(command_line, &command, &args) == 1) {
            continue;
        }

        if (strcmp(command, "exit") == 0) {
            exit(0);
        }

        execute_command(command, args);
    }

    return 0;
}

