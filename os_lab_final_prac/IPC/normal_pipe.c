#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define buffer 25
#define read_end 0
#define write_end 1

int main(void){
    char write_msg[buffer]="GREETINGS";
    char read_msg[buffer];

    int fd[2];
    pid_t pid; //process

    if(pipe(fd) == -1){//validate pipe
        fprintf(stderr,"PIPE FAILED");
        return 1;
    }
    pid = fork();
    if(pid < 0){ //error =-1
        fprintf(stderr,"FORK FAILED");
    }
    if(pid > 0){//parent process = 1
        close(fd[read_end]);
        write(fd[write_end],write_msg,strlen(write_msg)+1);
      //write(where to write, data to send, number of bytes);
        close(fd[write_end]);
    }
    if(pid == 0){//child process = 0
        close(fd[write_end]);
        read(fd[read_end],read_msg,buffer);
        //write(where to read from, where data will be stored, max bytes to read);
        printf("read %s",read_msg);
        close(fd[read_end]);
    }
    return 0;
}