#include "stdafx.h"
#include ".\simple_tftp_client.h"

using namespace std;

SimpleTFTPClient::SimpleTFTPClient(const std::string& ip, unsigned int timeout, unsigned int retry):
m_retry(retry),
m_ip(ip),
m_timeout(timeout)
{
	
}

int SimpleTFTPClient::read( const std::string& fn, char* data)
{
	char buff_send[1000]; int len_send;
	char buff_recv[30000]; int len_recv;
	int i, len = 0;

	UDPSocket m_sock;
	m_sock.setTimeOut(m_timeout);

	len_send = form_read_request(fn, buff_send);

	string temp_cam_addr;
	unsigned short cam_dst_port;


	for (i = 0; i < m_retry; ++i)
	{
	
		try{m_sock.sendTo(buff_send,len_send,m_ip,69);}	catch (SocketException &e){return 0;};

		

		while(1)
		{
			try
			{
				len_recv = 0;
				len_recv = m_sock.recvFrom(buff_recv, sizeof(buff_recv),temp_cam_addr, cam_dst_port);
			}
			catch (SocketException &e)	{
				m_status = time_out; break;
			}// did not get anything 

			if (len_recv<6) // unexpected answer 
				continue;

			if (buff_recv[0]==0 && buff_recv[1]==0x06)// this option ack
			{	
				m_status = all_ok;
				break;
			}
		}

		if (m_status==all_ok)
			break;		
	}

    if (m_status!=all_ok)// no response
		return 0;

	//unsigned short dstport =  m_sock.getForeignPort();

	// we've got option ack; => must send ack

	int blk = 0;

	while(1)
	{		
		bool last_packet = false;

		len_send = form_ack(blk, buff_send);

		for (i = 0; i < m_retry; ++i)
		{
			try{m_sock.sendTo(buff_send,len_send,m_ip,cam_dst_port);}	catch (SocketException &e){return 0;};


			while(1)
			{
				try
				{
					len_recv = 0;
					len_recv = m_sock.recv(buff_recv, sizeof(buff_recv));
				}
				catch (SocketException &e)	{
					m_status = time_out; break;
				}// did not get anything 

				if (len_recv<4) // unexpected answer
					continue;


				if (buff_recv[0] == 0 && buff_recv[1] == 3)// data block
				{
					int new_blk = buff_recv[2]*256 + buff_recv[3];
					if (new_blk!= blk+1) // not our data block
						continue;


					int data_len = len_recv-4;
					if (data_len<1450)
						last_packet = true;

					memcpy(data + len, buff_recv+4, data_len);
					len+=data_len;

					m_status = all_ok;
					blk++;
					break;
				}
			}

			if (m_status==all_ok)
				break;		


		}

		if (m_status == time_out)
			return 0;
		
		if (last_packet)
			break;
	}

	// need to send ack to last pack;
	len_send = form_ack(blk, buff_send);
	m_sock.sendTo(buff_send,len_send,m_ip,cam_dst_port); // send ack

	return len;
}


bool SimpleTFTPClient::write_reg(int reg,  unsigned int value)
{
	char buff_send[1000]; int len_send;
	char buff_recv[30000]; int len_recv;
	int i, len = 0;

	UDPSocket m_sock;
	m_sock.setTimeOut(m_timeout);

	len_send = form_write_reg_request(buff_send);

	string temp_cam_addr;
	unsigned short cam_dst_port;


	for (i = 0; i < m_retry; ++i)
	{

		try{m_sock.sendTo(buff_send,len_send,m_ip,69);}	catch (SocketException &e){return 0;};

		while(1)
		{
			try
			{
				len_recv = 0;
				len_recv = m_sock.recvFrom(buff_recv, sizeof(buff_recv),temp_cam_addr, cam_dst_port);
			}
			catch (SocketException &e)	{
				m_status = time_out; break;
			}// did not get anything 

			if (len_recv<2) // unexpected answer 
				continue;

			if (buff_recv[0]==0 && buff_recv[1]==0x06)// this option ack
			{	
				m_status = all_ok;
				break;
			}
		}

		if (m_status==all_ok)
			break;		

	}

	if (m_status!=all_ok)// no response
		return false;

	// now we have to send the only data block itself
	buff_send[0] = 0; buff_send[1] = 3; // data pack
	buff_send[2] = 0; buff_send[3] = 1; // blk num
	buff_send[4] = reg; 
	buff_send[5] = value>>8; buff_send[6] = (value&0xff);
	len_send = 7;

	for (i = 0; i < m_retry; ++i)
	{

		try{m_sock.sendTo(buff_send,len_send,m_ip,cam_dst_port);}	catch (SocketException &e){return 0;};

		while(1)
		{
			try
			{
				len_recv = 0;
				len_recv = m_sock.recvFrom(buff_recv, sizeof(buff_recv),temp_cam_addr, cam_dst_port);
			}
			catch (SocketException &e)	{
				m_status = time_out; break;
			}// did not get anything 

			if (len_recv<4) // unexpected answer 
				continue;

			if (buff_recv[0]==0 && buff_recv[1]==0x04 && buff_recv[2]==0 && buff_recv[3]==0x01)// this option ack for block1
			{	
				m_status = all_ok;
				break;
			}
		}

		if (m_status==all_ok)
			break;		

	}

	if (m_status!=all_ok)// no response
		return false;


	return true;
}


int SimpleTFTPClient::form_read_request(const std::string& fn, char* buff)
{
	buff[0] = 0; buff[1] = 1; // read request;
	int len = 2;
	int req_len = fn.length();

	memcpy(buff+len, fn.c_str(), req_len);	len+=req_len;
	buff[len] = 0;	len++;

	memcpy(buff+len, "netascii", 8); len+=8;
	buff[len] = 0;	len++;

	memcpy(buff+len, "blksize", 7); len+=7;
	buff[len] = 0;	len++;

	memcpy(buff+len, "1450", 4); len+=4;
	buff[len] = 0;	len++;

	return len;

}

int SimpleTFTPClient::form_ack(unsigned short blk, char* buff)
{
	buff[0] = 0; buff[1] = 0x04; 
	buff[2] = blk>>8; buff[3] = blk&0xff;

	return 4;
}

int SimpleTFTPClient::form_write_reg_request(char* buff)
{
	buff[0] = 0; buff[1] = 2; // write request;
	int len = 2;

	memcpy(buff+len, "reg_set", 8); len+=7;
	buff[len] = 0;	len++;


	memcpy(buff+len, "netascii", 8); len+=8;
	buff[len] = 0;	len++;

	memcpy(buff+len, "blksize", 7); len+=7;
	buff[len] = 0;	len++;

	memcpy(buff+len, "1450", 4); len+=4;
	buff[len] = 0;	len++;

	return len;


}