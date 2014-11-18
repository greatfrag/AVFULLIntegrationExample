#pragma once

#include "../streamreader.h"

class CPullStreamreader : public Streamreader
{
public:
	CPullStreamreader(int model, const std::string& ip):Streamreader(model){m_cam_ip = ip;};
	~CPullStreamreader(){stop();}
protected:
	virtual const char* getImageFromCam(unsigned long& size, long IsPercent, int* Ifarme, StreamParams& param) = 0;
	bool needToRead() const;
private:
	void run(); // in a loop: takes images from camera and put into quue

protected:
	std::string m_cam_ip;
};