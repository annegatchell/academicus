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
#include <semaphore.h>

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
    struct timeval void_timeout;
    void_timeout.tv_sec = 2147483647;
    void_timeout.tv_usec = 2147483647;

    state->LAR = -1; //Ends up being 255
    state->LFS = -1; //Ends up being 255
    sem_init(&state->sendWindowNotFull, 0, SWS); //set the semaphore to be the size of the SWS 
    setSwpHdr(&state->hdr, 0 , 0 , 0); //Flag = 0 when data, 1 when ack
    state->receivedACK_ptr = seqno_pter;
    state->newSend = SWS;
    int i;
    for(i = 0; i < SWS; i++){
        state->sendQ[i].timeout = void_timeout;
    }
}



int sendNewFrame(SwpState *state, Msg *frame, char isSelectRunning, int sd, 
                    fd_set *read_fds, struct timeval *default_timeout, 
                    struct sockaddr_in *remoteServAddr, int sizeAddr){
    struct sendQ_slot *slot;

    struct timeval void_timeout;
    void_timeout.tv_sec = 2147483647;
    void_timeout.tv_usec = 2147483647;


    state->hdr.SeqNum = ++state->LFS;
    slot = &state->sendQ[state->hdr.SeqNum%SWS];
    state->hdr.Flags = 0;
    --state->newSend;
    frame->m[0] = state->hdr.SeqNum;
    frame->m[1] = state->hdr.AckNum;
    frame->m[2] = state->hdr.Flags;
    printf("%d %d %d %c\n", frame->m[0], frame->m[1], frame->m[2], frame->m[3]);
    //printf("%ld\n", sizeof(frame->m));
    deepCopyArray(frame,&slot->msg); 
    slot->acked = 0;
    printf("%d %d %d %c\n", slot->msg.m[0], slot->msg.m[1], slot->msg.m[2], slot->msg.m[3]);
    gettimeofday(&slot->timeout, NULL);
    //Send the frame;
    if(sendto_(sd, slot->msg.m, sizeof(slot->msg.m),0, (struct sockaddr *) remoteServAddr,
                sizeAddr) < 0)
    {
        printf("EROROROEOs\n");
    }
    // //Start select;
    // int noSelects = 1;
    // int i;
    // for(i=0;i<SWS;i++){
    //     if(state->sendQ[i].timeout.tv_sec != void_timeout.tv_sec){
    //         noSelects = 0;
    //         break;
    //     }
    // }
    if(frame->m[0] == 0){
        FD_ZERO(read_fds); //clear the select set
        FD_SET(sd, read_fds); //add socket to the listening list
        if (select(sd+1, read_fds, NULL, NULL, default_timeout) == -1) {
            perror("select");
            exit(4);
        }
    }
    return 3;//sendto_(sd, frame->m, sizeof(frame->m),0, (struct sockaddr *) remoteServAddr,
            //sizeof(remoteServAddr));
}

