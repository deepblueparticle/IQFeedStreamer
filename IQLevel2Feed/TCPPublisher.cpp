#include "TCPPublisher.h"
#include <iostream>
#include "ZeroMQ/zmsg.h"
#include "ZeroMQ/zmq_addon.hpp"

enum MSG_SIZES : int
{
	SUMMARY = 14
};

TCPPublisher::TCPPublisher(SOCKET iq_socket, sockaddr_in iq_address, bool verbose): TCPStream(iq_socket,iq_address,verbose)
{
	m_verbose  = verbose;
	m_context  = new zmq::context_t;
	m_pub_sock = new zmq::socket_t(*m_context, ZMQ_PUB);
	m_message  = new zmsg();
}

TCPPublisher::~TCPPublisher()
{

}

/* void TCPPublisher::bind(std::string endpoint)

	  Binds publisher to an endpoint

*/
void TCPPublisher::bind(std::string endpoint)
{
	m_endpoint = endpoint;
	m_pub_sock->bind(m_endpoint.c_str());
	s_console("I: IQFeed publisher/0.1.1 is active at %s", endpoint.c_str());
}

void TCPPublisher::publish_summary_message(std::vector<std::string> & msg_content)
{
	if (msg_content.size() == MSG_SIZES::SUMMARY)
	{
		for (int i = 0; i < MSG_SIZES::SUMMARY; i++)
		{
			m_message->push_back((char*)msg_content[i].c_str());
		}
		m_message->send(*m_pub_sock);
	}
	else
	{
		if (m_verbose)
		{
			std::cout << "ERROR WHILE PUBLISHING SUMMARY MESSAGE" << std::endl;
		}
	}
	m_message->clear();
}

void TCPPublisher::publish_update_message(std::vector<std::string> & msg_content)
{
	publish_summary_message(msg_content);

}
void TCPPublisher::ProcessMessage(std::vector<std::string>& msg_content)
{
	std::string sMessageType = msg_content[0];

	if (sMessageType.compare("Z") == 0) // Summary message
	{
		publish_summary_message(msg_content);
	}
	else if (sMessageType.compare("2") == 0) // An update message
	{
		publish_update_message(msg_content);
	}
	else if (sMessageType.compare("n") == 0) // Symbol not found message
	{

	}
	else if (sMessageType.compare("M") == 0) // A Market Maker name query response message
	{

	}
	else if (sMessageType.compare("E") == 0)  // An error message
	{

	}
	else if (sMessageType.compare("O") == 0) // Depeprecated message that is included only for backwards compatability
	{

	}
	else if (sMessageType.compare("T") == 0) // Timestamp
	{

	}
};