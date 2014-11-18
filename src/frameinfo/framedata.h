#ifndef FrameData_h_1454
#define FrameData_h_1454

#include "stream.h"
#include "frame.h"
#include "frameinfo.h"

struct FrameData
{
	FrameData(int size, const void*src):
	uncm_data(size, src)
	{

	}


	Stream uncm_data;
	Frame frame; // smth to display
	FrameInfo info; 
};

#endif //Stream_h_1454