#pragma once
#include "../common/Runnable.h"
#include "../common/types.h"
#include "../decoder/videodecoder.h"
#include "../viewsupport/FrameDrawer.h"
#include "../viewsupport/colorspace.h"

class  StreamDecoder : public Runnable
{
public:
	StreamDecoder(FrameDataQueue& dataqueue, Drawer& drawer );
	~StreamDecoder();

private:
	void run();
	long SaveToBmp(Frame&);
	long ExtractZoomFrame(const Frame&, Frame&);

private:
	unsigned char *m_pcBmpBuffer;
	unsigned long m_dwBufferSize;
	unsigned long m_dwFrameCount;

	FrameDataQueue& m_dataqueue;
	VideoCodec m_h264;
	VideoCodec m_mjpeg;
	Drawer& m_drawer;
	ColorSpaceDll& m_csdll;
    
};