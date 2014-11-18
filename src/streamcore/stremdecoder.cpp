#include "stdafx.h"
#include "stremdecoder.h"
#include "../common/Console.h"

StreamDecoder::StreamDecoder(FrameDataQueue& dataqueue, Drawer& drawer ):
m_dataqueue(dataqueue),
m_drawer(drawer),
m_h264(CODEC_ID_H264),
m_mjpeg(CODEC_ID_MJPEG),
m_csdll(ColorSpaceDll::instance())
{
	m_pcBmpBuffer = 0;
	m_dwBufferSize = 0;
	m_dwFrameCount = 0;
}

StreamDecoder::~StreamDecoder()
{
	stop();
}

void StreamDecoder::run()
{

	FILE* f_out = fopen("test.264", "wb");

	//FILE* f_out = fopen("C:/Documents and Settings/Sergey/Desktop/tests/test.264", "wb");

	while(!needExit())	
	{
		FrameData* frame_data;


		bool get = m_dataqueue.pop(frame_data, 50);

		if (!get)
		{
			continue;
		}

		const char *data = frame_data->uncm_data.getData();
		int len = frame_data->uncm_data.getSize();
		int width = frame_data->info.width;
		int height = frame_data->info.height;


		bool decoded = false;


		switch(frame_data->info.codecType) // H264
		{
		case H264_CODEC:
			if (f_out)
				fwrite(data,len,1,f_out);
			decoded = m_h264.decode(data,len,width, height, frame_data->frame, 0);
			break;

		case JPEG_CODEC:
			decoded = m_mjpeg.decode(data,len,width, height, frame_data->frame, 0);
			break;


		}
	
		//decoded = false;

		if (decoded && !needExit())
		{
			// put frame to display
			m_drawer.DrawFrame(&frame_data->frame);
		}
		else
		{
			// we should not put bad frame to display
		}


		delete frame_data;


	}

	fclose(f_out);

	{
		std::ostringstream os;
		os << "StreamDecoder joined id = " << int(this);
		CConsole::getInstance().Print(os);
	}


}


