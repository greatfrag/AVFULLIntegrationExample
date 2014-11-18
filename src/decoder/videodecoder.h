#ifndef H264_h_2326
#define H264_h_2326


#include <windows.h>
#include "../ffmpeg/avcodec.h"
#include "../frameinfo/frame.h"




class VideoCodec
{
public:
	VideoCodec(CodecID id);
    bool decode(const char* data, int len, int width, int height, Frame& frame , int temp);
	~VideoCodec();



	class CodecDll
	{
	public:
		CodecDll();
         bool  init();
		~CodecDll();
		//============dll =======
		typedef void (*dll_avcodec_init)(void);
		typedef AVCodec* (*dll_avcodec_find_decoder)(enum CodecID id);

		typedef void (*dll_avcodec_register_all)(void);
		typedef AVCodecContext* (*dll_avcodec_alloc_context)(void) ;
		typedef AVFrame*(*dll_avcodec_alloc_frame)(void);
		typedef int (*dll_avcodec_open)(AVCodecContext *avctx, AVCodec *codec);
		typedef int (*dll_avcodec_close)(AVCodecContext *avctx);
		typedef void (*dll_av_free)(void *ptr);
		typedef int (*dll_avcodec_decode_video)(AVCodecContext *avctx, AVFrame *picture, int *got_picture_ptr, uint8_t *buf, int buf_size) ;


		dll_avcodec_init avcodec_init;
		dll_avcodec_find_decoder avcodec_find_decoder;
		dll_avcodec_register_all avcodec_register_all;
		dll_avcodec_alloc_context avcodec_alloc_context ;
		dll_avcodec_alloc_frame avcodec_alloc_frame;
		dll_avcodec_open avcodec_open;
		dll_avcodec_close avcodec_close;
		dll_av_free av_free;
		dll_avcodec_decode_video avcodec_decode_video;
	private:
		HINSTANCE m_dll, m_dll2;
	};

private:

	

    void resart_decoder();

	static bool m_first_instance;

	AVCodec *codec;
	AVCodecContext *c;
	AVFrame *picture;

    int m_width;
    int m_height;

	CodecDll dll;

	CodecID m_id; 

	char* extended_buf;
	//===================


};



#endif //H264_h_2326