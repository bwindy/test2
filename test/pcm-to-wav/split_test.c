#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
/* #include <getopt.h> */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wave.h"

int strsplit(char *chString,char *chDelimit,char *chResult[]);
void test(char *str);


#define  END_WAV   ".wav"

char* dummy_get_raw_pcm (char *p, int *bytes_read);
void get_wav_header(int raw_sz, wav_header_t *wh);
void dump_wav_header (wav_header_t *wh);
int  pcmtowav(char* argv1, char* argv2);

int main(int argc, char* argv[]) {

//    char substr[500] = "-pcm=../data/wakeup.pcm  -model=../model/wakeup_ja.fuet -record";

    char substr[500] = "sample/sin.pcm";
    char *result[20];
    char *result2[20];
    int strnum = 0;

    char  firstFNpcm[30];
    char  firstFNwav[30];
    char  secondFNpcm[30];
    char  secondFNwav[30];

    strcpy( firstFNpcm, argv[1]);
//    strnum = strsplit(argv[1],".",result);
    printf("1. [%s]\n", firstFNpcm);
    strncpy( firstFNwav, firstFNpcm, strlen(firstFNpcm)-4);
    printf("===========================\n");
    printf("1. [%s]\n", firstFNwav);
//    printf("2. [%s]\n", result[1]);

    strcpy( secondFNpcm, argv[2]);

//    strnum = strsplit(argv[2],".",result2);
    strncpy( secondFNwav, secondFNpcm, strlen(secondFNpcm)-4);
    printf("===========================\n");
    printf("1. [%s]\n", secondFNwav);
//    printf("2. [%s]\n", result2[1]);

    char  resultfile[100];
    char  resultfile2[100];

    memset(resultfile, 0x00, 100);
    memset(resultfile2, 0x00, 100);

    strcat(resultfile, firstFNwav);
    strcat(resultfile, END_WAV);

    printf("== result = [%s] \n", resultfile);

    strcat(resultfile2, secondFNwav);
    strcat(resultfile2, END_WAV);

    printf("== result = [%s] \n", resultfile2);

    pcmtowav(firstFNpcm, resultfile);
    pcmtowav(secondFNpcm, resultfile2);
//    test(substr);

    return 0;
}


void testtest( char *str )
{
    char *result[20];
    int strnum = 0;

    strnum = strsplit(str," ",result);
    printf("===========================\n");
    printf("1. [%s]\n", result[0]);
    printf("2. [%s]\n", result[1]);
    printf("3. [%s]\n", result[2]);
}

int strsplit(char *chString,char *chDelimit,char *chResult[])
{
        int i;
        char *chTok;

        i = 0;
        chTok = strtok(chString,chDelimit);
        chResult[i]=chTok;

        while( (chTok = strtok(NULL, chDelimit)) != NULL)
        {
                chResult[++i]=chTok;
        }

        return (i+1);
}

char* dummy_get_raw_pcm (char *p, int *bytes_read)
{
	long lSize;
	char *pcm_buf;
	size_t result;
	FILE *fp_pcm;

	fp_pcm = fopen (p, "rb");
	if (fp_pcm == NULL) {
		printf ("File error");
		exit (1);
	}

	// obtain file size:
	fseek (fp_pcm , 0 , SEEK_END);
	lSize = ftell (fp_pcm);
	rewind (fp_pcm);

	// allocate memory to contain the whole file:
	pcm_buf = (char*) malloc (sizeof(char) * lSize);
	if (pcm_buf == NULL) {
		printf ("Memory error");
		exit (2);
	}

	// copy the file into the pcm_buf:
	result = fread (pcm_buf, 1, lSize, fp_pcm);
	if (result != lSize) {
		printf ("Reading error");
		exit (3);
	}

	*bytes_read = (int) lSize;
	return pcm_buf;
}

void get_wav_header(int raw_sz, wav_header_t *wh)
{
	// RIFF chunk
	strcpy(wh->chunk_id, "RIFF");
	wh->chunk_size = 36 + raw_sz;

	// fmt sub-chunk (to be optimized)
	strncpy(wh->sub_chunk1_id, "WAVEfmt ", strlen("WAVEfmt "));
	wh->sub_chunk1_size = 16;
	wh->audio_format = 1;
	wh->num_channels = 1;
	wh->sample_rate = 16000;
	wh->bits_per_sample = 16;
	wh->block_align = wh->num_channels * wh->bits_per_sample / 8;
	wh->byte_rate = wh->sample_rate * wh->num_channels * wh->bits_per_sample / 8;

	// data sub-chunk
	strncpy(wh->sub_chunk2_id, "data", strlen("data"));
	wh->sub_chunk2_size = raw_sz;
}

void dump_wav_header (wav_header_t *wh)
{
	printf ("=========================================\n");
	printf ("chunk_id:\t\t\t%s\n", wh->chunk_id);
	printf ("chunk_size:\t\t\t%d\n", wh->chunk_size);
	printf ("sub_chunk1_id:\t\t\t%s\n", wh->sub_chunk1_id);
	printf ("sub_chunk1_size:\t\t%d\n", wh->sub_chunk1_size);
	printf ("audio_format:\t\t\t%d\n", wh->audio_format);
	printf ("num_channels:\t\t\t%d\n", wh->num_channels);
	printf ("sample_rate:\t\t\t%d\n", wh->sample_rate);
	printf ("bits_per_sample:\t\t%d\n", wh->bits_per_sample);
	printf ("block_align:\t\t\t%d\n", wh->block_align);
	printf ("byte_rate:\t\t\t%d\n", wh->byte_rate);
	printf ("sub_chunk2_id:\t\t\t%s\n", wh->sub_chunk2_id);
	printf ("sub_chunk2_size:\t\t%d\n", wh->sub_chunk2_size);
	printf ("=========================================\n");
}

int  pcmtowav(char* argv1, char* argv2)
{

printf("+++++++++++++++++++++++++++++++++++\n");
printf(" == pcm filename = [%s]\n", argv1);
printf(" == wav filename = [%s]\n", argv2);
	int raw_sz = 0;
	FILE *fwav;
	wav_header_t wheader;

	memset (&wheader, '\0', sizeof (wav_header_t));

	// dummy raw pcm data
	char *pcm_buf = dummy_get_raw_pcm (argv1, &raw_sz);

	// construct wav header
	get_wav_header (raw_sz, &wheader);
	dump_wav_header (&wheader);

	// write out the .wav file
	fwav = fopen(argv2, "wb");
	fwrite(&wheader, 1, sizeof(wheader), fwav);
	fwrite(pcm_buf, 1, raw_sz, fwav);
	fclose(fwav);

	if (pcm_buf)
		free (pcm_buf);

	return 0;
}
