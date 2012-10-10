/* Sample UDP server program, server.c,  */
/* Note: This is a very simple example just to get you started.
   For example, we show the server calls recvfrom() to receive data.
   The following are not shown below, but will have to be added to
   your server program for the assignment:
 * you will have to add sendto_() to send ACK's
 * you will also have to initialize the library using init_net_lib()
 * both sendto_() and init_net_lib() need to be declared as external
 * you should probably call select() from within the loop
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include <sendto_.h>
#include <helper.h>


#define LOCAL_SERVER_PORT 50000


int main(int argc, char *argv[]) {

	struct sockaddr_in cliAddr, servAddr;
	unsigned int cliLen;
	int sd; //This is the socket
	int rc; 
	char recvmsg[100];
	char buffer[100];
	Msg receiveBuf;
	FILE *fout; //Output file

    SwpSeqno LFR = 0;
    SwpSeqno LAF = 0;
    sem_t semaphore;
    sem_init(&semaphore, 0, RWS);
    SwpSeqno seq_num = 0;
    SwpSeqno ack_num = 0;
    SwpHdr header = {seq_num, ack_num, 0};
    struct timeval time;
    gettimeofday(&time, NULL);
    Msg clientMsg;

    static SwpState state;
    state.LFS = LFR;
    state.LAR = LAF;

	/* You should have some command-line processing that follows the format
	   ./server_SRP <error_rate> <random_seed> <output_file> <receive_log>
	   */

	if(argc<5) {
		printf("usage : %s <error_rate> <random_seed> <output_file> <receive_log> \n", argv[0]);
		exit(1);
	}
	printf("error rate : %f\n",atof(argv[1]));


	/* Note: you must initialize the network library first before calling
	   sendto_().  The arguments are the <errorrate> and <random seed> */
	init_net_lib(atof(argv[1]), atoi(argv[2]));

	/* socket creation */
	if((sd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
		printf("%s: cannot open socket \n",argv[0]);
		exit(1);
	}


	/* bind local server port.  Note the server port must be a
	   "well-known" whose value is known by the client */
	bzero(&servAddr,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(LOCAL_SERVER_PORT);

	rc = bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
	if(rc<0) {
		printf("%s: cannot bind port number %d \n",argv[0], LOCAL_SERVER_PORT);
		exit(1); 
	}

	int received[100];
	int received_ptr = 0;
	int received_size = 100;
	char temp;
	char ack[4] = "ackl";
	int t;
	//Open the output file
	char* filename = argv[3];
	fout = fopen(filename, "a");

	printf("%s: waiting for data on port UDP %u\n",argv[0],LOCAL_SERVER_PORT);
	int nbytes;
	if((nbytes = recvfrom(sd, &receiveBuf.m, sizeof(receiveBuf.m), 0, 
		(struct sockaddr *) &cliAddr, &cliLen)) == -1)
	{printf("error on packet receive\n");}

	
	printf("%d %d %d %c\n", receiveBuf.m[0], receiveBuf.m[1], receiveBuf.m[2], receiveBuf.m[3]);

	temp = receiveBuf.m[0];
	printf("%c\n", temp);

	if(sendto_(sd, ack, strlen(ack), 0, (struct sockaddr *) &cliAddr,
    	sizeof(cliAddr)) == -1){
		printf("ACK send error\n");
	}
	int num_of_bytes_written = fwrite(receiveBuf.m,sizeof(char),sizeof(receiveBuf.m),fout);
	

/*
	int iter = 0;
	while(iter < 4){
		printf("%s: waiting for data on port UDP %u\n",argv[0],LOCAL_SERVER_PORT);

		int nbytes;
		if((nbytes = recvfrom(sd, &buffer, sizeof (buffer), 0, 
			(struct sockaddr *) &cliAddr, &cliLen)) == -1)
		{printf("error on packet receive\n");}

		//printf("%d %d %d %c\n", frame->m[0], frame->m[1], frame->m[2], frame->m[3]);


		temp = buffer[0];
		t = atoi(&temp);
		printf("%c  %d\n", temp, t);
		if(t == iter -1){
			iter--;
		}
		if(t == iter){
			received[received_ptr] = buffer[0];
			ack[3] = buffer[0];
			iter++;
			char writeInfo[100];
			printf("buffer size %ld \n", sizeof(buffer));
			trimwhitespace(buffer, writeInfo,sizeof(buffer));
			printf("trim %ld\n", sizeof(buffer));
			int size_of_buffer = getSizeOfBuffer(writeInfo);
			printf("size of buffer %d\n", size_of_buffer);
			int num_of_bytes_written = fwrite(writeInfo,sizeof(char),size_of_buffer,fout);

			sendto_(sd, ack, strlen(ack), 0, (struct sockaddr *) &cliAddr,
            	sizeof(cliAddr));
		}

		toLog(0, argv[4], "Receive", seq_num, received, received_size, &state);	
	}
*/

	return 0;

}

