#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
/* #include <getopt.h> */
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define   TESTNUM         (50)
int main(int argc, char* argv[]) {

    printf("==== number = [%d]\n", TESTNUM);

/*
   fprintf(stdout, "1111111111111111111\n");

#ifndef  MM
   fprintf(stdout, "2222222222222222222\n");
   fprintf(stdout, "3333333333333333333\n");
   fprintf(stdout, "4444444444444444444\n");
#endif

   char  testfile[] = "test12345567890.pcm";

   int filenameLength = strlen(testfile);

    unsigned char temp_filename[100] = {0,};
    unsigned char buffer_filename[100] = {0,};
    unsigned char buffer_resultfilename[100] = {0,};

    strcpy( temp_filename, testfile );

    printf(" ======  file name  [%s] \n", testfile);

    int temp_loop = 0;
    int temp_location = 0;

    while(1){
            							if(temp_loop == 100){
            								//printf("%s\n", temp_filename + temp_location + 1);
            								memcpy(buffer_filename, temp_filename + temp_location + 1, (filenameLength - temp_location - 1));
            								break;
            							}
            							if(temp_filename[temp_loop] == '/'){
            								temp_location = temp_loop;
            							}

            							temp_loop++;
            						}

            						memcpy( buffer_resultfilename, 										temp_filename, 	  temp_location+1);
            						memcpy( buffer_resultfilename + temp_location +1, "b_", 						2);
            						memcpy( buffer_resultfilename + temp_location +3, buffer_filename,  (filenameLength - temp_location - 1));


     printf(" ======  file name  [%s] \n", buffer_resultfilename);
*/




// test test

    return 0;
}
