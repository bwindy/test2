#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/***********************************************************
 * 본 프로그램은 반복되는 이진수 갯수 체크 프로그램입니다.
 * Input(이진문자열)  :  01100010010001111011
 * Output(출력문자열) :  1231213412
 ***********************************************************/

int  strtest(int s, char * strBuf); 
 
int main()
{

	char  chBuf[1024] = "111";
	
	strtest( 1, chBuf);
	
    printf("Result [%s]\n", chBuf);
	
	return 0;
	
}


int  strtest(int s, char * strBuf)
{
	char  chAdd[10] = "test";
	
	strcat(strBuf, chAdd);
	
	printf("== [%s]\n", strBuf);
	
	
	return 1;
}


