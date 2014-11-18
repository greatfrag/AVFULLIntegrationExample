#include "stdafx.h"

#include "ss_tftpstremreader.h"
#include "../../../../cameramodel/cameramodel.h"
#include <sstream>

SSTFTPStreamreader::SSTFTPStreamreader(int model, const std::string& ip, int client_num):
CPullStreamreader(model, ip),
m_client_num(client_num)
{

	AV2000F::instance().CreateClient(m_client_num);
	AV2000F::instance().pSetClientIp(client_num, m_cam_ip.c_str());
	int ret = AV2000F::instance().pUpdateVersion(client_num);

	m_inited = true;

	if (ret!=1)
		m_inited = false;


}

SSTFTPStreamreader::~SSTFTPStreamreader()
{
	stop();
	AV2000F::instance().DestroyClient(m_client_num);
}

void SSTFTPStreamreader::setChannel(int channel )
{
	AV2000F::instance().SetChannelNumber(m_client_num, channel);
	AV2000F::instance().pSetClientTimeout(m_client_num,1000);
}

const char* SSTFTPStreamreader::getImageFromCam(unsigned long& size, long IsPercent, int* Ifarme, StreamParams& param)
{

	char* img_data;
	unsigned long capacity = 0;
	static unsigned short captured_num = 0;
	//static unsigned short 

	int ret;
	bool db = param.doublescan?true:false;

	long val = 0;

	img_data = 0;
	size = 0;

	if (!m_inited)
		return img_data;

	/*
	param.left = 0;
	param.top = 0;
	param.width = 1600;
	param.height = 1200;
	/**/
	if (!param.capture_trigger)

	ret = AV2000F::instance().GetWindowImageQEx(m_client_num, &img_data, &size , &capacity ,
		param.resolution, db, param.quality, param.left, param.top, param.width, param.height, 
		param.codecType, param.stream_id, Ifarme, param.bitrate, param.IntraFramePeriod);
	else 
	ret = AV2000F::instance().GetWindowImageQEx(m_client_num, &img_data, &size , &capacity ,
		param.resolution, false, param.quality, param.left, param.top, param.width, param.height, 
		JPEG_CODEC, param.stream_id, Ifarme, param.bitrate, param.IntraFramePeriod);

	if (ret!=1 || size==0)
	{
		img_data = 0;
		size = 0;
	}
	else 
	{
		const unsigned char* last_packet = AV2000F::instance().pGetLastPacket(m_client_num);
		if (last_packet[1]) // single capture image
		{
			if (m_streamParam.capture_trigger || (!m_streamParam.capture_trigger && param.codecType == JPEG_CODEC)) // make sure it's JPEG request
			{
				// save to disc
				std::ostringstream os;
				os << "sc/sp";
				os << captured_num;
				os << ".jpg";
				FILE* imgf = fopen(os.str().c_str(), "wb");
				captured_num++;
				if (imgf)
				{
					fwrite(img_data, 1, size, imgf);
					fclose(imgf);
				}
				img_data = 0;
				size = 0;
				if (param.capture_trigger)m_streamParam.capture_trigger--;
			}
			else
			{
				m_streamParam.capture_trigger = 0;
			}
		}
	}

	if (m_streamParam.capture_trigger) {				
		img_data = 0;
		size = 0;
	}

	return img_data;

}

void SSTFTPStreamreader::refreshSensorSize(StreamParams& param)
{
	
	long left = param.left;
	long top = param.top;

	long widthl = param.width;
	long heightl = param.height;

	AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_LEFT, &left);
	AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_TOP, &top);
	AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_WIDTH, &widthl);
	AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_HEIGHT, &heightl);
}

void SSTFTPStreamreader::setDNMode(DN_modes mode)
{
	long val = mode;
	AV2000F::instance().SetAV2000Parameter(m_client_num, cpDAY_NIGHT_MODE, &val);
}

void SSTFTPStreamreader::setLightHz(Light_modes mode)
{
	long curr_br;
	long val = mode;

	AV2000F::instance().SetAV2000Parameter(m_client_num, cpLIGHTING, &val); // change hz

	// brightness 
	AV2000F::instance().GetAV2000Parameter(m_client_num, cpBRIGHTNESS, &curr_br);

	val = -99;
	AV2000F::instance().SetAV2000Parameter(m_client_num, cpBRIGHTNESS, &val);// to min
	Sleep(150);

	
	AV2000F::instance().SetAV2000Parameter(m_client_num, cpBRIGHTNESS, &curr_br);// to cur again
	
}

void SSTFTPStreamreader::setCaptureEnable(bool enable)
{
//	int val = enable;
	//AV2000F::instance().SetSingleCapture(m_client_num, val);
	unsigned char buff[2];
	unsigned char reg = 21;
	buff[0] = 0;
	buff[1] = (unsigned char)enable;
	AV2000F::instance().SetAV2000Register(m_client_num, 25, (unsigned char*)&buff);
}

void SSTFTPStreamreader::setCaptureTrigger(unsigned short trigger)
{
	unsigned char buff[2];
	unsigned char reg = 21;
	buff[0] = 0;
	buff[1] = (unsigned char)trigger;
	if (m_streamParam.fmversion >= 65000) reg = 192;
	AV2000F::instance().SetAV2000Register(m_client_num, reg, (unsigned char*)&buff);
}

void SSTFTPStreamreader::atVeryBegining()
{
	/*
	long left = 0;
	long top = 0;

	long widthl = Camera::getMaxSensorWidth(m_model);;
	long heightl = Camera::getMaxSensorHeight(m_model);;

	AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_LEFT, &left);
	AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_TOP, &top);
	AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_WIDTH, &widthl);
	AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_HEIGHT, &heightl);
	/**/
}