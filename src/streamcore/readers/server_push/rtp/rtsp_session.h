#pragma once
#include <string>
#include "../../../../network/socket.h"

class RTSPSession
{
public:
	RTSPSession();
	~RTSPSession();
	void openStream(const std::string& url, const std::string& ip, unsigned short port);
	bool isOpened() const;
	void closeStream();
	unsigned int getRTPport() const;
private:
    TCPSocket m_sock;
	bool m_connected;
};