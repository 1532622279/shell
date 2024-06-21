 // execute.cpp
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <cstring> // For strerror

int execute(char *argv[]) {
    pid_t pid;
    int child_info = -1;

    if (argv[0] == NULL) /* nothing succeeds */
        return 0;

    if ((pid = fork()) == -1) {
        std::perror("fork");
    } else if (pid == 0) {
        // Child process
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        if (execvp(argv[0], argv) == -1) {
            std::perror("cannot execute command");
            std::exit(1);
        }
    } else {
        // Parent process
        if (wait(&child_info) == -1) {
            std::perror("wait");
        }
    }
    return child_info;
}
