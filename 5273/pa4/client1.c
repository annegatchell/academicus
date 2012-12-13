/*
 * client.c
 *
 	Ari Summer
 	Programming Assignment 3
 	11/16/2012
	TCP Client
 *  
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>

#include "clientlib.h"

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAX_CONNECTS 50
#define OFFSET1 20
#define OFFSET2 200
#define CMDSIZE 100
#define FILELISTSIZE 4048
#define MAXBUFSIZE (24*1024)
#define EXITCMD "exit"
#define LISTCMD "list"
#define SENDFILES "sendmyfileslist"
#define GET "get"
#define DELM " "
#define COMMA ","
#define NAME "name"
#define IP "ip"
#define MSG "msg"
#define TRANSFERPORT "5050"
#define DLDIR "../Downloaded_Files"

//#define RSA_CLIENT_KEY "../
#define RSA_CLIENT_CERT "client1.crt"
#define RSA_CLIENT_KEY "privatekey-client1.key"
 
#define RSA_CLIENT_CA_CERT "../ssl_stuff/myCA/certs/myca.crt"
#define RSA_CLIENT_CA_PATH "../ssl_stuff/myCA/certs/"

#define RETURN_NULL(x) if ((x)==NULL) exit (1)
#define RETURN_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define RETURN_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(1); }

#define ON 1
#define OFF 0

int sockfd;
char shareddir[100];
char clientname[20];
char dlfile[CMDSIZE];
int alreadyTransferring = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void sendUsrCommands(SSL *ssl);
void sendFileList(SSL *ssl);
void sendName(SSL *ssl);
void getCommand(char buffer[MAXBUFSIZE]);
void sendFile(char filename[CMDSIZE]);
void getFile(char ipaddress[INET6_ADDRSTRLEN]);
int verify_client = OFF; //To verify a client sertificate, set ON
//SSL *ssl;
SSL *hackyGlobal;

void alarmHandler(int sig){
	sendFileList(hackyGlobal);
	alarm(180);
}

int main(int argc, char *argv[])
{

	char buffer[MAXBUFSIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	fd_set readfds;
	pthread_t th;

	int err;
	SSL_CTX         *ctx;
    SSL            *ssl;
    SSL_METHOD      *meth;
    X509            *server_cert;
    EVP_PKEY        *pkey;
    char *str;

	if (argc != 5) {
		fprintf(stderr,"usage: ./client{#} <client name> <server ip> <server port#> <path to shared directory> \n");
		exit(1);
	}

	//==================================================================
	//Get SSL stuff initialized
	SSL_library_init(); //Load encryption and hash algs for SSL
	SSL_load_error_strings(); //Load error strings
	meth = SSLv3_method();
	 /* Create an SSL_CTX structure */
	ctx = SSL_CTX_new(meth);                        
	RETURN_NULL(ctx);
	
	//==============
	if(verify_client == ON)
 
        {
 
              /* Load the client certificate into the SSL_CTX structure */
                if (SSL_CTX_use_certificate_file(ctx, RSA_CLIENT_CERT, 
 
     SSL_FILETYPE_PEM) <= 0) {
                   ERR_print_errors_fp(stderr);
                        exit(1);
            }
 
              /* Load the private-key corresponding to the client certificate */
          if (SSL_CTX_use_PrivateKey_file(ctx, RSA_CLIENT_KEY, 
          SSL_FILETYPE_PEM) <= 0) {
                     ERR_print_errors_fp(stderr);
                        exit(1);
            }
 
              /* Check if the client certificate and private-key matches */
               if (!SSL_CTX_check_private_key(ctx)) {
                      fprintf(stderr,"Private key does not match the certificate public key\n");
                    exit(1);
            }
   }
   
   /* Load the RSA CA certificate into the SSL_CTX structure */
        /* This will allow this client to verify the server's     */
        /* certificate.                                           */
    if (!SSL_CTX_load_verify_locations(ctx, RSA_CLIENT_CA_CERT, NULL)) {
                ERR_print_errors_fp(stderr);
                exit(1);
    }
    
	/* Set flag in context to require peer (server) certificate */
    /* verification */
	SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL);
	SSL_CTX_set_verify_depth(ctx,1);
	
	//==================================================================================
	signal(SIGALRM, alarmHandler);

	strcpy(shareddir, argv[4]);
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[2], argv[3], &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	//SET up TCP Socket
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}


		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}
	printf("connected!\n");

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure

	//Create SSL structure
	ssl = SSL_new(ctx);
	RETURN_NULL(ssl);
	hackyGlobal = ssl;
	printf("here");
	
	//Assign the socket into the SSL structure
	SSL_set_fd(ssl, sockfd);
	
	//Perform SSL Handshake on the SSL client 
	err = SSL_connect(ssl);
	RETURN_SSL(err);
	
	printf("SSL connection using %s\n", SSL_get_cipher(ssl));
	
	//---GET the SERVER's CERTIFICATE
	server_cert = SSL_get_peer_certificate(ssl);
	
	if(server_cert != NULL)
	{
		printf ("Server certificate:\n");
             str = X509_NAME_oneline(X509_get_subject_name(server_cert),0,0);
            RETURN_NULL(str);
           printf ("\t subject: %s\n", str);
           free (str);
 
            str = X509_NAME_oneline(X509_get_issuer_name(server_cert),0,0);
             RETURN_NULL(str);
           printf ("\t issuer: %s\n", str);
            free(str);
 
             X509_free (server_cert);
	}
	else{
		printf("The SSL server does not have certificate.\n");
	}
	//===================================================
	//Now do some data exchange!

	//save client name
	bzero(&clientname,sizeof(clientname));
	strcpy(clientname,argv[1]);


	sendName(ssl); //send name of client to server
	sendFileList(ssl); //send file list to server

	//Create thread that waits for commands from the user
	int r = pthread_create(&th, 0, sendUsrCommands, ssl);
	if (r != 0) { fprintf(stderr, "thread create failed\n"); }

	while(1){

		//Add socket to socket set
		FD_ZERO(&readfds);
		FD_SET(sockfd,&readfds);

		//Wait for communication on socket
		int selRet = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);

		if ((selRet != -1) && (selRet != 0)) {
			//Handle messages from server
			//SSL Style!
			//int rv = recv(sockfd, buffer, MAXBUFSIZE, 0);
			int rv = SSL_read(ssl, buffer, MAXBUFSIZE);
			if (rv > 0){getCommand(buffer);}

			else if(rv == -1){
				printf("Receive error: ");
				perror("recv()");
			}

			else if(rv == 0){
				//Server has disconnected
				printf("Server Closed Socket Connection!\n");
				close(sockfd);
				exit(0);
			}

			//Determine what was received and what to do with it



		}

		else if(selRet == -1) {
			printf("select() failed : \n");
		}



	}

	close(sockfd);

	return 0;

}

