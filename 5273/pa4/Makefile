CFLAGS = -Wall -g


SRCS = client.c server.c
LOGS = client-log.txt server-log.txt
CLIENT = clientpa4/source/client
SERVER = serverpa4/source/server
CLIENT_SRC = clientpa4/source/client.c
SERVER_SRC = serverpa4/source/server.c
CLIENT_OBJ = clientpa4/source/client.o
SERVER_OBJ = serverpa4/source/server.o

MOVE_FILES = ${HELPER} ${SRCS} Makefile client1/
TRANSFER = scp
SSH = ssh
SCPFLAGS = -C -r #Compresses data and do folders
SSHFLAGS = -C #Compress data
REMOTE = anga2518@elra-03.cs.colorado.edu:/home/guests/anga2518/Documents/PA4/
ELRA = anga2518@elra-03.cs.colorado.edu



#UNAME := $(shell uname)

all: $(CLIENT) $(SERVER)

move: ${MOVE_FILES}
	${TRANSFER} -r ${MOVE_FILES} ${REMOTE}

compile: 
	${SSH} ${SSHFLAGS} ${ELRA} "cd Documents/PA4 && make client-exes"

$(CLIENT): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(CLIENT_OBJ) -o $(CLIENT)

$(CLIENT_OBJ): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -c $(CLIENT_SRC) -o $(CLIENT_OBJ)

$(SERVER): $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -o $(SERVER)

$(SERVER_OBJ): $(SERVER_SRC)
	$(CC) $(CFLAGS) -c $(SERVER_SRC) -o ${SERVER_OBJ}



#${OBJS}: ${HELPER}

clean:
	rm *.o ${CLIENT} ${SERVER} clientpa4/Downloaded_Files/*
