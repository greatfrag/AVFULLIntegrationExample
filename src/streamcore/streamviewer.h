#pragma once

#include "../common/types.h"
#include "stremdecoder.h"
#include "../viewsupport/FrameDrawer.h"
#include <string>

//frames reader => frames queue => decoder => display 
// This calass contains: queue,  decoder,  display; and share frames queue fore any kind of reader

class StreamViewer
{
public:
	explicit StreamViewer(Drawer& drawer );
	~StreamViewer();

	void start();
	void stop();

	FrameDataQueue* getFrameQuue(){return &m_frameQueue;}

private:
	FrameDataQueue m_frameQueue;
	StreamDecoder m_decoder;
	Drawer& m_drawer;

};
