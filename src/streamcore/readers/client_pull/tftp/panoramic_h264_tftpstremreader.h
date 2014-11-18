#pragma once

#include "panoramic_tftpstremreader.h"


class PanoramicH264TFTPStreamreader : public PanoramicTFTPStreamreader
{
public:
	explicit PanoramicH264TFTPStreamreader(int model, const std::string& ip, int client_num):
	PanoramicTFTPStreamreader(model, ip, client_num)
	{

	};

	~PanoramicH264TFTPStreamreader()
	{
		stop();
	}


protected:
	virtual const char* getImageFromCam(unsigned long& size, long IsPercent, int* Ifarme, StreamParams& param)
	{
			char* img_data;
			unsigned long capacity = 0;

			int ret;

			long val = 0;



			ret = AV2000F::instance().GetWindowImageQEx(m_client_num, &img_data, &size , &capacity ,
				param.resolution, false, param.quality, param.left, param.top, param.width, param.height, 
				param.codecType, param.stream_id, Ifarme, param.bitrate, param.IntraFramePeriod);



			if (ret!=1)
			{
				img_data = 0;
				size = 0;
			}

			return img_data;

	}
	

};