int main(int argc, char *argv[]) {
typedef char * string;

    struct sockaddr_in cliAddr, remoteServAddr; //Socker addresses for the client and remote
    unsigned int remote_length = sizeof(remoteServAddr); //Length of remote address struct
    int sd; //The socket that we will use
    u_char receive_buffer[3]; //The buffer that receives acks. 512 bytes long
    struct timeval default_timeout; //The default timeout we are using for the select()
    default_timeout.tv_sec = 2; //2 secs
    default_timeout.tv_usec = 500000; //0.5 secs
    fd_set read_fds;  // temp file descriptor list for select()

    //Initialize the state of the client
    SwpSeqno receivedAcks[500]; //(Non-optimal) implementation of the ACK list
    static SwpState state;
    initializeState(&state, receivedAcks);


    //Check command line args.
    if(argc<6) {
	printf("usage : %s <serve_ip_address> <error_rate> <random_seed> <send_file> <send_log> \n", argv[0]);
	exit(1);
    }
    //Print error rate
    printf("error rate : %f\n",atof(argv[2]));

    /* Note: you must initialize the network library first before calling
       sendto_().  The arguments are the <error_rate> and <random_seed> */
    init_net_lib(atof(argv[2]), atoi(argv[3]));

    /* Test printing to the client Log */
    /*int temp[] = {1,2,3,5};
    int size_of_temp = 4;
    toLog(1, argv[5], "Send", state.hdr.SeqNum, temp, size_of_temp, &state);*/

    /* get server IP address (input must be IP address, not DNS name) */
    printf("%s: sending data to '%s' \n", argv[0], argv[1]);

    //Set up the address of the remote server.
    bzero(&remoteServAddr, sizeof(remoteServAddr)); //zero out the address
    remoteServAddr.sin_family = AF_INET; //address family
    remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT); //Use the port we set up

    //Convert the char string into an INET address structure
    int retVal;
    retVal = inet_pton(AF_INET, argv[1], &remoteServAddr.sin_addr);
    if(retVal == -1){
        printf("Invalid address family for IP address\n");
    }

    //Same deal with our client address
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
    Open up our send file to get it ready to packet-ize and send
    */
    FILE *fsend;
    fsend = fopen(argv[4],"r"); //Open the filename we entered
    if(fsend == NULL){
        printf("Please use an existing file to send\n");
        printf("usage : %s <serve_ip_address> <error_rate> <random_seed> <send_file> <send_log> \n", argv[0]);
        exit(1);
    }
    
    //Prep for the send loop
    FD_ZERO(&read_fds); //clear the select set
    FD_SET(sd, &read_fds); //add socket to the listening list
    int nbytes;
    char doneReadingFile = 0; //Flag to tell us if we are done reading the file
    SwpSeqno lastSeqNum = -1; //What is the last sequence number?
    Msg frame;
    char isSelectRunning = 0;
    struct timeval new_timeout; //The default timeout we are using for the select()
    new_timeout.tv_sec = 2; //2 secs
    new_timeout.tv_usec = 500000; //0.5 secs
    struct timeval void_timeout;
    void_timeout.tv_sec = 2147483647;
    void_timeout.tv_usec = 2147483647;
    struct recvQ_slot *rslot;
    /*
    Start the loop for sliding window.
    Stop conditions are that the whole file has been read and the LAR == LFS
    */
    int count = 0;
    do{
        //size_t fread(void *ptr, size_t size_of_elements, size_t number_of_elements, FILE *a_file);
        //printf("size of reading%ld\n",sizeof(Msg)-3*sizeof(char));
        //Read a frame's worht of the file.
        if((nbytes = fread(&frame.m[3],sizeof(char),sizeof(frame.m)-3*sizeof(u_char),
                            fsend))== -1){
            printf("Error Reading File, Try again...\n");
            // break;
        }
        //If the file is done being read
        else if (nbytes == 0){
            doneReadingFile = 1;
            lastSeqNum = state.hdr.SeqNum;
        }
        //If there was something to read!
        else{
            //printf("positions 4 5 6 %c %c %c\n",frame.m[3], frame.m[4],frame.m[5]);
            if(sendNewFrame(&state, &frame, isSelectRunning, sd,&read_fds, 
                            &default_timeout,&remoteServAddr,sizeof(remoteServAddr)) < 0){
               printf("Error on packet send\n");
            }
            // if(sendto_(sd, frame.m, sizeof(frame.m),0, (struct sockaddr *) &remoteServAddr,
            //     sizeof(remoteServAddr)) < 0)
            // {
            //     printf("EROROROEOs\n");
            // }
          
            if (FD_ISSET(sd, &read_fds)){
                printf("received packet!\n");
                if ((nbytes = recv(sd, &receive_buffer, sizeof(receive_buffer), 0)) <= 0) {
                    // got error or connection closed by client
                    if (nbytes == 0) {
                    // connection closed
                        printf("selectserver: socket %d hung up\n", sd);
                    } else {
                        perror("recv");
                    }
                } else 
                {//Got an ACK
                    printf("got %d\n", receive_buffer[1]);
                    SwpSeqno ack = receive_buffer[1];
                    if(ack == state.LAR +1){
                        state.sendQ[ack%SWS].timeout = void_timeout;
                        struct timeval *least = &state.sendQ[0].timeout;
                        struct timeval *temp;
                        int i;
                        for(i = 0;i < SWS;i++){
                            temp = &state.sendQ[0].timeout;
                            if((least->tv_sec * 1000000 + least->tv_usec) > 
                                (temp->tv_sec * 1000000 + temp->tv_usec)){
                                least = temp;
                            }
                        }
                        FD_ZERO(&read_fds); //clear the select set
                        FD_SET(sd, &read_fds); //add socket to the listening list
                        if (select(sd+1, &read_fds, NULL, NULL, least) == -1) {
                            perror("select");
                            exit(4);
                        }
                        ++state.newSend;
                        ++state.LAR;
                        ++state.LFS;
                        state.sendQ[ack%RWS].acked = 0;
                        while(state.sendQ[(state.LFS+1)%RWS].acked){
                            ++state.newSend;
                            ++state.LAR;
                            ++state.LFS;
                            state.sendQ[ack%RWS].acked = 0;   
                        }
                    }
                    else if(ack > state.LFS && ack < state.LAR){
                        state.sendQ[ack%SWS].timeout = void_timeout;
                        struct timeval *least = &state.sendQ[0].timeout;
                        struct timeval *temp;
                        int i;
                        for(i = 0;i < SWS;i++){
                            temp = &state.sendQ[0].timeout;
                            if((least->tv_sec * 1000000 + least->tv_usec) > 
                                (temp->tv_sec * 1000000 + temp->tv_usec)){
                                least = temp;
                            }
                        }
                        FD_ZERO(&read_fds); //clear the select set
                        FD_SET(sd, &read_fds); //add socket to the listening list
                        if (select(sd+1, &read_fds, NULL, NULL, least) == -1) {
                            perror("select");
                            exit(4);
                        }
                        state.sendQ[ack%RWS].acked = 1;
                    }
                }
            }
            else{
                printf("Timed out. Resend\n");
                SwpSeqno done;
                int i;
                struct timeval *temp;
                struct timeval *least = &state.sendQ[0].timeout;
                //Find which packet must be resent.
                for(i = 0;i < SWS;i++){
                    temp = &state.sendQ[0].timeout;
                    if((temp->tv_sec * 1000000 + temp->tv_usec) >= 0)
                    {
                        done = state.sendQ[i].msg.m[0];
                        gettimeofday(&state.sendQ[i].timeout, NULL);
                        //Ressend the frame;
                        if(sendto_(sd, state.sendQ[i].msg.m, sizeof(state.sendQ[i].msg.m),0, 
                            (struct sockaddr *) &remoteServAddr, sizeof(remoteServAddr)) < 0){
                            printf("EROROROEOs\n");
                        }
                    }
                }

                //Redo select
                for(i = 0;i < SWS;i++){
                    temp = &state.sendQ[0].timeout;
                    if((least->tv_sec * 1000000 + least->tv_usec) > 
                        (temp->tv_sec * 1000000 + temp->tv_usec)){
                        least = temp;
                    }
                }
                FD_ZERO(&read_fds); //clear the select set
                FD_SET(sd, &read_fds); //add socket to the listening list
                new_timeout.tv_sec = (int)least->tv_sec; //2 secs
                printf("%d\n", least->tv_sec);
                new_timeout.tv_usec = (int)least->tv_usec; 
                if (select(sd+1, &read_fds, NULL, NULL, &default_timeout) == -1) {
                    perror("select");
                    exit(4);
                }


                //sendto_(sd, msgs[iter], strlen(msg),0, (struct sockaddr *) &remoteServAddr,
                //sizeof(remoteServAddr));
            }
        }
        
        count++;

    }while(!doneReadingFile && state.LAR != state.LFS);
    fclose(fsend);


/*
    int sendto_(int sockfd, const void *msg, int len, unsigned int flags,
                const struct sockaddr *to, socklen_t tolen);
*/
/*
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
            
*/
        


    //}


    return 0;
}
