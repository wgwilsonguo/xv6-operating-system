#include "../kernel/types.h"
#include "user.h"
#include "../kernel/fs.h"

/*

Write a program that uses UNIX system calls to ''ping-pong'' a byte between two processes over a pair of pipes, 
one for each direction. The parent should send a byte to the child; the child should print "<pid>: received ping", 
where <pid> is its process ID, write the byte on the pipe to the parent, and exit; the parent should read the byte 
from the child, print "<pid>: received pong", and exit. Your solution should be in the file user/pingpong.c.

*/ 


int main(int argc, char *argv[]){
    int ping_pong_pipe[2];
    pipe(ping_pong_pipe);

    if(!fork()){
        // child:
        char buff[128];

        if(read(ping_pong_pipe[0], buff+strlen(buff), sizeof(buff)) != 0){
            if (strcmp(buff, "ping") != 0){
                printf("Incorrect string\n");
                exit(1);
            }

            else{
                write(ping_pong_pipe[1], "pong\0", 5);
                // write(ping_pong_pipe[1], "AFTER THE TERMINATOR", 21);
                printf("%d: received ping\n", getpid());
            }
        }
        // close(ping_pong_pipe[0]);
        close(ping_pong_pipe[1]);
    }

    else{
        // parent:
        char buf[128];// = "ABCDEFG";
        write(ping_pong_pipe[1], "ping\0", 5);
        wait(0);
        close(ping_pong_pipe[1]);

        if(read(ping_pong_pipe[0], buf, 5) != 0){
            if(!strcmp("pong\0", buf)){
                // printf("This is buff: %s\n", buf);
                printf("%d: received pong\n", getpid());
                // printf("\n");
            }
        }
        // close(ping_pong_pipe[0]);
    }
    exit(0);
}
