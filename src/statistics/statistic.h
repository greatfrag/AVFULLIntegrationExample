#ifndef statistic_h_501
#define statistic_h_501

#include <string>

class Statistic
{
public:
	void start();
	void stop();

	bool Update();
	
	void addData(int dataSize);
	double getBitrate() const; // in megabits per second
	double getFps() const;
	std::string ToString();



private:
	
	unsigned int m_frames;
	double m_fps;
	double m_bitrate;
	unsigned long m_t0;
	unsigned int m_bytes;

};


#endif //statistic_h_501