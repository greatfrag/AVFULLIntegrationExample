#pragma once
#include <string>
#include "../cpull_stremreader.h"



//single sensor HTTP reader
class SSHTTPStreamreader : public CPullStreamreader
{
public:
	explicit SSHTTPStreamreader(int model, const std::string& ip);
	~SSHTTPStreamreader();

protected:
	virtual const char* getImageFromCam(unsigned long& size, long IsPercent, int* Ifarme, StreamParams& param);
	int getSensorNumber(){return 0;}; // single sensor camera has only one sensor
	void refreshSensorSize(StreamParams& param);

	virtual void setDNMode(DN_modes mode);
	virtual void setLightHz(Light_modes mode);
	virtual void setCaptureEnable(bool enable){};
	virtual void setCaptureTrigger(unsigned short trigger){};

private:

protected:
	char* m_data;


};
