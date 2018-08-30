#include "TCPConnector.h"
#include "TCPStream.h"
#include <iostream>
#include <string>

template<class T>
TCPConnector<T>::TCPConnector()
{
}

template<class T>
TCPConnector<T>::~TCPConnector()
{
}

/* TCPStream* TCPConnector::create_stream(int port, std::string ipAddress)

	call takes a server port and IP address string as arguments.

	EXAMPLE:
	==========

	std::string ipAddress = "127.0.0.1";    // IP Address of the server
	int port              = 9200;			// Listening port # on the server


*/
template<class T>
T* TCPConnector<T>::create_stream(int port, std::string ip_address,bool verbose)
{

	// Initialize WinSock
	WSAData data;
	WORD    ver  = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		return NULL;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return NULL;
	}

	// Fill in a hint structure
	sockaddr_in        address;
	address.sin_family = AF_INET;
	address.sin_port   = htons(port);
	inet_pton(AF_INET, ip_address.c_str(), &address.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&address, sizeof(address));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return NULL;
	}

	return new T(sock, address, verbose);
}