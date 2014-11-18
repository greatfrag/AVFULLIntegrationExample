#include "stdafx.h"
#include ".\statistic.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <map>
#include <time.h>

#include <mmsystem.h>

#pragma comment (lib, "Winmm.lib")


#define UPDATE_WINDOW 2000 // statistic update window in ms

void Statistic::start()
{
	m_t0 = timeGetTime();
	m_bitrate = 0;
	m_fps = 0;
	m_frames = 0;
}

void Statistic::stop()
{

}

bool Statistic::Update()
{
	unsigned long tc = timeGetTime();
	unsigned long delta = tc - m_t0;
	if (delta >= UPDATE_WINDOW)
	{
		// we need to update statistics
		m_fps = (double)m_frames*1000.0/UPDATE_WINDOW;
		m_bitrate = (double)m_bytes*1000.0*8/UPDATE_WINDOW/1024/1024; // in megabits 

		m_frames = 0;
		m_bytes = 0;

		m_t0 = tc;

		return true;

	}


	return false;
}

void Statistic::addData(int dataSize)
{
	++m_frames;
	m_bytes+=dataSize;

}

double Statistic::getBitrate() const // in megabits per second
{
	return m_bitrate;
}

double Statistic::getFps() const
{
	return m_fps;
}

std::string Statistic::ToString()
{
	std::ostringstream os;
	os << "fps = " << m_fps << ";" << "bitrate = " << m_bitrate << " Mbps";
	return os.str();
}
