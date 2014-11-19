#include "stdafx.h"

#define MAX_IMAGE_SIZE (1024*1024*5)

#include "ss_push_httpstremreader.h"
#include "../../../../network/simple_http_client.h"
#include "../../../../common/Console.h"
#include <sstream>

SSHTTPPushStreamreader::SSHTTPPushStreamreader(int model, const char* ip):
SPushStreamreader(model),
m_cam_ip(ip),
m_http_client(0)
{
	m_data = new char[MAX_IMAGE_SIZE];
}

SSHTTPPushStreamreader::~SSHTTPPushStreamreader()
{
	stop();
	delete[] m_data;
	delete m_http_client;
}


const char* SSHTTPPushStreamreader::getNextImageFromCam(unsigned long& size, int *Iframe )
{
	if (!isStreamOpened())
	{
		size = 0;
		return 0;
	}

	
	
	int frame_len = 0;

	if (rest_len)
	{
		memcpy(m_data, rest, rest_len);
		rest = m_data;
		frame_len = rest_len;
		rest_len = 0;
	}



	while(m_http_client->isOpened() && !needExit())
	{
		int readed = m_http_client->read(m_data +  frame_len, 2000);
		if (readed<=0)
		{
			closeStream();
			size = 0;
			return 0;
		}

		int start_position = max(frame_len - del_len, 0); // start search position; we have to avoid multiple search;
		int serch_lengt = start_position==0 ? readed + frame_len : readed + del_len; // how many bytes look in; readed + may be last bytes from prev reading
		int fbdr_index = contain_subst(m_data + start_position, serch_lengt, delimiter, del_len); // if fbdr_index is positive then we found fbdr; fbdr_index now in termes of how many bytes from start_position; needs to be corrected
		

		if (fbdr_index >=0 )// found edge of the frame
		{
			fbdr_index+=start_position; // correction;  now it is shift from very begining of date;

			size = fbdr_index; // size of the frame is all bytes until fbdr;
			rest_len = frame_len + readed - fbdr_index - del_len; // how many bytes we've read after fbdr
			if (rest_len)
				rest = m_data + frame_len + readed - rest_len;


			if (size>0)// remove zeroes at the end of the last packets 
			{
				while(m_data[size-1]==0)
					size--;
			}

			//unit delimetr
			m_data[size] = 0;
			m_data[size+1] = 0;
			m_data[size+2] = 1;
			m_data[size+3] = 0x09;
			m_data[size+4] = 0x10;

			size+=5;
			


			// now we have to locolase Content-Type;
			int frame_data = contain_subst(m_data, min(50, size/2), "\r\n\r\n",4);
			if (frame_data<=0) // smth wrong 
			{
				closeStream();
				size = 0;
				return 0;
			}

			frame_data+=4;
			size-=frame_data;

			if ( contain_subst(m_data, frame_data , "jpeg",strlen("jpeg"))>=0 )
				*Iframe = 1;
			else if (contain_subst(m_data, frame_data , "video/H.264I",strlen("video/H.264I"))>=0 )
                *Iframe = 1;
			else if (contain_subst(m_data, frame_data , "video/H.264P",strlen("video/H.264P"))>=0 )
				*Iframe = 0;
			else *Iframe = 0;


            
			return m_data + frame_data;
		}

		frame_len+=readed;
	}

}

void SSHTTPPushStreamreader::openStream(StreamParams& param)
{
	static unsigned short chan = 1;
	std::string db = "&doublescan=0";

	if (isStreamOpened()) // already opened 
		return;

	
	if (param.codecType==H264_CODEC)
		param.quality=37-param.quality; // for H.264 it's not quality; it's qp 

	std::ostringstream os;
	if (param.codecType==JPEG_CODEC)
		os <<"mjpeg";
	else
		os<<"h264stream";
/*
	os<<chan;
	if (++chan > 4) chan = 1;
*/
	if (param.client) os<<param.client;

	os<<"?res=";

	if(param.resolution==imFULL)
		os<<"full";
	else
		os<<"half";

	os<<"&x0=" << param.left << "&y0=" << param.top << "&x1=" << param.left + param.width << "&y1=" << param.top + param.height;

	if (param.codecType==JPEG_CODEC)
		os<<"&quality=";
	else
		os<<"&qp=";

	if(param.doublescan)
		db = "&doublescan=1";
	else
		db = "&doublescan=0";
	os<< param.quality << db << "&ssn=" << param.stream_id;

	if (param.fps>0)
		os << "&fps=" << param.fps;
	

	if (param.codecType==H264_CODEC)
	{
		if (param.bitrate)
			os <<"&bitrate=" << param.bitrate;
	}

	if (param.scaledimage)
		os << "&channel=scaled";



	m_http_client = new SimpleHTTPClient(m_cam_ip);
	m_http_client->setRequestLine(os.str());
	m_http_client->openStream();

	if (!m_http_client->isOpened())
		closeStream();
	else
		m_http_client->read(m_data,del_len); // first delimiter;

	rest = m_data;
	rest_len = 0;
}

void SSHTTPPushStreamreader::closeStream()
{
	delete m_http_client;
	m_http_client = 0;
}

bool SSHTTPPushStreamreader::isStreamOpened()
{
	return (m_http_client && m_http_client->isOpened());
}

void SSHTTPPushStreamreader::refreshSensorSize(StreamParams& param)
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

void SSHTTPPushStreamreader::setDNMode(DN_modes mode)
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

void SSHTTPPushStreamreader::setLightHz(Light_modes mode)
{
	SimpleHTTPClient http_client(m_cam_ip);
	std::ostringstream os;
	os << "set?freq="<<(int)mode;
	http_client.setRequestLine(os.str());
	http_client.openStream();
}
