# Server Pro*C makefile

include $(ORACLE_HOME)/precomp/lib/env_precomp.mk
#include $(ORACLE_HOME)/precomp/admin/pcscfg.cfg 



#LIBS += -L/u01/app/oracle/product/12.1.0/dbhome_1/lib -l clntsh





ORALIBDIR = $(LIBHOME)
ORALIB = $(PROLDLIBS)

PCTARGET= insert
APOBJS= $(PCTARGET).o

OBJS = $(APOBJS)

OBJS1 = server.o 	community.o 	insert.o

OBJS2 = client.o   	community.o 

TARGET1 = server 
TARGET2 = client 


CC     = gcc
CFLAGS = -I. -O -g -D_DBG

#
.SUFFIXES : .c .o

.c.o: 
	$(CC) $(CFLAGS) -c $<

#
# server compile
#
all: server client

$(APOBJS): $(PCTARGET).pc
	proc iname=$(PCTARGET) #include=$(INCDIR)
	$(CC) $(CFLAGS) -c $(PCTARGET).c
	
$(TARGET1): $(OBJS1)
	$(CC) $(CFLAGS) -o $(TARGET1) -L$(ORALIBDIR) $(ORALIB) $(OBJS1) $(LFLAGS)

$(TARGET2): $(OBJS2)
	$(CC) -o $(TARGET2) $(OBJS2) $(LFLAGS)




clean:
	rm -f *.o core  $(PCTARGET) $(APPDIR)/$(TARGET) $(PCTARGET).c $(PCTARGET).lis




	



