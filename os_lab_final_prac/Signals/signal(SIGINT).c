#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Signal handler function
void sigint_handler(int signum) {
    fprintf(stdout, "Caught SIGINT signal (%d)\n", signum);
}

int main() {
    // Registering signal handler for SIGINT
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        printf("Error setting up signal handler for SIGINT\n");
        return 1;
    }

    printf("Press Ctrl+C to send a SIGINT signal\n");

    // Infinite loop to keep the program running
    while(1) {
        sleep(1);
    }

    return 0;
}