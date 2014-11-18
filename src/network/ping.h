#ifndef ping_1750
#define ping_1750

#define MAX_PACKET       1024      // Max ICMP packet size
#include <string>


class Ping
{
public:
	bool ping(const std::string& ip, int retry, int timeout, int pack_size);
private:

	char icmp_data[MAX_PACKET];
	char recvbuf[MAX_PACKET];

};

#endif //ping_1750