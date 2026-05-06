#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

#define FIFO_FILE "/tmp/myfifo"

int main() {
    int fd;
    char buffer[BUFSIZ];
    pid_t pid;

    // Create FIFO
    mkfifo(FIFO_FILE, 0666);

    pid = fork();

    if(pid > 0) {
        /*********** PRODUCER (PARENT) ***********/
        fd = open(FIFO_FILE, O_WRONLY);
        if(fd == -1) {
            perror("open write");
            exit(EXIT_FAILURE);
        }

        while(1) {
            printf("Producer: Enter message (type exit to stop): ");
            fgets(buffer, BUFSIZ, stdin);

            write(fd, buffer, strlen(buffer));

            if(strncmp(buffer, "exit", 4) == 0)
                break;
        }

        close(fd);
    }

    else if(pid == 0) {
        /*********** CONSUMER (CHILD) ***********/
        fd = open(FIFO_FILE, O_RDONLY);
        if(fd == -1) {
            perror("open read");
            exit(EXIT_FAILURE);
        }

        while(1) {
            memset(buffer, 0, BUFSIZ);

            read(fd, buffer, BUFSIZ);

            printf("Consumer received: %s", buffer);

            if(strncmp(buffer, "exit", 4) == 0)
                break;
        }

        close(fd);
        unlink(FIFO_FILE);
    }

    else {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}