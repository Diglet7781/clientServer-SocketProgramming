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
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
	//create a sock
	int handshake = 2020;
	int randomPort;
	int receiveRandomPort;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		return -1;
	}

	//create a hint structure for the server we are connecting
	int PORT  = atoi(argv[2]);
	
	//get the hostname
	char *hostname = argv[1];
	char ip[100];

	struct hostent *host;
	struct in_addr **addr_info;
	int i;
		
	if ( (host = gethostbyname( hostname ) ) == NULL) 
	{
		// get the host info
		herror("gethostbyname");
		exit(EXIT_FAILURE);
	}

	addr_info = (struct in_addr **) host->h_addr_list;
	
	for(i = 0; addr_info[i] != NULL; i++) 
	{
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_info[i]) );
	}
	
	struct sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	inet_pton(AF_INET, ip, &hint.sin_addr);

	//connect to ther server on the socket
	int connection = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if( connection == -1)
	{
		return -1;
	}

	//send the handShake message to server
	send(sock, &handshake, sizeof handshake, 0);
	cout << "handshake message sent :"  << handshake << endl;
	
	//receive the randomport to send data  from server
	receiveRandomPort = recv(sock, &randomPort, sizeof randomPort, 0);
	//cout << "received random port number " << randomPort << endl;
	//close clientSocket
	close(sock);

	//client creates the UDP socket to the server in the port received and send data
	int udpSocket;
	//create udpSocket file descriptor
	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket < 0 )
	{
		cout << "Problem creating UDP socket";
		return -2;
	}
	//change the port number for hint struct
	hint.sin_port = htons(randomPort);
	hint.sin_addr.s_addr = INADDR_ANY;
	
	//send the data to server
	//read  clientData.txt file
	char ch;
	int receivedDataSize;
	char payload[4];
	char payloadACK[4];
	int payloadSizeCount = 0;
	socklen_t len; //len of server
	len = sizeof hint;
	fstream readFile(argv[3]);
	
	//cleaning the payload and payloadACK 
	memset(payload, 0, 5);
	memset(payloadACK, 0, 5);

	while (readFile >> noskipws >> ch)
	{
		payload[payloadSizeCount] = ch;
		payloadSizeCount += 1;
		if(payloadSizeCount == 4 || readFile.peek() == EOF)
		{
			if(readFile.peek() == EOF)
			{
				payload[payloadSizeCount] = '0';
			}
			else
			{
				payload[payloadSizeCount] = '1';
			}
			//cout << payload << endl;
			//send the payload to server
			sendto(udpSocket, (const char *)payload, 5, MSG_CONFIRM, (const struct sockaddr *)&hint, sizeof hint);
			//wait for ACK and print ACK to screen
			recvfrom(udpSocket, (char*)payloadACK, 4, MSG_WAITALL, (struct sockaddr*)&hint, &len);
			//print the ACK payload
			cout << payloadACK << endl;
			//clear pay load and ACK
			memset(payload, 0, 5);
			memset(payloadACK, 0, 5);
			payloadSizeCount = 0;
		}
		if (readFile.peek() == EOF)
		{
			break;
		}
	}
	readFile.close();
	//close udp socket
	close(udpSocket);

	return 0;
}

