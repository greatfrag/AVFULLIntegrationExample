#ifndef __SIMPLE_HTTP_CLIENT__
#define __SIMPLE_HTTP_CLIENT__

#include "socket.h"



enum StatusCode
{
	Success = 200,
	NotFound = 404,
	Denied  = 401

};



class SimpleHTTPClient
{
public:	
	SimpleHTTPClient(const std::string& host, int port=80);
	~SimpleHTTPClient();

	void setRequestLine(const std::string& request){m_request = request;};

	void openStream();
	bool isOpened()const{return m_connected;} ;

	long read(char* data, unsigned long max_len);

	StatusCode getStatus()const {return m_statCode;} 
	std::string getContentType() const {return m_contenttype;}

	

private:

	std::string m_host;
	int m_port;
	std::string m_request;
	TCPSocket m_sock;

	std::string m_contenttype;
	StatusCode m_statCode;

	bool m_connected;
};


#endif //__SIMPLE_HTTP_CLIENT__
