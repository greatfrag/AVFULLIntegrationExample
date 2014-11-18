#include "stdafx.h"

#include "./cpull_stremreader.h"
#include "../../../viewsupport/FrameDrawer.h"

bool CPullStreamreader::needToRead() const
{
	// need to read only if all queues has more space and at least one queue is exist
	bool result = false;
	for (int i = 0; i < MAX_AMMOUNT_SENSOR; ++i)
	{
		if (m_dataqueue[i]!=0)
		{
			 if (m_dataqueue[i]->size() < m_dataqueue[i]->maxSize())
				result = true;
			else 
				return false;
		}
	}

	return result;
}

void CPullStreamreader::run()
{

	int ret;
	setThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);

	atVeryBegining();

	m_stat[0].start();
	m_stat[1].start();
	m_stat[2].start();
	m_stat[3].start();


	m_streamParam.IntraFramePeriod = 50;

	while(!needExit())
	{
		// check queue sizes

		if (!needToRead())
		{
			Sleep(5);
			continue;
		}

		
		// get image from net 
		unsigned long data_len = 0;
		const char* img_data;
		

		//Iframe set to 1 will force the camera to return an Intra frame with a corresponding SPS and PPS 
		//as an IDR slice – so that the stream is decodable from this point. 
		//When opening a new stream (for example when changing the image size and/or frame rate) 
		//the Intra frame will be sent  automatically regardless of the input value of Iframe.  
		//To reduce the stream size, reduce the frequency of Iframe = 1 in the requests.  
		//The default number of P-frames for any of the streams sent by the camera is set using 
		//SetAV2000Register with register=21, value=number of P-frames  (but also see intra_period below).  
		//The camera will return an Intra frame even if Iframe in the request is set to 0 
		//when the on-camera counter of P-frames fills up.  
		//To find out whether an Intra frame was received, check Iframe after call getImage... function
		int Iframe = 0;

		StreamParams params = getStreamParam();

		img_data = getImageFromCam(data_len,  false, &Iframe, params);
		
	
		int sensorNumber = getSensorNumber();
		if (sensorNumber>=MAX_AMMOUNT_SENSOR || sensorNumber<0)
			img_data = 0; 


		if (img_data==0)
		{
			Sleep(100);
			continue;
		}



		// put it in queue
		FrameData* frd = new FrameData(data_len, img_data);


		frd->info.codecType = params.codecType;
		frd->info.frametype = (Iframe || frd->info.codecType!=JPEG_CODEC) ? FrameType_I : FrameType_P;
		frd->info.width = ( params.resolution == imFULL ) ? params.width : params.width/2;
		frd->info.height = ( params.resolution == imFULL ) ? params.height: params.height/2;

		m_dataqueue[sensorNumber]->push(frd);

		m_stat[sensorNumber].addData(data_len);

		for (int i = 0; i < MAX_AMMOUNT_SENSOR; ++i)
		{
			if (m_drawer[i] && m_stat[i].Update())
				m_drawer[i]->onStatistic(m_stat[i].ToString());

		}
	}

}
