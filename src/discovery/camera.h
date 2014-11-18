#ifndef camera_h_1612
#define camera_h_1612

#include <map>
#include <string>
#include <list>

typedef std::list<std::string> AlertList;

struct CameraInfo
{
	CameraInfo()
	{
		checked=-1; 
		bIPConflict = false;
		model = 2000; // unknown
		firmware = 0;
		iplocked = 0;
		description_updated = false;
		iris_present = false;
		switch_present = false;
		mega_dome = false;
		
	}

	char macc[6];
	std::string mac;
	std::string ip;
	unsigned int model;
	bool mini, iris_present, switch_present, iplocked, mega_dome;
	unsigned int firmware;
	std::string description;
	bool description_updated; // just to make it faster;

	AlertList alert;



	int checked;  // "-1" - no verify, "0" - not accessible, "1" - accesible
	bool bIPConflict;

	

};

typedef std::map<std::string, CameraInfo> CameraList;
typedef std::map<std::string, CameraInfo*> CameraPointerList;





#endif //camera_h_1612,