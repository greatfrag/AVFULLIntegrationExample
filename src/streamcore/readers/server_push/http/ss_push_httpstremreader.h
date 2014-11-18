#pragma once
#include <string>
#include "../spush_stremreader.h"

class SimpleHTTPClient;
//single sensor HTTP reader


class SSHTTPPushStreamreader : public SPushStreamreader
{
public:
	explicit SSHTTPPushStreamreader(int model, const char* ip);
	~SSHTTPPushStreamreader();

protected:
	int getSensorNumber(){return 0;}; // single sensor camera has only one sensor

	const char* getNextImageFromCam(unsigned long& size, int *Iframe );
	void openStream(StreamParams& params);
	void closeStream();
	bool isStreamOpened();
	void refreshSensorSize(StreamParams& param);
	void setDNMode(DN_modes mode);
	void setLightHz(Light_modes mode);

protected:
	char* m_data;
	std::string m_cam_ip;

	char* rest;
	int rest_len;
	
	SimpleHTTPClient *m_http_client;

};

