/* client.c */
/* This is a sample UDP client/sender.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>   /* memset() */
#include <sys/time.h> /* select() */
#include <signal.h>
#include <unistd.h>

#include <sendto_.h>
#include <helper.h>

#define REMOTE_SERVER_PORT 50000
#define BUFSIZE 512

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
    state->LAR = -1; //Ends up being 255
    state->LFS = -1; //Ends up being 255
    sem_init(&state->sendWindowNotFull, 0, SWS); //set the semaphore to be the size of the SWS 
    setSwpHdr(&state->hdr, 0 , 0 , 0);
    state->receivedACK_ptr = seqno_pter;
}

int main(int argc, char *argv[]) {
typedef char * string;

    struct sockaddr_in cliAddr, remoteServAddr; //Socker addresses for the client and remote
    unsigned int remote_length = sizeof(remoteServAddr); //Length of remote address struct
    int sd; //The socket that we will use
    Msg receive_buffer; //The buffer we use to send our data it is 512 bytes long
    struct timeval default_timeout; //The default timeout we are using for the select()
    default_timeout.tv_sec = 2; //2 secs
    default_timeout.tv_usec = 500000; //0.5 secs
    fd_set read_fds;  // temp file descriptor list for select()

    //Initialize the state of the client
    SwpSeqno receivedAcks[500];
    static SwpState state;
    initializeState(&state, receivedAcks);


    //check command line args.
    if(argc<6) {
	printf("usage : %s <serve_ip_address> <error_rate> <random_seed> <send_file> <send_log> \n", argv[0]);
	exit(1);
    }

    printf("error rate : %f\n",atof(argv[2]));

    /* Note: you must initialize the network library first before calling
       sendto_().  The arguments are the <error_rate> and <random_seed> */
    init_net_lib(atof(argv[2]), atoi(argv[3]));

    /* Test printing to the client Log */
    int temp[] = {1,2,3,5};
    int size_of_temp = 4;
    toLog(1, argv[5], "Send", state.hdr.SeqNum, temp, size_of_temp, &state);

    /* get server IP address (input must be IP address, not DNS name) */

    printf("%s: sending data to '%s' \n", argv[0], argv[1]);

    bzero(&remoteServAddr, sizeof(remoteServAddr));
    remoteServAddr.sin_family = AF_INET;
    remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);

    int retVal;
    retVal = inet_pton(AF_INET, argv[1], &remoteServAddr.sin_addr);
    if(retVal == -1){
        printf("Invalid address family for IP address\n");
    }

    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(0);

    /* socket creation */
    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sd<0) {
	printf("%s: cannot open socket \n",argv[0]);
	exit(1);
    }


/*
    int sendto_(int sockfd, const void *msg, int len, unsigned int flags,
                const struct sockaddr *to, socklen_t tolen);
*/

    FD_ZERO(&read_fds);
    FD_SET(sd, &read_fds);
    int nbytes;
    //sd; //Keep track of biggest file descriptor

    //for(;;){
    char msg[] = "send this";
    char msg2[] = "send that";
    string msgs[4];
    msgs[0] = "0send this";
    msgs[1] = "1send that";
    msgs[2] = "2cat in ";
    msgs[3] = "3hat";
    int iter = 0;
    while(iter < 4){
        sendto_(sd, msgs[iter], strlen(msg),0, (struct sockaddr *) &remoteServAddr,
            sizeof(remoteServAddr));

        if (select(sd+1, &read_fds, NULL, NULL, &default_timeout) == -1) {
            perror("select");
            exit(4);
        }

        if (FD_ISSET(sd, &read_fds)){
            printf("received packet!\n");
            if ((nbytes = recv(sd, receive_buffer, sizeof(receive_buffer), 0)) <= 0) {
                // got error or connection closed by client
                if (nbytes == 0) {
                // connection closed
                    printf("selectserver: socket %d hung up\n", sd);
                } else {
                    perror("recv");
                }
            } else 
            {
                printf("%s\n", receive_buffer);
                iter++;
            }
        }
        else{
            printf("Timed out. Resend\n");
            //sendto_(sd, msgs[iter], strlen(msg),0, (struct sockaddr *) &remoteServAddr,
            //sizeof(remoteServAddr));
        }
    }
            

        


    //}


    return 0;
}
