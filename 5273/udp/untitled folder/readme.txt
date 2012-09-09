Anne Gatchell
Programming Assignment #1
Networking Systems

7 Sept 2012


This is a UDP socket program.

There are files for a server and a client. They both set up a datagram socket.
The client prompts the user for commands, which can either send a file to the
server, list the files in the server's current directory, or tell the server
to exit. Undesignated commands will simply prompt a repeat reply from the server.

For sending files, the user types put <filename>.
The client checks to see that the filename exists before sending the input message to
the server to let it know that a file is coming.
The server sends back a response. When the client receives this response, it opens
the file, reads it into a 1024 char buffer, and sends it out in packets to the
server.
When the client is done reading and sending the file, it sends a message of 'e' to 
the server.

The client can also request a list of files on the server. The server looks in its
current directory, appends each filename to a path string, and sends it to the client.

The client can tell the server to exit. The server sends a closing message to the
client, closes its socket, and exits.