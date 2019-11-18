#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
/* #include <getopt.h> */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define FILE_CHANGE_FILE  "result.pcm"

static int file_change( const char *aName, const char *aResult );

int main(int argc, char* argv[]) {

    if( argc != 3 ) {
      printf("===========================================\n");
      printf("\n");
      printf("./t1  sample.pcm  result.pcm\n");
      printf("\n");
      printf("===========================================\n");
    } else if (argc == 3){

        int  ret =  file_change( argv[1], argv[2] );

        if( ret == 0 ) {
            printf("+++ File Change Success !! +++\n");

        } else {
             printf("+++ File Change Fail !! +++\n");
        }
    }


    return 0;

}


static int file_change( const char *aName,  const char *aResult)
{
	int ret = -1;
	FILE *fp;
	long fileSize;

	// File open
	fp = fopen( aName, "rb" );
	if( fp == NULL ){
		return ret;
	}

	// File size acquisition
	fseek( fp, 0, SEEK_END );
	fileSize = ftell( fp );
    fseek( fp, 0, SEEK_SET );

	// malloc
	if( fileSize > 0 ){
		unsigned char *readData;
		unsigned char swap_buffer = 0x00;

		readData = (unsigned char *)malloc( fileSize );
		memset(readData, 0, fileSize);

		if( readData != NULL ){
			size_t readSize;

			// read
			readSize = fread( readData, 1, fileSize, fp);

			if( readSize == fileSize ){

				for(int i=0; i <= readSize; )
				{
					swap_buffer = readData[i];
					readData[i] = readData[i+1];
					readData[i+1] = swap_buffer;
					i = i+2;
				}

				FILE * fp2 = fopen(aResult, "w+");
				fwrite((void*)readData,1,readSize, fp2);

				free(readData);
				int state = fclose(fp2);

				if( state == 0 )  /* File Close Check */
				{
					printf("\n------------   File Write success!! \n\n");
                    ret = 0;
				}
				else
				{
					printf("\n------------   File Write Fail!!\n");
				    ret = -1;
				}


			}else{
				free( readData );
			}
		}
	}

	fclose( fp );

	return ret;
}
