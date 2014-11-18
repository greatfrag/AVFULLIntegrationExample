#ifndef IP_ADDR_1758
#define IP_ADDR_1758

#include <string>
#include <winsock.h>
#include <list>

class IpAddress;
typedef std::list<IpAddress> IpAddressList;

class IpAddress
{
public:
	IpAddress(const IpAddress& rip);

	//construct an InetAddress by performing a look-up of a host's name.	
	IpAddress(const std::string& ip);


	IpAddress(const unsigned char* ip);

	const unsigned char* getAddress(); 
	static unsigned int getAddressLen();


	//Returns the IP address in dotted decimal notation.
	//For example: "192.168.0.1"
	std::string toString() const;

	// gel local IP addresses ; could be more than one;
	// os = true; forme os; os = false => frome memory;  it takes time to get it frome os;
	static IpAddressList getLocalIpAddresses(bool os = false);


private:
	IpAddress(){ memset(&m_addr, 0 , sizeof(in_addr)); }; // must not be available;

private:
	in_addr m_addr;


	static bool initialized;
	static IpAddressList addrlist;

};



#endif //IP_ADDR_1758