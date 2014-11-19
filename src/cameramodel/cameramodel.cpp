#include "stdafx.h"
#include "cameramodel.h"

#include "../sdk_dll/AV2000Types.h"

void CameraModel::init()
{
	CameraOptions av2000(2000,1280,1024,false,1);
	add(av2000);

	CameraOptions av1300(AV1300,1280,1024,false,1);
	add(av1300);
	CameraOptions av1305(AV1305,1280,1024,true,1);
	add(av1305);


	CameraOptions av2100(AV2100,1600,1200,false,1);
	add(av2100);
	CameraOptions av2105(AV2105,1600,1200,true,1);
	add(av2105);

	CameraOptions av2805(AV2805,1920,1080,true,1);
	add(av2805);



	CameraOptions av3100(AV3100,2048,1536,false,1);
	add(av3100);
	CameraOptions av3105(AV3105,2048,1536,true,1);
	add(av3105);

	CameraOptions av5100(AV5100,2596,1944,false,1);
	add(av5100);
	CameraOptions av5105(AV5105,2596,1944,true,1);
	add(av5105);

	CameraOptions av10005(AV10005,3648,2752,true,1);
	add(av10005);



	CameraOptions av3130(AV3130,2048,1536,false,1);
	add(av3130);
	CameraOptions av3135(AV3135,2048,1536,true,1);
	add(av3135);


	CameraOptions av8180(AV8180,1600,1200,false,4);
	add(av8180);
	CameraOptions av8185(AV8185,1600,1200,true,4);
	add(av8185);

	CameraOptions av8360(AV8360,1600,1200,false,4);
	add(av8360);
	CameraOptions av8365(AV8365,1600,1200,true,4);
	add(av8365);

	CameraOptions av12186(AV12186,2048,1536,true,4);
	add(av12186);
	CameraOptions av12366(AV12366,2048,1536,true,4);
	add(av12366);

	CameraOptions av20185(AV20185,2560,1920,true,4);
	add(av20185);

	CameraOptions av20365(AV20365,2560,1920,true,4);
	add(av20365);

	CameraOptions av12176(AV12176,2048,1536,true,4);
	add(av12176);

	CameraOptions av20175(AV20175,2560,1920,true,4);
	add(av20175);

	CameraOptions av40185(AV40185,3648,2752,true,4);
	add(av40185);

	CameraOptions av5185(AV5185,1280,960,true,4);
	add(av5185);

}

bool CameraModel::isH264support(int model)
{
	return getCamoption(model).supportH264;
}

int CameraModel::getMaxSensorWidth(int model)
{
	return getCamoption(model).max_sensor_width;
}
int CameraModel::getMaxSensorHeight(int model)
{
	return getCamoption(model).max_sensor_height;
}

int CameraModel::getNumberOfSensors(int model)
{
	return getCamoption(model).number_of_sensors;
}

void CameraModel::add(CameraOptions cam)
{
	cams[cam.model] = cam;
}

CameraOptions CameraModel::getCamoption(int model)
{
	std::map<int, CameraOptions>::iterator it = cams.find(model);
	if (it==cams.end())
	{
		return cams[2000];
	}

	return it->second;
	
}

std::map<int, CameraOptions> CameraModel::cams;