#pragma once
#include <vector>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 65536

class TCPStream
{
public:

	 TCPStream(SOCKET socket, sockaddr_in address, bool verbose);
	~TCPStream();

	void close_stream();
	void set_protocol(std::string value);
	int  send_request(std::string request);
	void run();

private: 

	std::vector<std::string> tokenize_message(std::string const &in, char sep);
	virtual void ProcessMessage(std::vector<std::string>& msg_content);

private:
	bool        m_verbose;
	SOCKET      m_socket;
	sockaddr_in m_address;
	char        m_buffer[BUFFER_SIZE];
};

