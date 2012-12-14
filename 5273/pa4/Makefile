CFLAGS = -Wall -g -pthread
SSLFLAGS = -lssl -lcrypto

SRCS = client.c server.c
LOGS = client-log.txt server-log.txt
CLIENT1 = client1/client
CLIENT2 = client2/client
CLIENT3 = client3/client
CLIENT4 = client4/client
SERVER = serverpa4/server
CLIENT1_SRC = client1.c
CLIENT2_SRC = client2.c
CLIENT3_SRC = client3.c
CLIENT4_SRC = client4.c
SERVER_SRC = server.c
CLIENT1_OBJ = client1.o
CLIENT2_OBJ = client2.o
CLIENT3_OBJ = client3.o
CLIENT4_OBJ = client4.o
SERVER_OBJ = server.o

MOVE_FILES = ${HELPER} ${SRCS} Makefile client1/
TRANSFER = scp
SSH = ssh
SCPFLAGS = -C -r #Compresses data and do folders
SSHFLAGS = -C #Compress data
REMOTE = anga2518@elra-03.cs.colorado.edu:/home/guests/anga2518/Documents/PA4/
ELRA = anga2518@elra-03.cs.colorado.edu

#UNAME := $(shell uname)

all: $(CLIENT1) $(CLIENT2) $(CLIENT3) $(CLIENT4) $(SERVER)

move: ${MOVE_FILES}
	${TRANSFER} -r ${MOVE_FILES} ${REMOTE}

compile: 
	${SSH} ${SSHFLAGS} ${ELRA} "cd Documents/PA4 && make client-exes"

$(CLIENT1): $(CLIENT1_OBJ)
	$(CC) $(CFLAGS)  $(CLIENT1_OBJ) -o $(CLIENT1) $(SSLFLAGS)
	
$(CLIENT2): $(CLIENT2_OBJ)
	$(CC) $(CFLAGS) $(CLIENT2_OBJ) -o $(CLIENT2) $(SSLFLAGS)
	
$(CLIENT3): $(CLIENT3_OBJ)
	$(CC) $(CFLAGS)  $(CLIENT3_OBJ) -o $(CLIENT3) $(SSLFLAGS)
	
$(CLIENT4): $(CLIENT4_OBJ)
	$(CC) $(CFLAGS)  $(CLIENT4_OBJ) -o $(CLIENT4) $(SSLFLAGS)

$(CLIENT1_OBJ): $(CLIENT1_SRC)
	$(CC) $(CFLAGS) -c $(CLIENT1_SRC) -o $(CLIENT1_OBJ) $(SSLFLAGS)
	
$(CLIENT2_OBJ): $(CLIENT2_SRC)
	$(CC) $(CFLAGS) -c $(CLIENT2_SRC) -o $(CLIENT2_OBJ) $(SSLFLAGS)
	
$(CLIENT3_OBJ): $(CLIENT3_SRC)
	$(CC) $(CFLAGS) -c $(CLIENT3_SRC) -o $(CLIENT3_OBJ) $(SSLFLAGS)
	
$(CLIENT4_OBJ): $(CLIENT4_SRC)
	$(CC) $(CFLAGS) -c $(CLIENT4_SRC) -o $(CLIENT4_OBJ) $(SSLFLAGS)

$(SERVER): $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -o $(SERVER) $(SSLFLAGS)

$(SERVER_OBJ): $(SERVER_SRC)
	$(CC) $(CFLAGS) -c $(SERVER_SRC) -o ${SERVER_OBJ} $(SSLFLAGS)



#${OBJS}: ${HELPER}

clean:
	rm *.o $(CLIENT1) $(CLIENT2) $(SERVER) $(CLIENT3) $(CLIENT4)
