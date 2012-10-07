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
#include "../sendto_.h"

#define REMOTE_SERVER_PORT 50000

struct sockaddr_in cliAddr, remoteServAddr;

int main(int argc, char *argv[]) {
    
    //check command line args.
    if(argc<6) {
	printf("usage : %s <server> <error rate> <random seed> <send_file> <send_log> \n", argv[0]);
	exit(1);
    }

    printf("error rate : %f\n",atof(argv[2]));

    /* Note: you must initialize the network library first before calling
       sendto_().  The arguments are the <errorrate> and <random seed> */
    init_net_lib(atof(argv[2]), atoi(argv[3]));

    /* get server IP address (input must be IP address, not DNS name) */

    printf("%s: sending data to '%s' \n", argv[0], argv[1]);

    remoteServAddr.sin_family = AF_INET;
    remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);
    inet_pton(AF_INET, argv[1], &remoteServAddr.sin_addr);

    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(0);

    /* socket creation */
    sd = socket(************************);
    if(sd<0) {
	printf("%s: cannot open socket \n",argv[0]);
	exit(1);
    }

    msg[] = "send this";
    sendto_(sd,msg, strlen(msg),0, (struct sockaddr *) &remoteServAddr,
	    sizeof(remoteServAddr));
}
