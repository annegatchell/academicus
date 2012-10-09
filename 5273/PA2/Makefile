CC = gcc
CFLAGS = -I.
OBJSS = helper.o SRPserver.o
OBJSC = helper.o SRPclient.o

all: server/SRPserver client/SRPclient

server/SRPserver: ${OBJSS}
	${CC} -o server/SRPserver ${CFLAGS} ${OBJSS}
#	gcc -o server/SRPserver -Wall server/SRPserver.c helper.c -I.

client/SRPclient: ${OBJSC}
	${CC} -o client/SRPclient ${CFLAGS} ${OBJSC}

helper.o: helper.c helper.h sendto_.h
	${CC} ${CFLAGS} -c helper.c

SRPclient.o: client/SRPclient.c helper.h sendto_.h
	${CC} ${CFLAGS} -c client/SRPclient.c

SRPserver.o: server/SRPserver.c helper.h sendto_.h
	${CC} ${CFLAGS} -c server/SRPserver.c
	
#	gcc -o client/SRPclient -Wall client/SRPclient.c helper.c -I.

server/SRPserver.c client/SRPclient.c helper.c: sendto_.h helper.h

clean:
	rm -f *.o server/SRPserver client/SRPclient ${OBJS}