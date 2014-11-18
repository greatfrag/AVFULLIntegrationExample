#include "stdafx.h"

#include "./spush_stremreader.h"
#include "../../../viewsupport/FrameDrawer.h"



void SPushStreamreader::setStreamParams(StreamParams newParam)
{
	
	bool runing = isRuning();

	if (runing)
	{
		stop();
		Streamreader::setStreamParams(newParam);
		start();
	}
	else
		Streamreader::setStreamParams(newParam);

}

void SPushStreamreader::run()
{

	int ret;
	setThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);

	atVeryBegining();

	m_stat[0].start();
	m_stat[1].start();
	m_stat[2].start();
	m_stat[3].start();


	m_streamParam.IntraFramePeriod = 50;

	StreamParams params;

	while(!needExit())
	{
		// check queue sizes

		if (!isStreamOpened())
		{
			params = getStreamParam();
			openStream(params);
			
			if (!isStreamOpened())
			{
				Sleep(50);
				continue;
			}
		}


		
		// get image from net 
		unsigned long data_len = 0;
		const char* img_data;

		int Iframe = 0;
		

		img_data = getNextImageFromCam(data_len, &Iframe);


	
		int sensorNumber = getSensorNumber();
		if (sensorNumber>=MAX_AMMOUNT_SENSOR || sensorNumber<0)
			img_data = 0; 


		if (img_data==0)
		{
			Sleep(50);
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

	if (isStreamOpened())
		closeStream();

}
