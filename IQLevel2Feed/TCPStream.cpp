#include "TCPStream.h"
#include <iostream>

char MSG_DELIMITER = '\n\r';

TCPStream::TCPStream(SOCKET socket, sockaddr_in address, bool verbose)
{
	m_verbose = verbose;
	m_socket  = socket;
	m_address = address;
}

TCPStream::~TCPStream()
{
}

/* void TCPStream::close_stream()
	
	Gracefully closes down everything

*/
void TCPStream::close_stream()
{
	closesocket(m_socket);
	WSACleanup();
}

void TCPStream::set_protocol(std::string protocol_value)
{
	std::string sProtocolRequest = ("S,SET PROTOCOL," + protocol_value + "\r\n").c_str();
	send_request(sProtocolRequest);
}

/* void TCPStream::send_request(std::string symbol)

	Market Depth & NASDAQ Level 2 via TCP/IP

	Request Commands:
	------------------

	------------------------------------------------------------------------------------------------------------
	w[SYMBOL]<CR><LF>
	Begins watching a symbol for updates.
	[SYMBOL] - The symbol that you wish to receive updates on

	Result:
	You will receive a series of Summary ('Z') messages. You will receive one message for each
	current market maker (Nasdaq Level 2 if watching equities) or Level of Market Depth (if watching Futures).
	This series of messages will represent a snapshot of the current data available for the symbol.
	Following this, you will immediately start to receive Update ('2') messages as updates arrive from the
	exchange. Additionally, the message format has changed slightly.

	------------------------------------------------------------------------------------------------------------
	m[MMID]<CR><LF>
	Requests a Market Maker's name by its Market Maker ID.
	[MMID] - Returns the full name/description for a given market maker MMID.

	------------------------------------------------------------------------------------------------------------
	r[SYMBOL]<CR><LF>
	Terminates updates for the symbol specified.
	[SYMBOL] - The symbol that you wish to terminates updates on




*/
int TCPStream::send_request(std::string symbol)
{
	return send(m_socket, symbol.c_str(), strlen(symbol.c_str()), 0);
}

/*std::vector<std::string> TCPStream::tokenize_message(std::string const &in, char delimiter)
	
	Tokenizes input string based on delimiter into a vector of strings.

*/
std::vector<std::string> TCPStream::tokenize_message(std::string const &in, char delimiter)
{
	std::string::size_type b = 0;
	std::vector<std::string> result;

	while ((b = in.find_first_not_of(delimiter, b)) != std::string::npos)
	{
		auto e = in.find_first_of(delimiter, b);
		result.push_back(in.substr(b, e - b));
		b = e;
	}
	return result;
}

/*void TCPStream::ProcessMessage(std::vector<std::string>& msg_content)

	Method that processes method, intended to be overridden by a derived class.

*/
void TCPStream::ProcessMessage(std::vector<std::string>& msg_content)
{
	for (const auto &item : msg_content)
	{
		std::cout << item.c_str() << "!\n";
	}
}

/* void TCPStream::run()

	Market Depth & NASDAQ Level 2 via TCP/IP

	Result Format:
	---------------

	Data comming from the feed is a stream of comma delimited strings. Each message type can be identified by the single
	character in the first field of the comma delimited message.

	NOTE: <CR><LF> represents a terminating character.
	NOTE: for Market Depth data the [MMID] will be in the form MDxx where xx is a number indicating the "depth" of the
	data the update message represents.
	NOTE: for Market Depth data, the [Condition Code] field may be populated but are currently unused and should not be interpreted.

	------------------------------------------------------------------------------------------------------------
	Z,[SYMBOL],[MMID/DEPTH LEVEL],[FIELD 1],[FIELD 2], ... ,[FIELD N],<CR><LF>
	A summary message. See complete message definition in Level 2 Update/Summary messages.
	[SYMBOL]           - The symbol which generated this message.
	[MMID/DEPTH LEVEL] - The Market Maker ID or the Depth level which generated this message.
	[FIELD 1]          - The first data field for this message.
	[FIELD 2]          - The second data field for this message.
	... - The ... here represents the rest of the data fields for this message between the second and last fields.
	[FIELD N]          - The last data field for this message.

	------------------------------------------------------------------------------------------------------------
	2,[SYMBOL],[MMID/DEPTH LEVEL],[FIELD 1],[FIELD 2], ... ,[FIELD N],,<CR><LF>
	An update message. See complete message definition in Level 2 Update/Summary messages.
	[SYMBOL]           - The symbol which generated this message.
	[MMID/DEPTH LEVEL] - The Market Maker ID or the Depth level which generated this message.
	[FIELD 1]          - The first data field for this message.
	[FIELD 2]          - The second data field for this message.
	... - The ... here represents the rest of the data fields for this message between the second and last fields.
	[FIELD N]          - The last data field for this message.

	------------------------------------------------------------------------------------------------------------
	n,[SYMBOL]<CR><LF>
	Symbol not found message.
	[SYMBOL] - The symbol which was not located.

	------------------------------------------------------------------------------------------------------------
	M,[MMID/DEPTH LEVEL],[DESCRIPTION]<CR><LF>
	A Market Maker name query response message.
	[MMID/DEPTH LEVEL] - The requested Market Maker ID or Depth Level.
	[DESCRIPTION]      - The description of the Market Maker or Depth Level.

	------------------------------------------------------------------------------------------------------------
	E,[ERROR TEXT]<CR><LF>
	An error message.
	[ERROR TEXT] - The error description.

	------------------------------------------------------------------------------------------------------------
	O<CR><LF>
	Depeprecated message that is included only for backwards compatability

	------------------------------------------------------------------------------------------------------------
	T,[TimeStamp]<CR><LF>
	Receives a time stamp every second.
	[Timestamp] - The timestamp in the format CCYY-MM-DD HH:MM:SS

*/
void TCPStream::run()
{
	size_t buffer_used = 0;
	std::string message;

	while (true)
	{
		int bytesReceived = recv(m_socket, (char*)&m_buffer[buffer_used], BUFFER_SIZE - buffer_used, 0);
		buffer_used      += bytesReceived;

		if (m_verbose)
		{
			std::cout << "------ RECEIVED " << bytesReceived << " BYTES ------" << std::endl;
		}
		char* line_start = m_buffer;
		char* line_end;
		while ((line_end = (char*)memchr(m_buffer, MSG_DELIMITER, buffer_used)) != NULL)
		{
			*line_end = 0;

			message = std::string(line_start, line_end - 1);
	
			// Split message into a vector
			std::vector<std::string> msg_content = tokenize_message(message, ',');

			ProcessMessage(msg_content);
			
			if (m_verbose)
			{
				std::cout << "PARSED MESSAGE: " << message << std::endl;
			}
			line_start = line_end + 1;
		}
		buffer_used -= line_start - m_buffer;
		memmove(m_buffer, line_start, buffer_used);

		if (m_verbose)
		{
			std::cout << "BUFFER USED AFTER PARSING: " << buffer_used << std::endl;

			if (buffer_used != 0)
			{
				std::cout << line_start << std::endl;
			}

		}
		if (m_verbose)
		{
			std::cout << "------ END OF MESSAGE ------" << std::endl;
		}
	};
}