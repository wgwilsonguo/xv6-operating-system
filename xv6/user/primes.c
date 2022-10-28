#include "../kernel/types.h"
#include "user.h"
#include "../kernel/fs.h"

/*
Write a concurrent version of prime sieve using pipes. This idea is due to Doug McIlroy, inventor of Unix pipes. 
The picture halfway down this page and the surrounding text explain how to do it. Your solution should be in the file user/primes.c.
*/


/*
Psuedocode: 
1. Call a generator function that writes the nautral nubmers into a pipe:
    generator(upper_limit):
        Create a pipe to write int oand read from
        Fork the process
        Child process will return the pipe to read from
        Parent process will write into the pipe
2. Read/Take the first number of the pipe to be the first prime number
3. Pass that prime into a filter function
    filter(pipe, prime)
        read from the pipe number by number
        if that read number canot be divided by the current prime, write it into a new pipe
        child process returns the pipe number
        parent process does the filtering and writing into the return pipe
4. Repeat steps 2 and 3 until all the write pipes are closed        


*/

int generate_natural_nums(int upper_limit){
    int nat_nums_pipe[2];
    pipe(nat_nums_pipe);
    if(!fork()){
        // wait(0);
        // CHILD process:
        close(nat_nums_pipe[0]); // child is not reading anything
        for(int num = 2; num <= upper_limit; num++){
            write(nat_nums_pipe[1], &num, sizeof(int));
        }
        close(nat_nums_pipe[1]);
        exit(0);
    }
    else{
        // PARENT process:
        close(nat_nums_pipe[1]);
        return nat_nums_pipe[0];
    }
    exit(0);
}

int primes_filter(int read_pipe, int current_prime){
    int out_pipe[2];
    pipe(out_pipe);
    int candidate_prime;

    if (!fork()){
        // wait(0);
        close(out_pipe[0]);
        while(read(read_pipe, &candidate_prime, sizeof(int))){
            if(candidate_prime % current_prime){
                // there is a non-zero remainder:
                write(out_pipe[1], &candidate_prime, sizeof(int) );
            }
        }
        close(read_pipe);// close the read_pipe from the previous sieve
        close(out_pipe[1]);
        exit(0);
    }
    else{
        close(out_pipe[1]);
        return out_pipe[0];
    }
    exit(0);
}

int main(int argc, char *argv[]){
    int upper_limit = 35;
    int read_pipe = generate_natural_nums(upper_limit);
    int current_prime;

    while(read(read_pipe, &current_prime, sizeof(int))){
        printf("prime %d\n", current_prime);
        read_pipe = primes_filter(read_pipe, current_prime);
    }
    close(read_pipe);
    exit(0);
}