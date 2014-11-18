#pragma once
#include "ss_push_httpstremreader.h"

//panoramic sensor HTTP reader

class PanoramicHTTPPushStreamreader : public SSHTTPPushStreamreader
{
public:
	PanoramicHTTPPushStreamreader(int model, const char* ip):
	  SSHTTPPushStreamreader(model, ip){}
	~PanoramicHTTPPushStreamreader(){};

protected:
	int getSensorNumber();

};
