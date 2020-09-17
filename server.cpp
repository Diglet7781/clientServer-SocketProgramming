// Name: KIRAN THAPA
// Student ID: 904-425-738

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 1)
	{
		cout << "Port Number must be passed" << endl;
		exit(EXIT_FAILURE);
	}
	int PORT = atoi(argv[1]);
	//create File to store dataReceived
	char filename [] = "dataReceived.txt";
	fstream dataReceived;

	dataReceived.open(filename, std::fstream::app);
	//if the  file does not exit create a new one
	if (!dataReceived)
	{
		dataReceived.open(filename, fstream::app);
	}
	//create a socket
	int handshake;
	int randomPort;
	int receivedPortInfo;
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1 )
	{
		cout << " Failed to create socket \n";
		return -1;
	}
	//Bind the socekt to a  IP / port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

	if(bind(socket_fd, (sockaddr *)&hint, sizeof(hint)) == -1)
	{
		cout << "cannot bind to IP/port";
		return -2;
	}

	//Mark the socket  for listening in
	if (listen(socket_fd, SOMAXCONN) == -1)
	{
		cout  << " failed at listening stage";
		return -3;
	}

 	//Accept a call
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);

	int clientSocket = accept(socket_fd, (sockaddr *)&client, &clientSize);

	if (clientSocket == -1)
	{
		cout << "Problem  connecting with client!";
		return -4;
	}

	close(socket_fd);
	receivedPortInfo = recv(clientSocket, &handshake, sizeof handshake, 0);
	cout << "handshake msg  received: " << handshake << endl;

	//select a random port number between  [1024, 65535]
	srand(time(0));
	randomPort =  (rand() % 65535 ) + 1025;
	cout << "Selected the random port " << randomPort << endl;
	//send the random port number to client
	send(clientSocket, &randomPort, sizeof randomPort, 0);

	//close  the listening socket
	close(clientSocket);

	//create a socket at the randomport for  receving data
	int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket < 0)
	{
		cout << "Probelem creating UDP socket" << endl;
		return -5;
	}
	//Forcefully attaching socket to the random port
	int opt = 1;
	if (setsockopt(udpSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		cout << "setsocket error UDP";
		return -6;
	}
	//change the port number for hint struct
	hint.sin_port = htons(randomPort);
	//hint.sin_addr.s_addr = INADDR_ANY;
	//bind the UDP socket
	if (bind(udpSocket, (const struct sockaddr *)&hint, sizeof hint) < 0 )
	{
		cout << "Binding error with UDP socket";
		return -7;
	}

	socklen_t len;
	char receivedPayload[5];
	int receivedPayloadLen;
	char sendPayloadACK[4];
	struct sockaddr_in clientAddress;
	//cleanup junk
	memset(receivedPayload, 0, sizeof receivedPayload);
	memset(sendPayloadACK, 0, sizeof sendPayloadACK);
	//clean up the clientAddress
	memset(&clientAddress, 0, sizeof clientAddress);
	len = sizeof(clientAddress);

	//here we receive the actual payload
	do{
	memset(receivedPayload, 0, 5);
	memset(sendPayloadACK, 0, 4);

	receivedPayloadLen = recvfrom(udpSocket, (char*)receivedPayload, 5, MSG_WAITALL, (struct sockaddr *)&clientAddress, &len);

	receivedPayload[receivedPayloadLen] = '\0';
//	cout << receivedPayloadLen << "payload size" << endl;
//	cout << "received payload : " << receivedPayload << endl;
	//remove the last 1
	char writeData[3];
	int j = 0;
	int iterate = 0;
	if (strlen(receivedPayload) < 5)
	{
		iterate = 2;

	}else {
		iterate = 4;
	}
	for (j ; j < iterate; j++)
	{
		writeData[j] = receivedPayload[j];
	}
	//write received data to file
	dataReceived.seekg(0, std::ios::end);
	//cout << "This is wrte Data: " << writeData << endl;
	dataReceived << writeData;
	memset(writeData, 0, sizeof writeData);
	int i = 0;
//	cout <<  "stlen receivedPayload : " << strlen(receivedPayload) << endl;
	for (i; i < strlen(receivedPayload)-1; i++)
	{
		sendPayloadACK[i] = toupper(receivedPayload[i]);
	}
	// send the CAPITALIZED payload as ACK
//	cout <<"sending payload all CAPS : " <<  sendPayloadACK << endl;
	sendto(udpSocket, (char*)sendPayloadACK, 4, MSG_CONFIRM, (const struct sockaddr *)&clientAddress, len);

	}while(receivedPayload[4] == '1');

	cout <<  "Sucessfully data written to file dataReceived.txt" << endl;
	//close the UDP socket
	close(udpSocket);

	return 0;
}