//Handle commands from user and send to server
void sendUsrCommands(SSL *ssl){
	char command[CMDSIZE];
	char sndbuffer[MAXBUFSIZE];
	char cmdbuffer[CMDSIZE];
	char filename[CMDSIZE];
	//char list[1024];
	//char lscommand[1024];
	char* fname;
	int err;

	pthread_detach(pthread_self());  //automatically clears the threads memory on exit

	alarm(180);

	while(1){

		//Get command from user
		fflush(stdout);
		printf("Enter Command: ");
		fgets(command,CMDSIZE,stdin);
		command[strlen(command)-1] = '\0'; //Eliminate newline char at end of command

		//Send commands to server based on user commands
		bzero(&sndbuffer,sizeof(sndbuffer));
		printf("command is %s\n", command);
		//Exit client
		if(!strcmp(command, EXITCMD)){

			memcpy(&sndbuffer[0], EXITCMD,sizeof(EXITCMD));
			//send(sockfd, sndbuffer, sizeof(sndbuffer), 0);
			printf("sending: %s\n", sndbuffer);
			SSL_shutdown(ssl);
			close(sockfd);
			exit(0);
		}

		//Get list of files from server
		else if(!strcmp(command, LISTCMD)){
			memcpy(&sndbuffer[0], LISTCMD,sizeof(LISTCMD));
			//Send SSL style!
			//send(sockfd, sndbuffer, sizeof(sndbuffer), 0);
			err = SSL_write(hackyGlobal, sndbuffer, sizeof(sndbuffer));
			RETURN_SSL(err);

		}

		//Send list of files to server
		else if(!strcmp(command, SENDFILES)){
			sendFileList(ssl);
		}

		//Get file from client also on server
		else if(command[0] == 'g'){

			bzero(&filename,sizeof(filename));

			fname = strtok(command,DELM);
			strcpy(cmdbuffer,fname); //put command into cmdbuffer
			fname = strtok(NULL, DELM);
			strcpy(filename,fname);
			bzero(command,sizeof(command));// clear command buffer
			strcpy(command,cmdbuffer);//copy command from cmdbuffer to command

			if(!(strcmp(command,GET))){
				memcpy(&sndbuffer[0], GET,sizeof(GET));
				memcpy(&sndbuffer[OFFSET1], filename, sizeof(filename));

				bzero(&dlfile, sizeof(dlfile));
				strcpy(dlfile, filename);

				printf("Getting File: %s\n", dlfile);

				//Send file name to server
				//SSL style!!
				//send(sockfd, sndbuffer, sizeof(sndbuffer), 0);
				err = SSL_write(ssl, sndbuffer, sizeof(sndbuffer));
				RETURN_SSL(err);

			}
		}

		else{
			printf("Invalid Command. Please try again.\n");

		}
	}

}

