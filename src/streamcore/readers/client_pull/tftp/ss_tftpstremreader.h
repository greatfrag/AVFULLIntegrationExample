#pragma once
#include "../cpull_stremreader.h"

//single sensor

class SSTFTPStreamreader : public CPullStreamreader
{
public:
	explicit SSTFTPStreamreader(int model, const std::string& ip, int client_num);
	~SSTFTPStreamreader();
	void setChannel(int channel ); // for panaramic H.264 only

protected:
	virtual const char* getImageFromCam(unsigned long& size, long IsPercent, int* Ifarme, StreamParams& param);
	int getSensorNumber(){return 0;}; // single sensor camera has only one sensor

	void atVeryBegining();
	void refreshSensorSize(StreamParams& param);
	
	void setDNMode(DN_modes mode);
	void setLightHz(Light_modes mode);
	void setCaptureEnable(bool enable);
	void setCaptureTrigger(unsigned short trigger);

private:

protected:
	int m_client_num;
	bool m_inited;
	
};