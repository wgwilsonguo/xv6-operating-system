#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "../kernel/fs.h"


int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(2, "Incorrect number of arguments to sleep");
        exit(1);
    }
    // int sleep(int);
    sleep((atoi(argv[1])));

    exit(0);
}