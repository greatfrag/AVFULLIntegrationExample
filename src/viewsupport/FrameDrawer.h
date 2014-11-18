#pragma once 

#include <string>

class Drawer
{
public:
	virtual void onStatistic(const std::string& str)=0;
	virtual void DrawFrame(Frame* frame) = 0;
};