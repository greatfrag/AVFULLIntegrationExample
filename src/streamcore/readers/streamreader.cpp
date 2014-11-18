#include "stdafx.h"

#include "streamreader.h"
#include "../../common/Console.h"
#include "../../viewsupport/FrameDrawer.h"
#include "../../network/simple_http_client.h"

char* delimiter = "\r\n--fbdr\r\n"; 
const int del_len = 10;

int contain_subst(char *data, int datalen, char *subdata, int subdatalen)
{
	if (!data || !subdata || datalen<=0 || subdatalen <= 0 ) 
		return -1;

	int coincidence_len = 0;
	char *pdata = data;


	while(1)
	{
		if (*pdata == subdata[coincidence_len])
			++coincidence_len;
		else
			coincidence_len = 0;

		if (coincidence_len==subdatalen)
			return (pdata-data - subdatalen + 1);

		if (pdata-data==datalen-1)
			return -1;//not found

		pdata++;

	}
}

Streamreader::Streamreader(int model):
m_model(model)
{
	memset(m_dataqueue, 0, sizeof(m_dataqueue));
	memset(m_drawer, 0, sizeof(m_drawer));

	m_streamParam.codecType = JPEG_CODEC;
	m_streamParam.width = 64;
	m_streamParam.height = 64;
	m_streamParam.top = 0;
	m_streamParam.left = 0;
	m_streamParam.bitrate= 0;
	m_streamParam.capture_enable = 0;
	m_streamParam.capture_trigger = 0;
}

Streamreader::~Streamreader()
{
	stop();
}

void Streamreader::setDataQueue(unsigned int index, FrameDataQueue* dataqueue, Drawer* dr )
{
	Mutex mutex(m_params_CS);

	if (index>=MAX_AMMOUNT_SENSOR)
		return;

	m_dataqueue[index] = dataqueue;
	m_drawer[index] = dr;
}


void Streamreader::recalculateSizeforH264(int & left, int& top, int& width, int& height)
{/*
	static const int h_div = 64;
	static const int v_div = 32;


	int max_width = width;
	int max_height = height;

	int rest_h = (width%h_div);
	int rest_v = (height%v_div);


	if (rest_h > 0)
	{
		// width already divisible on 32 ( it's from jpeg )
		// we sholud make it divisible on 64
		left-=rest_h/2;
		width+=rest_h;
		if (left < 0 || width > max_width)
		{
			left += rest_h;
			width -= 2*rest_h;
		}
	}

	if (rest_v > 0)
	{
		top-=rest_v/2;
		height+=rest_v;
		if (top < 0 || height > max_height)
		{
			top += rest_v;
			height -= 2*rest_v;
		}
	}
	//==========================================================
	rest_h = left%h_div;
	rest_v = top%v_div;

	left-=rest_h;
	top-=rest_v; */
}


