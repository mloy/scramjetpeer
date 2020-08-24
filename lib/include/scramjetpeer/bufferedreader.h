// Copyright 2014 Hottinger Baldwin Messtechnik
// Distributed under MIT license
// See file LICENSE provided

#ifndef _SCRAMJETPEER__BUFFEREDREADER_H
#define _SCRAMJETPEER__BUFFEREDREADER_H

#include <boost/asio.hpp>

#include <cstddef>

namespace scramjetpeer {
	/// Try to receive a big chunk even if only a small amount of data is requested.
	/// This reduces the number of system calls being made.
	/// Return the requested amount of data and keep the remaining data.
	/// \warning not reentrant
	class BufferedReader
	{
	public:
		BufferedReader(boost::asio::ip::tcp::socket& socket);

		/// behaves like standard recv
		ssize_t recv(void *buf, size_t len);

	private:
		BufferedReader(const BufferedReader& op);
		BufferedReader& operator=(const BufferedReader& op);

		boost::asio::ip::tcp::socket& m_socket;
		unsigned char m_buffer[65536];
		size_t m_fillLevel;
		size_t m_alreadyRead;
	};
}
#endif // BUFFEREDREADER_H
