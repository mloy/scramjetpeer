#include <cstdlib>
#include <iostream>
#include <string>

#include "boost/asio.hpp"


// connect to a scramjet daemon, receive and print the protocol version and disconnect
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

	size_t reply_length;


	// now read ProtocolVersion
	#pragma pack(push, 1)
	struct ProtocolVersionData {
		uint8_t messageType;
		uint32_t major;
		uint32_t minor;
		uint32_t patch;
	};
	#pragma pack(pop)
	
	std::vector < ProtocolVersionData > data(1);

	auto receiveBuffer = boost::asio::buffer(data);
	reply_length = boost::asio::read(socket, receiveBuffer);
	if (data[0].messageType!=1) {
		std::cerr << "unexpected message type" << std::endl;
	}
	
	std::cout << "major: " << data[0].major << std::endl;
	std::cout << "minor: " << data[0].minor << std::endl;
	std::cout << "patch: " << data[0].patch << std::endl;
	
	return EXIT_SUCCESS;
}
