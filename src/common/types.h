#ifndef common_types_1143
#define common_types_1143

#include "ThreadQueue.h"
#include "../frameinfo/framedata.h"

typedef ThreadQueue<FrameData* > FrameDataQueue ;
#define max_framequeue_size 5

#endif//_WinEvent_H_ 