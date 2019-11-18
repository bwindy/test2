#ifndef __INSERT_H__
#define __INSERT_H__

#include <stdio.h>
#include <string.h>


#define HANDPHONESIZE   11
#define NAMESIZE        20 + HANDPHONESIZE
#define CARDNUMBERSIZE  20 + NAMESIZE
					
					
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>


void  	sql_error(char * msg);					 
int 	insertCustomer(char *chPhone, char *chName, char *chCardNum, char *chCardInc);


#endif
