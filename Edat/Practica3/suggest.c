#include <stdio.h>
#include "table.h"

int main(int argc, char **argv){
    int score = 0;
    if (argc<2){
        printf("Not enought parameters: %d", argc);
        return -1;
    }
    
    score = atoi (argv[1]);
    
    return 0;
}