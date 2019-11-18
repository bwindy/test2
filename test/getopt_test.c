#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
/* #include <getopt.h> */
#include <unistd.h>
#include <stdio.h>



int main(int argc, char* argv[]) {
    
    int opt = -1;
 
 
     while ((opt = getopt(argc, argv, "f:l:s:")) != -1) {
            switch(opt) {
             case 'f':
                printf("==== [%s]====\n",  optarg);
            case 'l':
                printf("==== [%s]====\n",  optarg);
            case 's':
                printf("==== [%s]====\n",  optarg);
                break;
            default:
                break;
            }
    }
       
    return 0;
}

/*
./test -f123 -l123 -s1234567
==== [123]====
==== [123]====
==== [123]====
==== [123]====
==== [123]====
==== [1234567]====


./test -s1234
==== [1234]====

*/