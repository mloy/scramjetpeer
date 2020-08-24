#include <cstdlib>
#include <iostream>
#include <string>

#include "boost/asio.hpp"

#include "../lib/include/scramjetpeer/bufferedreader.h"

using MessageType = uint8_t;



int checkprotocolVersionBuffered(scramjetpeer::BufferedReader& bufferedReader)
{
	struct ProtocolVersionData {
		uint32_t major;
		uint32_t minor;
		uint32_t patch;
	};
	
	MessageType messageType = 0;
	ProtocolVersionData protocolVersion;
	
	bufferedReader.recv(&messageType, sizeof(messageType));
	bufferedReader.recv(&protocolVersion, sizeof(protocolVersion));


	if (messageType!=1) {
		std::cerr << "unexpected message type " << static_cast < int > (messageType) << std::endl;
		return -1;
	}
	
	std::cout << "major: " << protocolVersion.major << std::endl;
	std::cout << "minor: " << protocolVersion.minor << std::endl;
	std::cout << "patch: " << protocolVersion.patch << std::endl;
	return 0;

}


int checkprotocolVersionGathered(boost::asio::ip::tcp::socket& socket)
{
	struct ProtocolVersionData {
		uint32_t major;
		uint32_t minor;
		uint32_t patch;
	};

	size_t reply_length;
	
	// gather both protocol parts (message type and version informartion) with one read
	MessageType messageType;
	ProtocolVersionData protocolVersion;


	std::array<boost::asio::mutable_buffer, 2> bufs = {
		boost::asio::buffer(&messageType, sizeof(messageType)),
		boost::asio::buffer(&protocolVersion, sizeof(protocolVersion)) };
	reply_length = socket.receive(bufs);
	
	if (messageType!=1) {
		std::cerr << "unexpected message type" << std::endl;
		return -1;
	}
	
	std::cout << "major: " << protocolVersion.major << std::endl;
	std::cout << "minor: " << protocolVersion.minor << std::endl;
	std::cout << "patch: " << protocolVersion.patch << std::endl;
	return 0;
}


/// connect to a scramjet daemon, receive and print the protocol version and disconnect
/// \warning we expect do run on a little endian machine!
int main(int argc, char* argv[])
{
	// some defaults that may be overriden by parameter <address>:<port>
	std::string address = "localhost";
	std::string port = "12345";

	if (argc>1) {
		std::string s = argv[1];
		auto delimiterPos = s.find(':');
		address = s.substr(0, delimiterPos);
		if (delimiterPos!=std::string::npos) {
			port = s.substr(delimiterPos+1);
		}
	}

	std::cout << address << ":" << port << std::endl;
	
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::resolver r(io_context);
	boost::asio::ip::tcp::socket socket(io_context);


	boost::asio::ip::tcp::resolver::results_type endpoints = r.resolve(address, port);
	boost::asio::connect(socket, endpoints);

	//checkprotocolVersionGathered(socket);
	
	scramjetpeer::BufferedReader bufferedReader(socket);
	
	checkprotocolVersionBuffered(bufferedReader);
	
	
	socket.close();
	
	return EXIT_SUCCESS;
}
