#pragma once
#include "../cpull_stremreader.h"

class PanoramicTFTPStreamreader : public CPullStreamreader
{
public:
	explicit PanoramicTFTPStreamreader(int model, const std::string& ip, int client_num);
	~PanoramicTFTPStreamreader();


	virtual void setStreamParams(StreamParams newParam);

protected:
	virtual const char* getImageFromCam(unsigned long& size, long IsPercent, int* Ifarme, StreamParams& param);
	int getSensorNumber();
	void atVeryBegining();
	void refreshSensorSize(StreamParams& param);

	void setDNMode(DN_modes mode){};// do not have panoramic dn 
	void setLightHz(Light_modes mode);
	void setCaptureEnable(bool enable){};
	void setCaptureTrigger(unsigned short trigger){};

private:
	void setQuality(int q);
	void setResolution(IMAGE_RESOLUTION res);
	void setBitrate(int br);
	void setResolution(int width_, int height_);

protected:
	int m_client_num;
	bool m_inited;

	

};