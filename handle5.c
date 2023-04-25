// Simple shell 0.4 +
//    Implement the env built-in, that prints the current environment

#include <stdio.h>
#include <stdlib.h>

extern char **environ;

/**
 * main - Simple shell
 *
 * Return: Always 0.
 */
int main(void)
{
        char *line = NULL;
        size_t len = 0;
        ssize_t nread;
        char *argv[2] = {"/usr/bin/env", NULL};

        do {
                printf("$ ");
                nread = getline(&line, &len, stdin);
                if (nread == -1)
                        break;
                if (strcmp(line, "env\n") == 0) {
                        int i;

                        for (i = 0; environ[i] != NULL; i++)
                                printf("%s\n", environ[i]);
                } else if (strcmp(line, "exit\n") == 0) {
                        break;
                } else {
                        pid_t pid;
                        int status;

                        pid = fork();
                        if (pid == -1) {
                                perror("fork");
                        } else if (pid == 0) {
                                if (execve(line, argv, environ) == -1) {
                                        perror("execve");
                                }
                                exit(EXIT_FAILURE);
                        } else {
                                wait(&status);
                        }
                }
        } while (1);

        free(line);

        return (0);
}

