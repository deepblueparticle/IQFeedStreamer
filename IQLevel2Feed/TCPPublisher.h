#pragma once
#include "TCPStream.h"
#include <vector>
#include <string>
#include "ZeroMQ\zmq.hpp"
#include "ZeroMQ\zmsg.h"

class TCPPublisher: public TCPStream
{
public:
	
	 TCPPublisher(SOCKET socket, sockaddr_in address, bool verbose);
	~TCPPublisher();

public:
	
	void ProcessMessage(std::vector<std::string>& msg_content);
	void bind(std::string endpoint);

private:
	void publish_summary_message(std::vector<std::string>& msg_content);
	void publish_update_message(std::vector<std::string>& msg_content);

private:

	bool            m_verbose;
	zmq::context_t* m_context;
	zmq::socket_t * m_pub_sock;
	std::string     m_endpoint;
	zmsg *          m_message;
};

