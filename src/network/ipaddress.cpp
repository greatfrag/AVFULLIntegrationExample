#include "ipaddress.h"

using namespace std;
bool IpAddress::initialized = false;
IpAddressList IpAddress::addrlist ;
#define MAXHOSTNAMELEN 255

IpAddress::IpAddress(const IpAddress& rip)
{
	this->m_addr = rip.m_addr;
}


IpAddress::IpAddress(const std::string& ip)
{
	m_addr.s_addr =  inet_addr(ip.c_str());
}


IpAddress::IpAddress(const unsigned char* ip)
{
	memcpy(&m_addr, ip, getAddressLen());
}

const unsigned char* IpAddress::getAddress()
{
	return (const unsigned char*)(&m_addr);
}

unsigned int IpAddress::getAddressLen()
{
	return sizeof(in_addr);
}

std::string IpAddress::toString() const
{
	return string(::inet_ntoa(m_addr));
}

IpAddressList IpAddress::getLocalIpAddresses(bool os)
{
	if (!os)
		return addrlist;

	if (!initialized)
	{
		WORD wVersionRequested;
		WSADATA wsaData;

		wVersionRequested = MAKEWORD(1, 1);
		int rc = ::WSAStartup(wVersionRequested, &wsaData);

		if(rc != 0) 
			return addrlist;

	}

	initialized = true;

	const int buflen = MAXHOSTNAMELEN + 1;
	char buffer[buflen];
	if(::gethostname(buffer, buflen) < 0)
		return addrlist;


	hostent *phe = gethostbyname(buffer);
	if (phe == 0) 
		return addrlist;

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) 
	{
		IpAddress ipaddr;
		memcpy(&ipaddr.m_addr, phe->h_addr_list[i], sizeof(struct in_addr));

		string t = ipaddr.toString();
		addrlist.push_back(ipaddr);
	}


		
	return addrlist;
}

//===========================================================
