#ifndef cameramodel_h_115
#define cameramodel_h_115

#include <map>



struct CameraOptions
{
	CameraOptions(){};
	CameraOptions (int model_, int max_sensor_width_, int max_sensor_height_, bool supportH264_, int nos):
	model(model_), max_sensor_width(max_sensor_width_), max_sensor_height(max_sensor_height_), supportH264(supportH264_), number_of_sensors(nos)
	{

	}
	int model;
	int max_sensor_width;
	int max_sensor_height;
	bool supportH264;
	int number_of_sensors;

};

class CameraModel
{
public:
	static void init();

	static bool isH264support(int model);
	static int getMaxSensorWidth(int model);
	static int getMaxSensorHeight(int model);
	static int getNumberOfSensors(int model);
private:

	static void add(CameraOptions cam);
	static CameraOptions getCamoption(int model);
	static std::map<int, CameraOptions> cams;

};

#endif //cameramodel_h_115