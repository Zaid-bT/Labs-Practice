// /How a parent process handles child termination using signals

#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>

void proc_exit() {
    int wstat;
    pid_t pid;

    while (1) {
        // Get info about child process.
        // WNOHANG returns immediately if there is no child to wait
        pid = wait3(&wstat, WNOHANG, NULL);
        if (pid == 0 || pid == -1) {
            fprintf(stdout, "return value of wait3() is %d\n", pid);
            return;
        }
        fprintf(stdout, "Return code: %d\n", wstat);
    }
}

int main() {
    signal(SIGCHLD, proc_exit);
    switch (fork()) {
    case -1:
        perror("main: fork");
        exit(0);
    case 0:
        printf("I'm alive (temporarily)\n"); // chaid only executes this and exits
        int ret_code = rand();
        printf("Return code is %d\n", ret_code);
        exit(ret_code);
    default:
        pause(); // suspends main process execution untill a signal arrives
    }
    exit (0);
}