/*
	Send list of files to server.
	List is comma separated and contains file sizes as well.
	filename0,size0,filename1,size1,...

*/
void sendFileList(SSL *ssl){
	printf("NON-HACKY\n");
	char sndbuffer[MAXBUFSIZE];
	char filename[CMDSIZE];
	char list[1024];
	char lscommand[1024];
	char directory[1024];
	char sbuf[20];
	int lSize;
	FILE* fpls;
	FILE* fp;
	int err;


	bzero(filename,sizeof(filename));
	bzero(list,sizeof(list));
	bzero(lscommand, sizeof(lscommand));

	strcat(lscommand, "ls ");
	strcat(lscommand,shareddir);

	//Use ls to get the list of files in shared directory
	fpls = popen(lscommand, "r");
	if(fpls == NULL){printf("Error obtaining list of files in directory\n");}

	//Get file names from files
	int flag = 0;
	while(fgets(filename,MAXBUFSIZE,fpls) != NULL){
		//Separate list with commas
		if (flag == 1){
			//strcat(fsize, COMMA);
			strcat(list, COMMA);
		}

		//Get size of each file
		bzero(&directory, sizeof(directory));
		strcat(directory, shareddir);
		strcat(directory,"/");
		strncat(directory,filename,strlen(filename)-1);
		fp = fopen(directory,"r");

		//Get size of file
		fseek(fp,0,SEEK_END);
		lSize = ftell(fp);
		rewind(fp);
		fclose(fp);

		bzero(&sbuf, sizeof(sbuf));
		sprintf(sbuf, "%d", lSize);

		strncat(list,filename,strlen(filename)-1); //Add file names to list buffer
		strcat(list, COMMA);
		strcat(list,sbuf); //Add size of file to list
		flag = 1;
	}

	//Close file
	pclose(fpls);

	memcpy(&sndbuffer[0], SENDFILES,sizeof(SENDFILES));
	memcpy(&sndbuffer[OFFSET1], list,sizeof(list));

	//Send data, SSL style
	err = SSL_write(ssl, sndbuffer, sizeof(sndbuffer));
	RETURN_SSL(err);
	//send(sockfd, sndbuffer, sizeof(sndbuffer), 0);

}

/*
	Send name of client to server.
*/
void sendName(SSL *ssl){
	char namebuffer[MAXBUFSIZE];
	int err;

	memcpy(&namebuffer[0], NAME, sizeof(NAME));
	memcpy(&namebuffer[OFFSET1], clientname, sizeof(clientname));

	//Send SSL style
	err = SSL_write(ssl, namebuffer, sizeof(namebuffer));
	//send(sockfd, namebuffer, sizeof(namebuffer), 0);
}

