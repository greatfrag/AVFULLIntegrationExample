#ifndef cam_seacher_1746
#define cam_seacher_1746

#include "camera.h"
#include "../common/Runnable.h"


// this class is designed as separate thread only to search cameras; ( not difine properties );

class CamSearcher : public Runnable
{
public:
	CamSearcher(int max_delay = 500):m_max_delay(max_delay){};
	CameraList getCamList() const{return m_camList;};
protected:
	void run();

protected:
	int m_max_delay;
	CameraList m_camList;

};



#endif //cam_seacher_1746