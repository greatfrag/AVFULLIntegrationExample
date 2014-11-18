#include "stdafx.h"
#include ".\videodecoder.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <map>
#include <time.h>

#define EXT_BUFF_SIZE 2*1024*1024
//===========Dll========
VideoCodec::CodecDll::CodecDll()
{
 
}
bool VideoCodec::CodecDll::init()
{
	const char* dll_name = "avcodec-52.dll";

	m_dll  = ::LoadLibrary(dll_name);

	if(!m_dll)
		return false;

	avcodec_init = reinterpret_cast<dll_avcodec_init>(::GetProcAddress(m_dll, "avcodec_init"));
	if(!avcodec_init)
		return false;

	avcodec_find_decoder = reinterpret_cast<dll_avcodec_find_decoder>(::GetProcAddress(m_dll, "avcodec_find_decoder"));
	if(!avcodec_find_decoder)
		return false;

	avcodec_register_all = reinterpret_cast<dll_avcodec_register_all>(::GetProcAddress(m_dll, "avcodec_register_all"));
	if (!avcodec_register_all)
		return false;


	avcodec_alloc_context  = reinterpret_cast<dll_avcodec_alloc_context>(::GetProcAddress(m_dll, "avcodec_alloc_context"));
	if (!avcodec_register_all)
		return false;

	avcodec_alloc_frame  = reinterpret_cast<dll_avcodec_alloc_frame>(::GetProcAddress(m_dll, "avcodec_alloc_frame"));
	if (!avcodec_alloc_frame)
		return false;


	avcodec_open  = reinterpret_cast<dll_avcodec_open>(::GetProcAddress(m_dll, "avcodec_open"));
	if (!avcodec_open)
		return false;


	avcodec_close  = reinterpret_cast<dll_avcodec_close>(::GetProcAddress(m_dll, "avcodec_close"));
	if (!avcodec_close)
		return false;

	const char* dll_name2 = "avutil-49.dll";
	m_dll2 = ::LoadLibrary(dll_name2);
	if(!m_dll2)
		return false;


	av_free = reinterpret_cast<dll_av_free>(::GetProcAddress(m_dll2, "av_free"));
	if (!av_free)
		return false;


	avcodec_decode_video = reinterpret_cast<dll_avcodec_decode_video>(::GetProcAddress(m_dll, "avcodec_decode_video"));
	if (!avcodec_decode_video)
		return false;
	//==================================================================================

}
VideoCodec::CodecDll::~CodecDll()
{
	::FreeLibrary(m_dll);
	::FreeLibrary(m_dll2);
}

//================================================
//VideoCodec::CodecDll VideoCodec::dll;
bool VideoCodec::m_first_instance = true;
//================================================


VideoCodec::VideoCodec(CodecID id):
c(0),
m_width(0),
m_height(0),
m_id(id)
{

	if (m_first_instance || true)
	{
		m_first_instance = false;

		dll.init();
		
		// must be called before using avcodec 
		dll.avcodec_init();

		// register all the codecs (you can also register only the codec you wish to have smaller code
		dll.avcodec_register_all();

	}
									
	//codec = dll.avcodec_find_decoder(CODEC_ID_H264);
	codec = dll.avcodec_find_decoder(id);


	c = dll.avcodec_alloc_context();
	picture= dll.avcodec_alloc_frame();

	if(codec->capabilities&CODEC_CAP_TRUNCATED)
		c->flags|= CODEC_FLAG_TRUNCATED;

     dll.avcodec_open(c, codec);


	 extended_buf = new char[EXT_BUFF_SIZE];
        /**/

}

VideoCodec::~VideoCodec(void)
{
	dll.avcodec_close(c);
	dll.av_free(c);
	dll.av_free(picture);
}

void VideoCodec::resart_decoder()
{
    if (c)
    {
	    dll.avcodec_close(c);
        dll.av_free(c);
        dll.av_free(picture);
    }

	c = dll.avcodec_alloc_context();
	picture= dll.avcodec_alloc_frame();

	if(codec->capabilities&CODEC_CAP_TRUNCATED)
		c->flags|= CODEC_FLAG_TRUNCATED;

    dll.avcodec_open(c, codec);

}


bool VideoCodec::decode(const char* inbuf_ptr, int len, int width, int height, Frame& frame , int temp)
{
	if (len > EXT_BUFF_SIZE - FF_INPUT_BUFFER_PADDING_SIZE)
		return false;

	
	if ((width!=m_width)||(height!=m_height))
    {
            m_width = width;
            m_height = height;
            resart_decoder();
    }

	//The input buffer must be FF_INPUT_BUFFER_PADDING_SIZE larger than the actual read bytes because some optimized bitstream readers read 32 or 64 bits at once and could read over the end.
	//The end of the input buffer buf should be set to 0 to ensure that no overreading happens for damaged MPEG streams.
	memcpy(extended_buf, inbuf_ptr, len);
	memset(extended_buf+len, 0 , FF_INPUT_BUFFER_PADDING_SIZE);


	int got_picture = 0;
    dll.avcodec_decode_video(c, picture, &got_picture,(unsigned char*)extended_buf, len);

	if (got_picture )
	{
	
		frame.width = c->width;
		frame.height = c->height;

		frame.Y = picture->data[0];
		frame.U = picture->data[1];
		frame.V = picture->data[2];

		frame.strideY = picture->linesize[0];
		frame.strideCRCB = picture->linesize[1];

		switch (m_id)
		{
		case CODEC_ID_H264:
			frame.format = yuv_420;
			break;

		case CODEC_ID_MJPEG:
			frame.format = yuv_422;
			break;
		}

		
	}
    else
    {
        // we have to do it after error in stream
        //resart_decoder();
		return false;
    }


	
}

