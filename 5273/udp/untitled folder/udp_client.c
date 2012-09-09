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
#include <ctype.h>

#define MAXBUFSIZE 100



//char *trimwhitespace(char *str, char *trimmed, int trimmed_size);
int trimwhitespace(char *str, char *trimmed, size_t trimmed_size)
{
  
  
  char* temp;
  temp = str;
  char* end = temp + trimmed_size - 1;

  while(isspace(*temp)) 
  	{temp++;}


  if(*temp == 0)  // All spaces?
    return 0; //Returns 0 is empty string


  // Trim trailing space
  //end = temp + strlen(temp) - 1;
  while(end > temp && isspace(*end)) 
  { end--;}
  //printf("temp %slll\n", temp);

  // Write new null terminator
  size_t stringSize = (end - temp) < trimmed_size-1 ? (end - str) : (trimmed_size - 1);
  trimmed[stringSize] = 0;
  //printf("temp %slll\n", temp);

  memcpy(trimmed, temp, trimmed_size);


  return 1;
}

/*
A function to compare the beginning of a string to a reference string
input string is the original
comparision string is the comparison
compLength is 
*/
int stringcmp(char* input, char* comparison, int compLength)
{
	int i;
	for(i = 0; i < compLength; i++)
	{
		//printf("input[i] %c\n", input[i]);
		if(input[i] == '\0' || input[i] != comparison[i])
		{
			return 0;
		}
	}
	return 1;
}

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
		//char command[] = "ls";
		//char* inst1, op1, op2, op3, op1clar, op2clar, op3clar,
		char *inst1 = "\nPlease enter one of the following options: \n";
		char *op[] = {"put <file_name>\n","ls\n","exit\n\n"};
		//char *opclar[] = {"put<file_name> will send the file to the server.\n","ls will ask the server for a list of files in its directory\n", "exit will exit the server\n"};
		//char op1 = "put <file_name>\n";
		//op2 = "ls\n";
		//op3 = "ls\n";
		char command[50];
		char trimmer[50];
		FILE *fp;
		char* filename;
		while(1){
			bzero(command,sizeof(command));
			fflush(stdin);
			fputs(inst1, stdout);
			int i;
			for(i=0;i<3;i++){
				fputs(op[i],stdout);
			}
			/*for(i=0;i<3;i++){
				fputs(opclar[i],stdout);
			}*/
			
			fflush(stdout);
			if ( fgets(command, sizeof command, stdin) != NULL )
			{
	      		char *newline = strchr(command, '\n'); /* search for newline character */
				if ( newline != NULL )
				{
	         		*newline = '\0'; /* overwrite trailing newline */
				}
				//printf("command = \"%s\"\n", command);
			}
			//printf("command %s\n", command);
			trimwhitespace(command, trimmer,sizeof(trimmer));
			//printf("command %s\n", command);
			//printf("trimmer %s\n", trimmer);
			strcpy(command,trimmer);

			bzero(trimmer,sizeof(trimmer));
			printf("\n");

			char file_buffer[1024];
			int num_bytes_returned;
			if(stringcmp(command,"put ", 4))
			{
				filename = &command[4];
				fp = fopen(filename,"r");
				if(fp != NULL){
					//printf("HERE\n");
					if((nbytes = sendto(sock, &command, sizeof(command), 0, (struct sockaddr *)&remote, remote_length)) == -1) 
					{printf("Client error on packet send %s\n",command);}
					if((nbytesrecv = recvfrom(sock, buffer, MAXBUFSIZE, 0, (struct sockaddr *)&remote, &remote_length)) == -1)
				   	{printf("error on packet receive\n");}
				   	//printf("The server says %s\n", buffer);
				   	if(buffer[0] == 'y'){
						//FILE *fopen(const char *filename, const char *mode);
				   		//memcpy(filename, command,sizeof(command));
						do{
							//size_t fread(void *ptr, size_t size_of_elements, size_t number_of_elements, FILE *a_file);
							num_bytes_returned = fread(file_buffer,sizeof(char),sizeof(file_buffer),fp);
							if(num_bytes_returned==-1)
							{
								printf("Error Reading File, Try again...\n");
								break;
							}
							else if(num_bytes_returned > 0)
							{
								if((nbytes = sendto(sock, file_buffer, sizeof(file_buffer), 0, (struct sockaddr *)&remote, remote_length)) == -1) 
								{
									printf("Client error on packet send\n");
								}

							}
						}while(num_bytes_returned > 0);
						fclose(fp);
					
					
					//printf("the file is closed, send end\n");
						bzero(command,sizeof(command));
						command[0] = 'e';
						if((nbytes = sendto(sock, &command, sizeof(command), 0, (struct sockaddr *)&remote, remote_length)) == -1) 
						{
							printf("Client error on packet send %s\n",command);
						}
					}	
				}
				else{
						bzero(command,sizeof(command));
						printf("File does not exist\n\n");
				}
				/*if((nbytesrecv = recvfrom(sock, buffer, MAXBUFSIZE, 0, (struct sockaddr *)&remote, &remote_length)) == -1)
			   	{
			   		printf("error on packet receive\n");
			   	}
			   	printf("The server says %s\n", buffer);*/
			}
			else if(stringcmp(command,"ls",2)){
				if((nbytes = sendto(sock, &command, sizeof(command), 0, (struct sockaddr *)&remote, remote_length)) == -1) 
				{
					printf("Client error on packet send\n");
				}
				bzero(buffer,sizeof(buffer));
				if((nbytesrecv = recvfrom(sock, buffer, MAXBUFSIZE, 0, (struct sockaddr *)&remote, &remote_length)) == -1)
			   	{
			   		printf("error on packet receive\n");
			   	}
			   	printf("Server says:\n%s\n", buffer);
			}
			else{
				/*
				int sendto(int sockfd, const void *msg, int len, unsigned int flags,
		        const struct sockaddr *to, socklen_t tolen); 
		        */
				
				if((nbytes = sendto(sock, &command, sizeof(command), 0, (struct sockaddr *)&remote, remote_length)) == -1) 
				{
					printf("Client error on packet send\n");
				}

				// Blocks till bytes are received
				/*struct sockaddr_in from_addr;
				int addr_length = sizeof(struct sockaddr);*/
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
			   	printf("%s\n", buffer);
			}
			//nbytes = **** CALL RECVFROM() HERE ****;  
		   	//printf("Bottom of loop\n");
			//printf("Server says %s\n", buffer);
		//}

	   }

	close(sock);

}

