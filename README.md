# CLIENT-SERVER TCP SOCKET PROGRAMMING

## Developed two network programs, one for client and one for server

- The client sends a file containing text content as defined in the project to the server.
- The server accepts the text and creates a new text file and saves it to the same file directory and sends the received text all in UPPERCASE as a form of Acknowledgement.
- The client prints the Acknowledgement received from the server.

# APPLICATION PROTOCAL

## Architecture

Client-server architecture is used for the network application. Server is given an IP address which does not change throughout the session. They need to establish connection before they can communicate. The Client makes requests services to the server and server responds to it.

## Types of Messages

- Request after a connection is established between the client and the server, client sends a request to the server.
- Response to answer to the request of a client, the server receives the data and starts to store the data into a text file and sends Acknowledgement response all UPPERCASE received text to client.

## Rules of sending messages
- Client always sends the request to the server.
- Server responds to the request from the client.
- There is always one response for one request.

# Instructions to compile client
The client can be compiled by the following command in the commandline:
:arrow_right: g++ client.cpp -o client
Where <client.cpp> is the client cpp file, <client> is the compiled client executable filename.

# Instructions to complile server
The Server can be compiled by the following command in the commandline:
:arrow_right: g++ server.cpp -o server
Where <server.cpp> is the server cpp file, <server> is the compiled server executable filename.

# Usage

## Once compiled run the following command in the commandline
- ./client fileName portNumber
- ./server portNumber