/*
	Process commands coming from server.

*/
void getCommand(char buffer[MAXBUFSIZE]){
	char command[CMDSIZE];

	//Store command
	bzero(&command,sizeof(command));
	strncpy(command, buffer, OFFSET1);

	//Print out list of files from server
	if(!strcmp(command,LISTCMD)){
		char filelist[FILELISTSIZE];
		memcpy(filelist, &buffer[OFFSET1],sizeof(filelist));
		printf("%s",filelist);
	}

	//Get ready to send a file to another client
	if (!strcmp(command,GET)){

		//Make sure you are not already transferring a file
		if(alreadyTransferring == 0){
			pthread_mutex_lock(&mutex);
			alreadyTransferring = 1;
			pthread_mutex_unlock(&mutex);
			pthread_t th2;
			char filename[CMDSIZE];

			//If not already transferring a file, call sendFile
			//to create a socket and send file
			memcpy(filename, &buffer[OFFSET1],sizeof(filename));
			int g = pthread_create(&th2, 0, sendFile, filename);
			if (g != 0) {
				fprintf(stderr, "thread create failed\n");
				pthread_mutex_lock(&mutex);
				alreadyTransferring = 0;
				pthread_mutex_unlock(&mutex);
			}
		}
	}

	//Get IP address of client that has the requested file
	if (!strcmp(command,IP)){
		char ipaddress[INET6_ADDRSTRLEN];
		pthread_t thr2;

		memcpy(ipaddress, &buffer[OFFSET1],sizeof(ipaddress));

		printf("%s has the file I need\n", ipaddress);

		//Call getFile to create socket and get ready to 
		//receive requested file.
		int g = pthread_create(&thr2, 0, getFile, ipaddress);
		if (g != 0) { fprintf(stderr, "thread create failed\n"); }

	}

	//Print messages from server
	else if(!strcmp(command,MSG)){
		char message[FILELISTSIZE];
		bzero(&message,sizeof(message));
		memcpy(message, &buffer[OFFSET1],sizeof(message));
		printf("%s",message);
	}
}

