#include "stdafx.h"

#define MAX_IMAGE_SIZE (1024*1024*5)

#include "httpstremreader.h"
#include "../../../../network/simple_http_client.h"
#include "../../../../common/Console.h"
#include <sstream>


SSHTTPStreamreader::SSHTTPStreamreader(int model, const std::string& ip):
CPullStreamreader(model, ip)
{
	m_data = new char[MAX_IMAGE_SIZE];
}

SSHTTPStreamreader::~SSHTTPStreamreader()
{
	stop();
	delete[] m_data;
}


const char* SSHTTPStreamreader::getImageFromCam(unsigned long& size, long IsPercent, int* Ifarme, StreamParams& param)
{
	unsigned long capacity = 0;
	size = 0;
	static unsigned short chan = 1;

	if (param.codecType==H264_CODEC)
		param.quality=37-param.quality; // for H.264 it's not quality; it's qp 

	std::ostringstream os;
	if (param.codecType==JPEG_CODEC)
		os <<"image";
	else
		os<<"h264f";

//	os<<chan;
//	if (++chan > 4) chan = 1;

	os<<"?res=";

	if(param.resolution==imFULL)
		os<<"full";
	else
		os<<"half";

	/*
	param.left = 0;
	param.top = 0;
	param.width = 1600;
	param.height = 1200;
	/**/

	os<<"&x0=" << param.left << "&y0=" << param.top << "&x1=" << param.left + param.width << "&y1=" << param.top + param.height;

	if (param.codecType==JPEG_CODEC)
		os<<"&quality=";
	else
		os<<"&qp=";

	if(param.doublescan)
		os<< param.quality << "&doublescan=1" << "&ssn=" << param.stream_id;
	else
		os<< param.quality << "&doublescan=0" << "&ssn=" << param.stream_id;

	if (param.codecType==H264_CODEC)
	{
		os <<"&iframe=" << *Ifarme;

		if (param.bitrate)
			os <<"&bitrate=" << param.bitrate;
	}

	if(param.scaledimage)
		os << "&channel=scaled";




	SimpleHTTPClient http_client(m_cam_ip);
	http_client.setRequestLine(os.str());
	http_client.openStream();


	while(http_client.isOpened())
	{
		int readed = http_client.read((m_data+(int)size), 2000);

		if (readed<=0 || size + 2000 >= MAX_IMAGE_SIZE)
		{
			break;
		}

		size+= readed;
	}

	if (size>0)// remove zeroes at the end of the last packets 
	{
		while(m_data[size-1]==0)
			size--;
	}
	
	//unit delimetr
	if (param.codecType==H264_CODEC)
	{
		m_data[size] = 0;
		m_data[size+1] = 0;
		m_data[size+2] = 1;
		m_data[size+3] = 0x09;
		m_data[size+4] = 0x10;

		size+=5;

	}

	return m_data;

}


void SSHTTPStreamreader::refreshSensorSize(StreamParams& param)
{
	//return;
	bool opened = false;

	{
		SimpleHTTPClient http_client(m_cam_ip);
		std::ostringstream os;
		os << "set?sensorleft="<<param.left;
		http_client.setRequestLine(os.str());
		http_client.openStream();
		opened = http_client.isOpened();
	}

	if (!opened)
		return;

	{
		SimpleHTTPClient http_client(m_cam_ip);
		std::ostringstream os;
		os << "set?sensortop="<<param.top;
		http_client.setRequestLine(os.str());
		http_client.openStream();
	}


	{
		SimpleHTTPClient http_client(m_cam_ip);
		std::ostringstream os;
		os << "set?sensorwidth="<<param.width;
		http_client.setRequestLine(os.str());
		http_client.openStream();
	}

	{
		SimpleHTTPClient http_client(m_cam_ip);
		std::ostringstream os;
		os << "set?sensorheight="<<param.height;
		http_client.setRequestLine(os.str());
		http_client.openStream();
	}


}

void SSHTTPStreamreader::setDNMode(DN_modes mode)
{
	SimpleHTTPClient http_client(m_cam_ip);
	std::ostringstream os;
	os << "set?daynight=";
	switch(mode)
	{
	case AV_DN_Auto:
		os<<"auto";
		break;

	case AV_DN_Day:
		os<<"day";
		break;

	case AV_DN_Night:
		os<<"night";
		break;
	}

	http_client.setRequestLine(os.str());
	http_client.openStream();

}

void SSHTTPStreamreader::setLightHz(Light_modes mode)
{
	SimpleHTTPClient http_client(m_cam_ip);
	std::ostringstream os;
	os << "set?freq="<<(int)mode;
	http_client.setRequestLine(os.str());
	http_client.openStream();
}


/**/