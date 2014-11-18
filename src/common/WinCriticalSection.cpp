// WinCriticalSection.cpp: implementation of the WinCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "WinCriticalSection.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//=======================================
WinCriticalSection::WinCriticalSection()
{
	InitializeCriticalSection(&Section);
}
//=======================================
WinCriticalSection::~WinCriticalSection()
{
	DeleteCriticalSection(&Section);
}
//==========================================================
//==========================================================