/*
	Send file to another client that has
	requested it.

*/
void sendFile(char filename[CMDSIZE]){

	FILE* fps;
	int rv2;
	int fsendsock;
	int yes=1;
	struct addrinfo hints2, *servinfo2, *p2;
	struct timeval timeout;
	fd_set readfds2;
	int new_fd;
	socklen_t sin_size;
	struct sockaddr_storage their_addr;

	memset(&hints2, 0, sizeof hints2);
	hints2.ai_family = AF_UNSPEC;
	hints2.ai_socktype = SOCK_STREAM;
	hints2.ai_flags = AI_PASSIVE; // use my IP


	if ((rv2 = getaddrinfo(NULL, TRANSFERPORT, &hints2, &servinfo2)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv2));
		pthread_mutex_lock(&mutex);
		alreadyTransferring = 0;
		pthread_mutex_unlock(&mutex);
		pthread_exit(NULL);
	}

	for(p2 = servinfo2; p2 != NULL; p2 = p2->ai_next) {
		if ((fsendsock = socket(p2->ai_family, p2->ai_socktype,
				p2->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(fsendsock, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
			perror("setsockopt");
			close(fsendsock);
			pthread_mutex_lock(&mutex);
			alreadyTransferring = 0;
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}

		/*
		 * Bind our local address so that the client can send to us.
		 */
		if (bind(fsendsock, p2->ai_addr, p2->ai_addrlen) == -1) {
			close(fsendsock);
			perror("client: bind");
			continue;
		}

		break;
	}

	if (p2 == NULL)  {
		fprintf(stderr, "Client: failed to bind to Transfer Socket\n");
		close(fsendsock);
		pthread_mutex_lock(&mutex);
		alreadyTransferring = 0;
		pthread_mutex_unlock(&mutex);
		pthread_exit(NULL);
	}

	freeaddrinfo(servinfo2); // all done with this structure

	//listen on socket
	if (listen(fsendsock, MAX_CONNECTS) == -1) {
		perror("listen");
		pthread_exit(NULL);
		close(fsendsock);
		pthread_mutex_lock(&mutex);
		alreadyTransferring = 0;
		pthread_mutex_unlock(&mutex);
		pthread_exit(NULL);
	}


	timeout.tv_sec = 30;
	timeout.tv_usec = 0;

	//add socket to set
	FD_ZERO(&readfds2);
	FD_SET(fsendsock,&readfds2);

	//Wait for communication on socket
	int selRet = select(FD_SETSIZE, &readfds2, NULL, NULL, &timeout);

	if ((selRet != -1) && (selRet != 0)) {
		char directory[1024];
		char sndbuffer[MAXBUFSIZE];
		char sizebuffer[MAXBUFSIZE];
		char sbuf[20];
		char ip[INET6_ADDRSTRLEN];

		//if communication was from main socket
		if(FD_ISSET(fsendsock,&readfds2)){

			sin_size = sizeof their_addr;

			//Create new socket for communication to send file
			new_fd = accept(fsendsock, (struct sockaddr *)&their_addr, &sin_size);
			if (new_fd == -1) {
				perror("accept");
				close(fsendsock);
				close(new_fd);
				pthread_mutex_lock(&mutex);
				alreadyTransferring = 0;
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}

			inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),ip, sizeof ip);
			printf("\nSending File %s to %s\n", filename, ip);

			bzero(&directory, sizeof(directory));
			bzero(&sndbuffer,sizeof(sndbuffer));
			bzero(&sizebuffer,sizeof(sizebuffer));

			strcat(directory, shareddir);
			strcat(directory,"/");
			strncat(directory,filename,strlen(filename));
			fps = fopen(directory,"r");

			//If file is not available send message to client
			if (fps == NULL){
				char message[MAXBUFSIZE];
				char nofilemsg[] = "\nFile Not Available\n";
				memcpy(message, MSG, sizeof(MSG));
				memcpy(message[OFFSET1], nofilemsg, sizeof(nofilemsg));

				send(new_fd, message, sizeof(message), 0);

				close(new_fd);
				pthread_mutex_lock(&mutex);
				alreadyTransferring = 0;
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}

			//Get size of file
			fseek(fps,0,SEEK_END);
			int lSize = ftell(fps);
			rewind(fps);

			//Read the file and copy it to filebuffer
			int readResult = fread(sndbuffer,1,lSize,fps);
			if(readResult != lSize){
				printf("Reading error!\n");
				close(fsendsock);
				close(new_fd);
				pthread_mutex_lock(&mutex);
				alreadyTransferring = 0;
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}

			fclose(fps);

			bzero(&sbuf, sizeof(sbuf));
			sprintf(sbuf, "%d", lSize);

			memcpy(sizebuffer,sbuf,sizeof(sbuf));

			int s;

			//Send size of file to client
			s = send(new_fd, sizebuffer, sizeof(sizebuffer), 0);

			if( s == -1){
				printf("Sending error!\n");
				perror("send()");
				close(fsendsock);
				close(new_fd);
				pthread_mutex_lock(&mutex);
				alreadyTransferring = 0;
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}

			//Send file data to client
			s = send(new_fd, sndbuffer, sizeof(sndbuffer), 0);

			if( s == -1){
				printf("Sending error!\n");
				perror("send()");
				close(fsendsock);
				close(new_fd);
				pthread_exit(NULL);
			}

		}
	}
	else if (selRet == 0) {
		printf("Timeout: Client did not join to receive file\n");
		close(fsendsock);
		pthread_mutex_lock(&mutex);
		alreadyTransferring = 0;
		pthread_mutex_unlock(&mutex);
		pthread_exit(NULL);
	}

	else if (selRet == -1) {
		perror("select()");
		printf("select() failed : \n");
		close(fsendsock);
		pthread_mutex_lock(&mutex);
		alreadyTransferring = 0;
		pthread_mutex_unlock(&mutex);
		pthread_exit(NULL);
	}

	pthread_mutex_lock(&mutex);
	alreadyTransferring = 0;
	pthread_mutex_unlock(&mutex);

	close(fsendsock);
	close(new_fd);
	pthread_exit(NULL);

}

