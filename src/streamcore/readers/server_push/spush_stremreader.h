#pragma once

#include "../streamreader.h"

class SPushStreamreader : public Streamreader
{
public:
	SPushStreamreader (int model):Streamreader(model){};
	~SPushStreamreader (){stop();}

	virtual void setStreamParams(StreamParams newParam);

protected:
	virtual const char* getNextImageFromCam(unsigned long& size, int *Iframe ) = 0;
	virtual void openStream(StreamParams& params) = 0;
	virtual void closeStream() = 0;
	virtual bool isStreamOpened() = 0;
	virtual void setCaptureEnable(bool enable){};
	virtual void setCaptureTrigger(unsigned short trigger){};

	void run(); // in a loop: takes images from camera and put into quue


};