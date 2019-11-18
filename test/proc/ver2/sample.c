#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/***********************************************************
 * 본 프로그램은 반복되는 이진수 갯수 체크 프로그램입니다.
 * Input(이진문자열)  :  01100010010001111011
 * Output(출력문자열) :  1 2  31 21  3   41 2
 ***********************************************************/

void main()
{
  int nCount0 = 0; /* 반복되는 0 문자열 갯수 */
  int nCount1 = 0; /* 반복되는 1 문자열 갯수 */
  int i = 0;       /* for loop 변수          */
  int nlen = 0;    /* 이진수문자열 길이 변수 */
  
  char  chBuffer[1024] = "01100010010001111011"; /* 이진 문자열 변수 */
  char  sPrint  [1024];  /* 출력 문자열 저장 변수       */
  char  cCount[2];       /* 반복되는 이진문자열 갯수    */   
  char  chBuf [2];       /* 이진문자열 저장 임시 문자열 */ 
  
  nlen = strlen(chBuffer);  /* 이진문자열 Size */
  memset( sPrint, 0x00, sizeof(sPrint)); /* 출력 문자열 초기화 */
  
  for( i ; i < nlen; i++)
  {
    memset(cCount, 0x00, sizeof(cCount)); /* 반복되는 이진문자열 초기화  */ 
	memset(chBuf,  0x00, sizeof(chBuf));  /* 이진문자열 저장 임시 문자열 초기화 */
	
	memcpy(chBuf, &chBuffer[i], 2);       /* 이진문자열 임시 저장 */
 
	if( strncmp( chBuf,"0", 1) == 0 )  /* 변수 값이 0 이면 */
	{
	   nCount0++;  /* 이진 문자열이 0 이면 반복횟수 증가시킨다 */ 

	    if( i != 0 ) /* 처음부터 반복되는 이진문자열 횟수를 저장하지 않는다. */ 
        {
            if( nCount1 != 0 )
            {             
               sprintf( cCount, "%d", nCount1);  /* 1 로 입력된 이진문자열 반복횟수를 저장한다. */
               strcat( sPrint, cCount); 
            }
             
            if( nCount1 != 0 )  /* 1 로 입력된 이진문자열 반복횟수를 초기화한다. */          
               nCount1 = 0;  
        }
           
        if( i == nlen -1)   /* 0 으로 들어온 마지막 값이 있으면 반복횟수를 저장한다 */
        {
            sprintf( cCount, "%d", nCount0);
            strcat( sPrint, cCount);
        } 
	}
	else if(strncmp( chBuf, "1", 1) == 0 ) /* 변수 값이 1 이면 */
	{
        nCount1++; /* 이진 문자열이 1 이면 반복횟수 증가시킨다 */ 
	   
        if( i != 0 ) /* 처음부터 반복되는 이진문자열 횟수를 저장하지 않는다. */ 
        {
            if( nCount0 != 0 ) /* 0 으로 입력된 이진문자열 반복횟수를 저장한다. */
            {      
               sprintf( cCount, "%d", nCount0);
               strcat( sPrint, cCount);
	        }
 
            if( nCount0 != 0 ) /* 0 으로 입력된 이진문자열 반복횟수를 초기화한다. */ 
				nCount0 = 0;
        }
 
        if( i == nlen -1 ) /* 1로 들어온 마지막 값이 있으면 반복횟수를 저장한다 */
        {
            sprintf( cCount, "%d", nCount1);
            strcat( sPrint, cCount);
        }
	 }
  
  }
  
  printf("%s\n", sPrint); /* 출력문자열 화면 프린트 */
  
}

