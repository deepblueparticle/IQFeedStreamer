// Test.cpp : Defines the entry point for the console application.
//

#include "TCPConnector.h"
#include "TCPStream.h"
#include "TCPPublisher.h"
#include <thread>
#include <iostream>

void main()
{

	std::string ipAddress = "127.0.0.1"; // IP Address of the server
	int port              = 9200;		 // Listening port # on the server
	bool verbose          = true;        // Should print stuff on the console?

	// Create Winsock Connector (factory)
	TCPConnector<TCPPublisher> Connector;

	// Stream
	auto* Stream = Connector.create_stream(port, ipAddress, verbose);

	// Set IQFeed Protocol - 6.0 for microsecond timestamping
	std::string most_recent_protocol = "6.0";
	Stream->set_protocol(most_recent_protocol);

	if (dynamic_cast<TCPPublisher*>(Stream))
	{
		Stream->bind("tcp://*:5563");
	}

	// Send request for market data
	Stream->send_request("w@ESU18\r\n");
	Stream->send_request("w@ESU18\n\r");
	Stream->send_request("w@ESH18\n\r");
	Stream->send_request("w@ESM19\n\r");

	Stream->send_request("w@NQU18\n\r");
	Stream->send_request("w@NQZ18\n\r");
	Stream->send_request("w@NQH19\n\r");


	Stream->send_request("wQHOZ18\n\r");
	Stream->send_request("wQHOF19\n\r");
	Stream->send_request("wQCLZ18\n\r");
	Stream->send_request("wQCLF19\n\r");
	Stream->send_request("wQCLH19\n\r");
	Stream->send_request("wQCLM19\n\r");
	Stream->send_request("wQRBZ18\n\r");
	Stream->send_request("wQRBF19\n\r");
	

	std::thread t(&TCPStream::run, Stream);

	t.join();
	

	std::cin.get();

}

