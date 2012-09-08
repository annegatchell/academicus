#include <sys/types.h>
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
#include <string.h>
/* You will have to modify the program below */

#define MAXBUFSIZE 1024
#define PATH_MAX 512 //
#define FILENM_MAX 512 //No the machine

int stringcmp(char* input, char* comparison, int compLength);

int stringcmp(char* input, char* comparison, int compLength)
{
	int i;
	for(i = 0; i < compLength; i++)
	{
		printf("input[i] %c\n", input[i]);
		if(input[i] == '\0' || input[i] != comparison[i])
		{
			return 0;
		}
	}
	return 1;
}

int getFileNameSize(char file[])
{
	int i = 0;
	while(file[i] != 0)
	{
		i++;
	}
	return i;
}

int main (int argc, char * argv[] )
{

	int done = 0;
	int sock;                           //This will be our socket
	struct sockaddr_in sin, remote;     //"Internet socket address structure"
	unsigned int remote_length;         //length of the sockaddr_in structure
	int nbytes;                        //number of bytes we receive in our message
	char buffer[MAXBUFSIZE];             //a buffer to store our received message
	if (argc != 2)
	{
		printf ("USAGE:  <port>\n");
		exit(1);
	}
	/******************
	  This code populates the sockaddr_in struct with
	  the information about our socket
	 ******************/
	bzero(&sin,sizeof(sin));                    //zero the struct
	sin.sin_family = AF_INET;                   //address family
	sin.sin_port = htons(atoi(argv[1]));        //htons() sets the port # to network byte order
	sin.sin_addr.s_addr = INADDR_ANY;           //supplies the IP address of the local machine
	//Do I need to to the htonl?

	/**************
	Causes the system to create a generic socket of type UDP (datagram)
	//int socket(int domain, int type, int protocol); 
	//PF_INET is the protocol family. Many people use AF_INET in this place, but technically it should be PF_INET
	//SOCK_DGRAM is the type of socket: datagram
	//UDP protocol (the standard transport layer protocol for datagrams in IP networks). 
	****************/
	if ((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		printf("unable to create socket\n");
	}
	printf("Server created a socket!\n");

	/******************
	  Once we've created a socket, we must bind that socket to the 
	  local address and port we've supplied in the sockaddr_in struct
	  int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
	  You can cast the sockaddr_in struct into a sockaddr. They have the same structure.
	  sock = our socket file descriptor
	  the cast sin = the pointer to our address info, etc.
	  addrlen = the length of that address in bytes
	 ******************/
	if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		printf("unable to bind socket\n");
	}
	printf("Server bound a socket!\n");
	remote_length = sizeof(remote);

	//waits for an incoming message
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
   	while(!done)
   	{
		bzero(buffer,sizeof(buffer));
		if((nbytes = recvfrom(sock, buffer, MAXBUFSIZE, 0, (struct sockaddr *)&remote, &remote_length)) == -1)
		{
			printf("error on packet receive\n");
		}
		
		printf("The client says %s\n", buffer);

		char msg[] = "y";
		if (stringcmp(buffer, "put ", 4))
		{
			
			if(buffer[4] != '\0')
			{
				printf("There must be a file\n");
			}
			if((nbytes = sendto(sock, &msg, sizeof(msg), 0, (struct sockaddr *)&remote, remote_length)) == -1) 
			{
				printf("Client error on packet send\n");
			}
			//printf("Server received file\n");
			FILE *fp;
			const char* filename;
			filename = &buffer[4];
			printf("filename %s\n", filename);		
			fp = fopen(filename,"a");
			int number_of_bytes_written;
			do{
				bzero(buffer,sizeof(buffer));
				if((nbytes = recvfrom(sock, buffer, MAXBUFSIZE, 0, (struct sockaddr *)&remote, &remote_length)) == -1)
					{printf("error on packet receive\n");}
				//size_t fwrite(const void *ptr, size_t size_of_elements, size_t number_of_elements, FILE *a_file);
				if(!stringcmp(buffer, "e",1)){
					number_of_bytes_written = fwrite(buffer,sizeof(char),sizeof(buffer),fp);
				}
			}while(!stringcmp(buffer, "e",1));
			fclose(fp);
			printf("wrote the file\n");
			/*char done[] = "Done";
			if((nbytes = sendto(sock, &done, sizeof(done), 0, (struct sockaddr *)&remote, remote_length)) == -1) 
			{
				printf("Client error on packet send\n");
			}*/

		}
		else if (stringcmp(buffer, "ls", 2))
		{
			FILE *fp;
			int status;
			char file[FILENM_MAX];
			//char * file1;
			char path[PATH_MAX];
			char * pathptr = path;

			
			fp = popen("ls *", "r"); //Write the ls * command output to stream fp, read only
			if(fp == NULL)
			{
				printf("Error in getting ls stream\n");
			}
			else
			{

				while(fgets(file, FILENM_MAX, fp) != NULL)
				{

					printf("size of filename %ld\n", sizeof(file));
					int size = getFileNameSize(file);
					printf("size of filename %d\n", size);
					strncpy(path, file, size);
					//*pathptr = file[0];
					pathptr += size;
					*pathptr = '\n';
					pathptr++;
					//strcat(file1, path);
				}
				*pathptr = '\0';
			}

			status = pclose(fp);
			if(status == -1)
			{
				printf("pslose: Error closing the file\n");
			}
			
			if((nbytes = sendto(sock, &path, sizeof(path), 0, (struct sockaddr *)&remote, remote_length)) == -1) 
			{
				printf("SErver error on packet send\n");
			}
			printf("Server sent file path\n");
		}
		else if (stringcmp(buffer, "exit", 4))
		{
			bzero(buffer,sizeof(buffer));
			char exitmsg[] = "Exiting the server";
			memcpy(buffer, exitmsg, sizeof(exitmsg));
			//free(exitmsg);
			done = 1;
			if((nbytes = sendto(sock, &buffer, sizeof(buffer), 0, (struct sockaddr *)&remote, remote_length)) == -1) 
			{
				printf("Client error on packet send\n");
			}
			printf("Exiting the server.........\n");
			close(sock);
		}
		else
		{

			if((nbytes = sendto(sock, &buffer, sizeof(buffer), 0, (struct sockaddr *)&remote, remote_length)) == -1) 
			{
				printf("Client error on packet send\n");
			}
			printf("Server sent buffer back\n");
		}

   	}

	close(sock);
	return 0;
}

