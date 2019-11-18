CC     = gcc
CFLAGS = -I. -g 
#LFLAGS = -lnsl -lsocket

OBJS1 = server.o community.o
#OBJS2 = insert_client.o community.o
OBJS3 = test.o   community.o

TARGET1 = server 
#TARGET2 = client 
TARGET3 = test 

#all: server insert_client test
all: server test

$(TARGET1): $(OBJS1)
	$(CC) -g -o $(TARGET1) $(OBJS1) $(LFLAGS)

#$(TARGET2): $(OBJS2)
#	$(CC) -g -o $(TARGET2) $(OBJS2) $(LFLAGS)

$(TARGET3): $(OBJS3)
	$(CC) -g -o $(TARGET3) $(OBJS3) $(LFLAGS)
	
.SUFFIXES: .c .o

.c.o: 
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o
