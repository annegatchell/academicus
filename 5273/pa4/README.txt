Anne Gatchell
PA 4
CSCI 5372
12/12/2012

This is a simple server/client SSL/TCP file distribution system. 
Clients have files that they would like to share with other clients 
connected to the server. Clients can see the list of files on the server 
and download files from other clients. The server acts as a coordinator/middle-man. 

Clients and servers all have public and private keys and all connections
are secure and use OpenSSL.


Compile with "make"
Client executables get stored in the clients folders

Server must be run before client.
To run server and client executables cd into the relevant folder and execute the commands:

./server <port #>
	port # is the port number the server will listen for connection on.

./client <client name> <server ip> <server port#> <path to shared directory>

Instead of providing a list of files in the shareddirectory folder, 
you can simply provide the path to the shared directory folder and the 
client will take care of the rest.


In this implementation, clients are able to connect to the server via TCP 
and execute the 
list, 
sendmyfileslist, 
get <filename>, and 
exit 
commands. 
The client can handle only one file request at a time and will message 
other clients if it is busy at the current time.
Clients can send and receive secure TCP messages and send files to each other.
The server will correctly route all file requests and receive to the 
correct clients.


Previous stuff from provided code:
The client will send its file list to the server every 3 minutes using 
a sigalarm. Since, there is a very very small chance that the client will 
get hung up in the sigalarm this is close to no chance that the server 
will not hear from the client for 5 minutes. Therefore, there is really 
no need to have the server check on the client every 5 minutes in this 
implementation.

In addition, if a client closes the socket with the server or exits, it 
will be removed from the servers connection list. 

If the server exits, all clients will exit as well, since they will 
sense this on the socket.

The server is not threaded in this implementation and handles requests 
as they come in. However, the client is threaded in this implementation. 
There is a thread waiting to receive commands from the server and a 
thread waiting to receive commands from the user typing into the command 
line. When a client is sending or receiving a file to/from another 
client, it will spawn a third thread that will handle the transfer. 
