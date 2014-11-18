#include "stdafx.h"
#include ".\simple_http_client.h"

#include <sstream>

using namespace std;

SimpleHTTPClient::SimpleHTTPClient(const std::string& host, int port):
m_host(host),
m_port(port),
m_statCode(NotFound),
m_connected(false)
{
	
}

SimpleHTTPClient::~SimpleHTTPClient()
{

}

void SimpleHTTPClient::openStream()
{
	try
	{
		m_sock.connect(m_host, m_port);

		
		ostringstream os;
		os << "GET /" << m_request<<" HTTP/1.1" <<"\r\n"\
			<< "Host: "<< m_host << "\r\n"\
			<< "\r\n";

		m_sock.send(os.str().c_str(), os.str().length());
		

		// response 
		ostringstream response1;
		char prev = 0 , curr = 0;

		while(1) 
		{
			m_sock.recv(&curr,1);
			if (prev=='\r' && curr == '\n')
				break;
			response1 << curr;
			prev = curr;
		}

		string s_response_code = response1.str();

		if (s_response_code.find("200 OK")==string::npos)// not ok
		{
			m_connected = false;
			m_statCode = Denied;
			return ;
		}
		m_statCode = Success;
		

		//content type
		ostringstream response2;
		prev = 0 , curr = 0;

		while(1) 
		{
			m_sock.recv(&curr,1);
			if (prev=='\r' && curr == '\n')
				break;
			response2 << curr;
			prev = curr;
		}

		s_response_code = response2.str();
		int pos  = s_response_code.find("Content-Type:");

		if (pos==string::npos)// not good
		{
			m_connected = false;
			m_statCode = Denied;
			return;
		}
		

		m_contenttype= s_response_code.substr(pos + strlen("Content-Type:"), s_response_code.length()- (pos + strlen("Content-Type:") + 1 ) );
        		

        m_connected = true;
		m_statCode = Success;
	}
	catch (...) 
	{
		m_connected = false;
		m_statCode = NotFound;
	}
	
}

long SimpleHTTPClient::read(char* data, unsigned long max_len)
{
	if (!m_connected) return -1;

	int readed = 0;
	try
	{
		readed = m_sock.recv(data, max_len);
	}
	catch (...)
	{
		readed = -1;
	}
	
	if (readed<=0)	
		m_connected = false;

	return readed;
}

