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

/*
Prints the given swphdr
*/
void printHdr(SwpHdr *hdr){
    printf("SeqNum %d AckNum %d Flags %d\n", hdr->SeqNum, hdr->AckNum, hdr->Flags);
}

/*
Sets a SwpHdr to the given values. Takes the SwpHdr as a pointer.
*/
void setSwpHdr(SwpHdr *targetHdr, SwpSeqno seq_num, SwpSeqno ack_num, u_char Flags){
    targetHdr->SeqNum = seq_num;
    targetHdr->AckNum = ack_num;
    targetHdr->Flags = Flags;
    printHdr(targetHdr);
}


/*
Initializes the state for the client side. Takes a pointer ot the state and a pointer
to the array of received acks.
*/
void initializeState(SwpState *state, SwpSeqno *seqno_pter){
	state->LFR = -1; //ends up being 255 - last frame recevied
	state->LAF = RWS; //last acceptable frame
	state->NFE = 0;
	state->receivedPCKT_ptr = seqno_pter;
	int i;
	for(i = 0; i < RWS; i++ ){
		state->recvQ[i].valid = 0;
	}
}

int receiveMsg(SwpState *state, Msg *frame, FILE *fout, int sd, 
			struct sockaddr_in *cliAddr, int sizeAddr){
	
	struct recvQ_slot *slot;

	SwpHdr header;
	SwpSeqno seq_num = frame->m[0];
	SwpSeqno ack_num = frame->m[1];
	u_char flag = frame->m[2];
	setSwpHdr(&header, seq_num,ack_num,flag);
	//SwpHdr ack;
	//setSwpHdr(&ack,0,seq_num, 1); //Prepare ack for sending
	char ack[3];
	ack[0] = 0;
	ack[1] = seq_num;
	ack[2] = 1;
	//Send the ack no matter what packet it is.
	if(sendto_(sd, ack, sizeof(ack), 0, (struct sockaddr *) cliAddr, sizeAddr) == -1)
		{printf("ACK send error\n");}
	printf("%d %d %d %c\n", frame->m[0], frame->m[1], frame->m[2], frame->m[3]);
	u_char mischiefChar = -1; //From the wraparound.
	printf("LAF %d LFR %d Seqno %d\n",state->LAF, state->LFR%mischiefChar, seq_num);
	//If the packet is in the window and has not been already received.
	if(seq_num < state->LAF && seq_num >= state->LFR%mischiefChar ){
		
		slot = &state->recvQ[header.SeqNum % RWS];
		deepCopyArrayShorter(frame,&slot->data); //save copy in slot
		slot->seqno = seq_num; //make note of seqnum
		slot->valid = 1; //This is valid data waiting to be written
		printf("%c %c %c %c\n", slot->data.d[0], slot->data.d[1], slot->data.d[2], slot->data.d[3]);

		if(seq_num == state->NFE){
			//This is the next frame expected. Write the data.
			int num_of_bytes_written = fwrite(slot->data.d,sizeof(char),
												sizeof(slot->data.d),fout);
			slot->valid= 0;
			++state->NFE;
			++state->LFR;
			++state->LAF;
			while(state->recvQ[state->NFE%RWS].valid){ //
			//Write any valid next frames.
				int num_of_bytes_written = fwrite(slot->data.d,sizeof(char),
													sizeof(slot->data.d),fout);
				slot->valid= 0;
				++state->NFE;
				++state->LFR;
				++state->LAF;
			}
		}

	}
	else{
		//Packet wasn't in window so we drop it.
		return -1;
	}
	printf("%c %c %c %c\n", slot->data.d[0], slot->data.d[1], slot->data.d[2], slot->data.d[3]);
}

int main(int argc, char *argv[]) {

	struct sockaddr_in cliAddr, servAddr;
	unsigned int cliLen;
	int sd; //This is the socket
	int rc; 
	char recvmsg[100];
	char buffer[100];
	Msg receiveBuf;
	FILE *fout; //Output file
	SwpSeqno receivedPackets[500]; //(Non-optimal) implementation of the ACK list

    static SwpState state;
    initializeState(&state, receivedPackets);
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
	
	// int receiveMsg(SwpState *state, Msg *frame, FILE fout, int sd, 
	// 		struct sockaddr_in *cliAddr, int sizeAddr){
	receiveMsg(&state,&receiveBuf, fout, sd, &cliAddr, sizeof(cliAddr));

	// if(sendto_(sd, ack, strlen(ack), 0, (struct sockaddr *) &cliAddr,
 //    	sizeof(cliAddr)) == -1){
	// 	printf("ACK send error\n");
	// }

	// int num_of_bytes_written = fwrite(receiveBuf.m,sizeof(char),sizeof(receiveBuf.m),fout);
	

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
	fclose(fout);
	return 0;

}

