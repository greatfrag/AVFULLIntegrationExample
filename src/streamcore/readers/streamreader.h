#pragma once

#define MAX_AMMOUNT_SENSOR 4

#include "../../common/Runnable.h"
#include "../../sdk_dll/AVLoadDll.h"
#include "../../common/types.h"
#include "../../common/WinCriticalSection.h"
#include "../../statistics/statistic.h"

class Drawer;

enum DN_modes {AV_DN_Auto = 0, AV_DN_Day = 1, AV_DN_Night = 2, AV_DN_Unknown};
enum Light_modes {AV_HZ_50 = 50, AV_HZ_60 = 60, AV_HZ_Unknown};

extern char* delimiter;			// = "\r\n--fbdr\r\n"; 
extern const int del_len;	// = 10;

extern int contain_subst(char *data, int datalen, char *subdata, int subdatalen);

struct StreamParams
{
	StreamParams()
	{
		dn_mode = AV_DN_Unknown;
		left = -1;
		light = AV_HZ_Unknown;
		doublescan = 0;
		scaledimage = false;
	}

	int left;
	int top;
	int width;
	int height;

	int fps;

	IMAGE_RESOLUTION resolution; // full or half
	AVCodecType codecType ; // JPEG or H264

	int quality;  // 1-21
	int IntraFramePeriod;
	int bitrate; // if bitrate more than zerro, then  quality is ignored
	int doublescan;	// if doublescan
	
	bool scaledimage; //if scaled image

	int stream_id; // only for H264 streams

	DN_modes dn_mode;
	Light_modes light;
	bool capture_enable;
	unsigned short capture_trigger;
	int fmversion;

	unsigned short client;
};


class Streamreader : public Runnable
{
public:
	explicit Streamreader(int model);
	virtual ~Streamreader();

	void setDataQueue(unsigned int index, FrameDataQueue* dataqueue, Drawer* dr );

	virtual void setStreamParams(StreamParams newParam)
	{
		Mutex mutex(m_params_CS); 
		recalculateSizeforH264(newParam.left, newParam.top, newParam.width, newParam.height); // it's not necessary to do}

		bool needrefreshSensor = m_streamParam.left!=newParam.left || m_streamParam.top!=newParam.top || 
			m_streamParam.width!=newParam.width || m_streamParam.height!=newParam.height;

		if (needrefreshSensor)
			refreshSensorSize(newParam);

		if (newParam.dn_mode!=m_streamParam.dn_mode)
			setDNMode(newParam.dn_mode);
			
		if (newParam.light!=m_streamParam.light)
			setLightHz(newParam.light);
			
		if (newParam.capture_enable!=m_streamParam.capture_enable)
			setCaptureEnable(newParam.capture_enable);

		if (newParam.capture_trigger!=m_streamParam.capture_trigger)
		{
			if(!m_streamParam.capture_enable) 
			{	newParam.capture_enable = TRUE;
				setCaptureEnable(newParam.capture_enable);
			}
			setCaptureTrigger(newParam.capture_trigger);
//			if (newParam.capture_trigger) newParam.capture_trigger--;
		}

		m_streamParam = newParam;
		


	}	

	StreamParams getStreamParam() const
	{
		Mutex mutex(m_params_CS);
		return m_streamParam;
	}


protected:
	virtual void run() = 0;
	virtual int getSensorNumber()= 0;
	virtual void atVeryBegining(){};
	virtual void refreshSensorSize(StreamParams& param)=0;
	virtual void setDNMode(DN_modes mode) = 0;
	virtual void setLightHz(Light_modes mode) = 0;
	virtual void setCaptureEnable(bool enable) = 0;
	virtual void setCaptureTrigger(unsigned short trigger) = 0;

protected:
	mutable WinCriticalSection m_params_CS;
	StreamParams m_streamParam;


	int m_model;

	FrameDataQueue* m_dataqueue[MAX_AMMOUNT_SENSOR];
	Statistic m_stat[MAX_AMMOUNT_SENSOR];
	Drawer* m_drawer[MAX_AMMOUNT_SENSOR];

private:
	static void recalculateSizeforH264(int & left, int& top, int& width, int& height);


	
	

	
	


	//cpSENSOR_WIDTH
	//cpSENSOR_HEIGHT
};