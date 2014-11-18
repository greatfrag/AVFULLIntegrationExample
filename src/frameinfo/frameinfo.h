#ifndef FrameInfo_h_1454
#define FrameInfo_h_1454

#include "../sdk_dll/AV2000Types.h"

enum FrameType {FrameType_P, FrameType_I };


struct FrameInfo
{
	int width;
	int height;
	FrameType frametype;
	AVCodecType codecType;
};

#endif //Stream_h_1454