
CC				= gcc
CXX				= g++
CPP				= g++
AR				= ar

#PROC=$(ORACLE_HOME)/bin/proc
PROFLAGS=ireclen=132 oreclen=132 select error=no sqlcheck=full userid=$(USERID)
dbms=v8



WARN			= -Wall
DEBUG			= -g 
#DEBUG			= -g -DDEBUG
DEBUG			= -g -DPDKDEBUG
DEFLIST			= -DUNIX -DUNIX_SVR4 -D_REENTRANT -DNEW_VERSION #-D__FAST_MODE__
CFLAGS			= $(WARN) $(DEBUG) $(DEFLIST)

COMMONLIB		= -lm -ldl -lresolv -lsocket -lnsl -lrt
THREADLIB		= -lposix4 -lpthread

#oracle
ORALIBDIR		= $(ORACLE_HOME)/lib
ORALIBS			= -L$(ORALIBDIR) $(PROLDLIBS)
ORAINCDIR		= $(ORACLE_HOME)/precomp/public
ORAINCS			= -I$(ORAINCDIR)

LIBS			= $(COMMONLIB) $(THREADLIB) $(BASELIBS)
INCS         	= $(HSINCS) $(BASEINCS)

include $(ORACLE_HOME)/precomp/lib/env_precomp.mk


PROCPPFLAGS=code=c cpp_suffix=c



.SUFFIXES: .c .cpp .o  .pc
.pc.c:
	@echo "1 ------------------------------------"
	$(PROC) $(PROCPPFLAGS) iname=$*.pc

.pc.o:
	@echo "2 ------------------------------------"
	$(PROC) $(PROCPPFLAGS)  iname=$*.pc
	$(CC) -c $*.c

.c.o:
	@echo "3 ------------------------------------"
	$(CC) -c $*.c 



SRVOBJS = test.c
#SRVOBJS = main.o rgserver.o rgbase.o soapC.o soapServer.o

TARGET = TESTPROC

all: $(TARGET)

	
TESTPROC: $(SRVOBJS)
		$(CC)  test.c  -o TESTPROC 

copy: 
	-/bin/mv -f $(PROJINSTDIR)/bin/$(TARGET) $(PROJINSTDIR)/bin/$(TARGET).old
	cp $(TARGET) $(PROJINSTDIR)/bin

clean:
	rm -f *.c *.o *.lis


