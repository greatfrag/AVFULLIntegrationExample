#include "stdafx.h"

#include "panoramic_tftpstremreader.h"



PanoramicTFTPStreamreader::PanoramicTFTPStreamreader(int model, const std::string& ip, int client_num):
CPullStreamreader(model, ip)
{
	m_client_num = client_num;

	AV2000F::instance().CreateClient(m_client_num);
	AV2000F::instance().pSetClientIp(client_num, m_cam_ip.c_str());
	int ret = AV2000F::instance().pUpdateVersion(client_num);

	m_inited = true;

	if (ret!=1)
		m_inited = false;

}

PanoramicTFTPStreamreader::~PanoramicTFTPStreamreader()
{
	stop();
	AV2000F::instance().DestroyClient(m_client_num);
}

 const char* PanoramicTFTPStreamreader::getImageFromCam(unsigned long& size, long IsPercent, int* Ifarme, StreamParams& param)
{
	char* img_data = 0;;
	unsigned long capacity = 0;

	int ret;

	if (!m_inited) return 0;


	ret = AV2000F::instance().GetDefaultImageEx(m_client_num, &img_data, &size , &capacity ,
		param.codecType, param.stream_id, Ifarme);


	if (ret!=1)
	{
		img_data = 0;
		size = 0;
	}

	return img_data;

}

int PanoramicTFTPStreamreader::getSensorNumber()
{
	if (!m_inited) return 0;
	long result;
	AV2000F::instance().pGetAV2000Parameter(m_client_num, cpMS_NUMBER_OF_SENSOR, &result);

	return result-1;
}

void PanoramicTFTPStreamreader::atVeryBegining()
{
	if (!m_inited) return;
	long val;

	val = 15;
	AV2000F::instance().SetAV2000Parameter(m_client_num,cpMS_CHANNEL_ENABLE,&val);


	val = 0;
	AV2000F::instance().SetAV2000Parameter(m_client_num,cpMS_QUAD_MODE, (long*)&val);

	val = 0;
	AV2000F::instance().SetAV2000Parameter(m_client_num,cpMS_ZOOM_WIN_ENABLE, (long*)&val);

	val = 0;
	AV2000F::instance().SetAV2000Parameter(m_client_num,cpMS_ONE_SHOT_ENABLE, (long*)&val);


}

void PanoramicTFTPStreamreader::setStreamParams(StreamParams newParam)
{
	if (!m_inited) return;
	Mutex mutex(m_params_CS);
	if (newParam.quality!=m_streamParam.quality)
		setQuality(newParam.quality);

	if (newParam.resolution != m_streamParam.resolution)
		setResolution(newParam.resolution);


	if (newParam.width != m_streamParam.width  || newParam.height != m_streamParam.height)
		setResolution(newParam.width, newParam.height);


	if (newParam.bitrate != m_streamParam.bitrate)
		setBitrate(newParam.bitrate);


	m_streamParam = newParam;
}


void PanoramicTFTPStreamreader::setQuality(int q)
{
	if (!m_inited) return;
	for (long i = 1; i<=4; ++i)
	{
		AV2000F::instance().SetAV2000Parameter(m_client_num,cpMS_NUMBER_OF_SENSOR, &i);
		AV2000F::instance().SetAV2000Parameter(m_client_num,cpQUALITY_FULL, (long*)&q);
		AV2000F::instance().SetAV2000Parameter(m_client_num,cpQUALITY_HALF, (long*)&q);
	}

}

void PanoramicTFTPStreamreader::setResolution(IMAGE_RESOLUTION res)
{
	if (!m_inited) return;
	int value = (res == imFULL) ? 15 : 0; // all sensors to full/half resolution

	unsigned char buff[2];
	buff[0] = value / 256;
	buff[1] = value % 256;
	AV2000F::instance().SetAV2000Register(m_client_num, 0xD1, (unsigned char*)&buff);
	//AV2000F::instance().SetAV2000Parameter(m_client_num, cpMS_FULL_RES_ENABLE, &value);

	long val = (res == imFULL) ? 0 : 1;

	AV2000F::instance().SetAV2000Parameter(m_client_num, cpRESOLUTION, &val);

	setResolution(m_streamParam.width, m_streamParam.height);
	setBitrate(m_streamParam.bitrate);
}

void PanoramicTFTPStreamreader::refreshSensorSize(StreamParams& param)
{
	setResolution(param.width, param.height);
}

void PanoramicTFTPStreamreader::setResolution(int width_, int height_) 
{
	if (!m_inited) return;
	long left = 0;
	long top = 0;
	long widthl = width_;
	long heightl = height_;


	for (long i = 1; i<=4; ++i)
	{
		AV2000F::instance().SetAV2000Parameter(m_client_num,cpMS_NUMBER_OF_SENSOR, &i);
		AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_LEFT, &left);
		AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_TOP, &top);
		AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_WIDTH, &widthl);
		AV2000F::instance().SetAV2000Parameter(m_client_num, cpSENSOR_HEIGHT, &heightl);
	}	
}

void PanoramicTFTPStreamreader::setBitrate(int br)
{
	// in this example I will set up the same bitrate for each channel;
	// but it could be a different ...

	long bitrate = br;

	for (long i = 1; i<=4; ++i)
	{
		AV2000F::instance().SetAV2000Parameter(m_client_num,cpMS_NUMBER_OF_SENSOR, &i);
		AV2000F::instance().SetAV2000Parameter(m_client_num, cpBIT_RATE, &bitrate);
	}

}

void PanoramicTFTPStreamreader::setLightHz(Light_modes mode)
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