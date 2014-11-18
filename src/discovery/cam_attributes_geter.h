#ifndef cam_attrgetter_2100
#define cam_attrgetter_2100

#include "camera.h"
#include "../common/Runnable.h"

// this class is designed as separate thread ;
// the prime task is to get cams details 
// input is list of cams 
// output - the same list with updated attributes 

class CamAttrGetter : public Runnable
{
public:
	CamAttrGetter(CameraPointerList* camlist):  m_camlist(camlist) { } ;
	~CamAttrGetter(){stop();}
protected:
	void run();

protected:
	CameraPointerList* m_camlist;
};



#endif //cam_attrgetter_2100