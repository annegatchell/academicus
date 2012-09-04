#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>

#define MAXBUFSIZE 100

/* You will have to modify the program below */

int main (int argc, char * argv[])
{

	int nbytes;                             // number of bytes send by sendto()
	int sock;                               //this will be our socket
	char buffer[MAXBUFSIZE];
	int nbytesrecv;							//number of bytes we receive in our message

	struct sockaddr_in remote;              //"Internet socket address structure"
	unsigned int remote_length = sizeof(remote);

	if (argc < 3)
	{
		printf("USAGE:  <server_ip> <server_port>\n");
		exit(1);
	}

	/******************
	  Here we populate a sockaddr_in struct with
	  information regarding where we'd like to send our packet 
	  i.e the Server.
	 ******************/
	bzero(&remote,sizeof(remote));               //zero the struct
	remote.sin_family = AF_INET;                 //address family
	remote.sin_port = htons(atoi(argv[2]));      //sets port to network byte order
	//int inet_pton(int af, const char *src, void *dst);
	//Converts the src character string into an address structure AF_INET or AF_INET6
	int retVal;
	retVal = inet_pton(AF_INET, argv[1], &remote.sin_addr.s_addr); //sets remote IP address
	if(retVal == 0)
	{
		printf("Invalid IP address for server\n");
		//TO DO: More error handling
	}
	else if(retVal == -1){
		printf("Invalid address family for IP address\n");
	}	



	/**************
	Causes the system to create a generic socket of type UDP (datagram)
	//int socket(int domain, int type, int protocol); 
	//PF_INET is the protocol family. Many people use AF_INET in this place, but technically it should be PF_INET
	//SOCK_DGRAM is the type of socket: datagram
	//UDP protocol (the standard transport layer protocol for datagrams in IP networks). 
	****************/
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		printf("unable to create socket\n");
	}
	printf("Client created a socket!\n");

	/* 
	We do not need to call bind(), the system will decide the port and interface to use)
	*/
	printf("Sending packet apple\n");

	//while(1)
	//{
		/******************
		  sendto() sends immediately.  
		  it will report an error if the message fails to leave the computer
		  however, with UDP, there is no error if the message is lost in the network once it leaves the computer.
		 ******************/
		char command[] = "ls";	

		/*
		int sendto(int sockfd, const void *msg, int len, unsigned int flags,
        const struct sockaddr *to, socklen_t tolen); 
        */
	
		if((nbytes = sendto(sock, &command, sizeof(command), 0, (struct sockaddr *)&remote, remote_length)) == -1) 
		{
			printf("Client error on packet send\n");
		}

		// Blocks till bytes are received
		struct sockaddr_in from_addr;
		int addr_length = sizeof(struct sockaddr);
		bzero(buffer,sizeof(buffer));

		/********
		int recvfrom(int sockfd, void *buf, int len, unsigned int flags,
	             struct sockaddr *from, int *fromlen); 
	             
	    sockfd = socket descriptor to read from
	    *buf = buffer to read to
	   	len = length of buffer to read to
	   	flags = 0 WHY WHY WHY
	   	from = a pointer to a local struct sockaddr_storage that will be filled with the IP address and port of the originating machine
	   	fromlen = a local int that will be set to the size of the address in from
		*********/		
	   	if((nbytesrecv = recvfrom(sock, buffer, MAXBUFSIZE, 0, (struct sockaddr *)&remote, &remote_length)) == -1)
	   	{
	   		printf("error on packet receive\n");
	   	}
	   	printf("The server says %s\n", buffer);
		//nbytes = **** CALL RECVFROM() HERE ****;  

		//printf("Server says %s\n", buffer);
	//}



	close(sock);

}

