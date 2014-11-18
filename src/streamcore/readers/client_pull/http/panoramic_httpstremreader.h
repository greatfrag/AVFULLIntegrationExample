#pragma once
#include <string>
#include "httpstremreader.h"



//single sensor HTTP reader
class PanoramicHTTPStreamreader : public SSHTTPStreamreader
{
public:
	explicit PanoramicHTTPStreamreader(int model, const std::string& ip) :
	SSHTTPStreamreader(model, ip){};
			~PanoramicHTTPStreamreader(){};

protected:
	int getSensorNumber(); // single sensor camera has only one sensor
};