# clientServer-SocketProgramming
This client server application will use linux sockets to transfer a text file from client to server.
The client initiates a handshake over TCP/IP protocal and sends a random port for transfering data to server
Server will acknowledge the handshake , and creates a UDP socket to receive the data at the agreed port

steps:
1. make
2. ./server 8080(user defined port)
2. ./client localhost 8080(server Port) test.txt(user defined text file)
