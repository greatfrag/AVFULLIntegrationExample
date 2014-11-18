#ifndef cam_discovery_1448
#define cam_discovery_1448

#include "camera.h"

// this function just search all cams; it gets only data from broadcast response;
CameraList findAllCameras();

// this function just check if we have more than one camera with such ip; if so bIPConflict is updated and warning
void checkConflicts(CameraPointerList& cams);

// this function will get all cams attributes( including DN, AI, model, description and so on )
void updateCamInfo(CameraPointerList& cams) ;

void clearCamAlerts(CameraPointerList& cams);

// check if ip is in use. 
bool isUsedIP(const std::string& ip) ;

// it will change cam ip to new one 
void changeCamIP(CameraInfo& cam, const std::string& new_ip) ;

// lock IP; it will lock/unlock cam ip if needed; return true if no error
bool lockCamIP(CameraInfo& cam, bool lock);

// check if camera is able to produce video
bool checkCamVideo(CameraInfo& cam);

bool setCamDescription(CameraInfo& cam, const std::string& descr);

bool setCamSaveToFlash(CameraInfo& cam);

bool setCamFactoryDefault(CameraInfo& cam);

//=================================================
CameraPointerList convertCamList(CameraList&);
void combineCamList(CameraPointerList& dst, CameraPointerList& addition);

#endif //cam_discovery_1448