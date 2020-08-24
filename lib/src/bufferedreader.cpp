// Copyright 2014 Hottinger Baldwin Messtechnik
// Distributed under MIT license
// See file LICENSE provided



#include <cstring>

#include <boost/asio.hpp>

#include "scramjetpeer/bufferedreader.h"

namespace scramjetpeer {
	BufferedReader::BufferedReader(boost::asio::ip::tcp::socket& socket)
		: m_socket(socket)
		, m_fillLevel(0)
		, m_alreadyRead(0)
	{
	}

	ssize_t BufferedReader::recv(void *buf, size_t desiredLen)
	{
		// check whether there is something left
		size_t bytesLeft = m_fillLevel - m_alreadyRead;

		if(bytesLeft>=desiredLen) {
			// there is more than or as much as desired
			memcpy(buf, m_buffer+m_alreadyRead, desiredLen);
			m_alreadyRead += desiredLen;
			return static_cast < ssize_t > (desiredLen);
		} else if(bytesLeft>0) {
			// return the rest which is less than desired (a short read)
			memcpy(buf, m_buffer+m_alreadyRead, bytesLeft);
			m_alreadyRead = m_fillLevel;
			return static_cast < ssize_t > (bytesLeft);
		}

		// try to read as much as possible into the provided buffer. In addition we fill our internal buffer if there is already more to read.
		// readv saves us from reading into the internal buffer first and copying into the provided buffer afterwards.
		std::array<boost::asio::mutable_buffer, 2> bufs = {
			boost::asio::buffer(buf, desiredLen),
			boost::asio::buffer(&m_buffer, sizeof(m_buffer)) };
		ssize_t retVal = m_socket.receive(bufs); // receive delivers at least one byte!
		
		m_alreadyRead = 0;

		if (retVal>static_cast < ssize_t > (desiredLen)) {
			// readv returns the total number of bytes read
			m_fillLevel = static_cast < size_t > (retVal)-desiredLen;
			return static_cast < ssize_t > (desiredLen);
		}
		m_fillLevel = 0;
		return retVal;
	}
}