/*
	Get ready to receive file from client..

*/
void getFile(char ipaddress[INET6_ADDRSTRLEN]){
	struct addrinfo hintsr2, *servinfor2, *pr2;
	int rv2;
	int recvsockfd;
	char filesize[MAXBUFSIZE];
	char filebuffer[MAXBUFSIZE];
	fd_set readfds2;
	struct timeval timeout;
	int lSize = 0;

	sleep(5);//Wait for client to make connection

	hintsr2.ai_family = AF_UNSPEC;
	hintsr2.ai_socktype = SOCK_STREAM;

	if ((rv2 = getaddrinfo(ipaddress, TRANSFERPORT, &hintsr2, &servinfor2)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv2));
		pthread_exit(NULL);
	}

	// loop through all the results and connect to the first we can
	for(pr2 = servinfor2; pr2 != NULL; pr2 = pr2->ai_next) {
		if ((recvsockfd = socket(pr2->ai_family, pr2->ai_socktype,
				pr2->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		int c;

		c = connect(recvsockfd, pr2->ai_addr, pr2->ai_addrlen);
		if(c == 0){
			break;
		}

		if(c == -1){
			printf("Machine hosting this file is not available at this moment.\n");
			close(recvsockfd);
			pthread_exit(NULL);
			continue;
		}

		break;
	}

	if (pr2 == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		close(recvsockfd);
		pthread_exit(NULL);
	}

	freeaddrinfo(servinfor2);

	timeout.tv_sec = 30;
	timeout.tv_usec = 0;
	FD_ZERO(&readfds2);
	FD_SET(recvsockfd,&readfds2);

	//wait for communication on socket
	int selRet = select(FD_SETSIZE, &readfds2, NULL, NULL, &timeout);

	if ((selRet != -1) && (selRet != 0)) {
		printf("Started Downloading File %s from %s ...\n",dlfile,ipaddress);

		//receive message
		int rv3 = recv(recvsockfd, filesize, MAXBUFSIZE, 0);

		if(rv3 > 0){
			//Get size of file
			char command[CMDSIZE];

			bzero(&command,sizeof(command));

			strncpy(command, filesize, OFFSET1);

			//If there is an error from the client that owned the file
			//print it out and exit
			if(!strcmp(command,MSG)){
				char message[FILELISTSIZE];
				bzero(&message,sizeof(message));
				memcpy(message, &filesize[OFFSET1],sizeof(filesize));
				printf("%s",message);
				close(recvsockfd);
				pthread_exit(NULL);
			}

			//Save size of file.
			lSize = atoi(filesize);

		}

		else if(rv3 == -1){
			printf("Receive error: ");
			perror("recv()");
			close(recvsockfd);
			pthread_exit(NULL);
		}

		else if(rv3 == 0){
			//Server has disconnected
			printf("Client closed connection!\n");
			close(recvsockfd);
			pthread_exit(NULL);
		}
	}
	else if (selRet == 0) {
		printf("Timeout: Client did not send file in time\n");
		close(recvsockfd);
		pthread_exit(NULL);
	}

	else if (selRet == -1) {
		perror("select()");
		printf("select() failed : \n");
		close(recvsockfd);
		pthread_exit(NULL);
	}

	timeout.tv_sec = 30;
	timeout.tv_usec = 0;
	FD_ZERO(&readfds2);
	FD_SET(recvsockfd,&readfds2);
	selRet = select(FD_SETSIZE, &readfds2, NULL, NULL, &timeout);

	if ((selRet != -1) && (selRet != 0)) {

		char directory[1024];
		bzero(&directory, sizeof(directory));
		//Receive SSL style!
		//int rv3 = recv(recvsockfd, filebuffer, MAXBUFSIZE, 0);
		int rv3 = recv(recvsockfd, filebuffer, MAXBUFSIZE, 0);

		if(rv3 > 0){
			//Get file
			FILE* fpr;
			strcat(directory, DLDIR);
			strcat(directory,"/");
			strncat(directory,dlfile,strlen(dlfile));
			fpr = fopen(directory,"w");

			//Write file data to open file
			fwrite(filebuffer,1,lSize,fpr);

			//Close file
			fclose(fpr);
		}

		else if(rv3 == -1){
			printf("Receive error: ");
			perror("recv()");
			close(recvsockfd);
			pthread_exit(NULL);
		}

		else if(rv3 == 0){
			//Server has disconnected
			printf("Server Closed Socket Connection!\n");
			close(recvsockfd);
			pthread_exit(NULL);
		}
	}
	else if (selRet == 0) {
		printf("Timeout: Client did not send file in time\n");
		close(recvsockfd);
		pthread_exit(NULL);
	}

	else if (selRet == -1) {
		perror("select()");
		printf("select() failed : \n");
		close(recvsockfd);
		pthread_exit(NULL);
	}


	close(recvsockfd);
	printf("Finished Downloading File\n");
	pthread_exit(NULL);
}
