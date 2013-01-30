CFLAGS = -Wall -g -pthread
SSLFLAGS = -lssl -lcrypto

SRCS = client.c server.c
LOGS = client-log.txt server-log.txt
CLIENT1 = client1/client
CLIENT2 = client2/client
SERVER = serverpa4/server
CLIENT_SRC = client.c
SERVER_SRC = server.c
CLIENT_OBJ = client.o
SERVER_OBJ = server.o

MOVE_FILES = ${HELPER} ${SRCS} Makefile client1/
TRANSFER = scp
SSH = ssh
SCPFLAGS = -C -r #Compresses data and do folders
SSHFLAGS = -C #Compress data
REMOTE = anga2518@elra-03.cs.colorado.edu:/home/guests/anga2518/Documents/PA4/
ELRA = anga2518@elra-03.cs.colorado.edu

#UNAME := $(shell uname)

all: $(CLIENT1) $(CLIENT2) $(SERVER)

move: ${MOVE_FILES}
	${TRANSFER} -r ${MOVE_FILES} ${REMOTE}

compile: 
	${SSH} ${SSHFLAGS} ${ELRA} "cd Documents/PA4 && make client-exes"

client1/client: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(SSLFLAGS) $(CLIENT_OBJ) -o $(CLIENT1)
	
client2/client: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(SSLFLAGS) $(CLIENT_OBJ) -o $(CLIENT2)

$(CLIENT_OBJ): $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(SSLFLAGS) -c $(CLIENT_SRC) -o $(CLIENT_OBJ)

$(SERVER): $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(SSLFLAGS) $(SERVER_OBJ) -o $(SERVER)

$(SERVER_OBJ): $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SSLFLAGS) -c $(SERVER_SRC) -o ${SERVER_OBJ}



#${OBJS}: ${HELPER}

clean:
	rm *.o $(CLIENT1) $(CLIENT2) $(SERVER)
