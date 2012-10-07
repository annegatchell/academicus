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
#include "sendto_h"

#define LOCAL_SERVER_PORT 50000


int main(int argc, char *argv[]) {

	struct sockaddr_in cliAddr, servAddr;
	unsigned int cliLen;

	/* You should have some command-line processing that follows the format
	   ./server_SRP <error_rate> <random_seed> <output_file> <receive_log>
	   */

	if(argc<5) {
		printf("usage : %s <error rate> <random seed> <send_file> <send_log> \n", argv[0]);
		exit(1);
	}
	printf("error rate : %f\n",atof(argv[1]));


	/* Note: you must initialize the network library first before calling
	   sendto_().  The arguments are the <errorrate> and <random seed> */
	init_net_lib(atof(argv[1]), atoi(argv[2]));

	/* socket creation */
	sd=socket(***************************);
	if(sd<0) {
		printf("%s: cannot open socket \n",argv[0]);
		exit(1);
	}

	/* bind local server port.  Note the server port must be a
	   "well-known" whose value is known by the client */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(LOCAL_SERVER_PORT);
	rc = bind (************************************************);
	if(rc<0) {
		printf("%s: cannot bind port number %d \n",argv[0], LOCAL_SERVER_PORT);
		exit(1); 
	}

	printf("%s: waiting for data on port UDP %u\n",argv[0],LOCAL_SERVER_PORT);
	char recvmsg[100];
	int n = recvfrom(sd, &rcvmsg, sizeof (recvmsg), 0,
			(struct sockaddr *) &cliAddr, &cliLen);
}

