#include "stdafx.h"
#include "./rtsp_session.h"
#include <sstream>

using namespace std;


RTSPSession::RTSPSession()
{
	m_connected = false;
}

RTSPSession::~RTSPSession()
{
	closeStream();
}

void RTSPSession::openStream(const std::string& url, const std::string& ip, unsigned short port)
{
	try
	{
		m_sock.connect(ip, port);
		ostringstream os;
	}
	catch (...) 
	{
		m_connected = false;
	}
}
bool RTSPSession::isOpened() const
{
	return m_connected;
}

void RTSPSession::closeStream()
{
	m_connected = false;
}


unsigned int RTSPSession::getRTPport() const
{
	return 0;
}
