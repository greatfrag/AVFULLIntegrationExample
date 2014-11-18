// Console.cpp: implementation of the CConsole class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\console.h"
#include <direct.h>
#include <iostream>

CConsole::CConsole():
m_visual(false)
{
}

CConsole::~CConsole()
{
	m_Stream.close();
}


void CConsole::setLogFile(const char* FileName)
{
	Mutex mutex(m_StreamMutex);
	m_Stream.open(FileName, std::ios_base::app);
}

CConsole & CConsole::getInstance()
{
	static CConsole inst;
	return inst;
}

void CConsole::Print(const std::ostringstream& msg)
{
	Print(msg.str());
}

void CConsole::Print(const std::string& msg)
{
	
	time_t curentTime;
	Mutex mutex(m_StreamMutex);

	if (m_visual)
		std::cout<<msg<<std::endl;

	if (!m_Stream.is_open()) return;
	::time(&curentTime);
	tm* tm = ::localtime(&curentTime);
	char timeBuf[40];
	::strftime(timeBuf, 40, "%d.%m.%Y %H:%M:%S", tm);
	if (m_Stream.is_open())
		m_Stream << timeBuf << ": " << msg.c_str() << std::endl;
}

void CConsole::doVisualPrint()
{
	m_visual = true;;
}