CC     = gcc
CFLAGS = -I. -g 
#LFLAGS = -lnsl -lsocket

OBJS1 = server.o community.o
OBJS2 = client.o community.o

TARGET1 = server 
TARGET2 = client 

all: server client

$(TARGET1): $(OBJS1)
	$(CC) -o $(TARGET1) $(OBJS1) $(LFLAGS)

$(TARGET2): $(OBJS2)
	$(CC) -o $(TARGET2) $(OBJS2) $(LFLAGS)


.SUFFIXES: .c .o

.c.o: 
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o
