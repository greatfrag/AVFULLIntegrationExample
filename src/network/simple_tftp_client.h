#ifndef __SIMPLE_TFTP_CLIENT__1117
#define __SIMPLE_TFTP_CLIENT__1117
#include <string>
#include "socket.h"

// this class is designed just to be friendly with AV cams;

class SimpleTFTPClient
{

	enum {all_ok,  time_out};


public:
	SimpleTFTPClient(const std::string& ip, unsigned int timeout, unsigned int retry);
	~SimpleTFTPClient(){};
	int read(const std::string& fn, char* data);

	bool write_reg(int reg,  unsigned int value);

private:
	int form_read_request(const std::string& fn, char* buff);
	int form_ack(unsigned short blk, char* buff);

	int form_write_reg_request(char* buff);
private:

	
	int m_retry;
	int m_timeout;
	int m_status;
	std::string m_ip;

};


#endif //__SIMPLE_TFTP_CLIENT__1117
