// WinCriticalSection.h: interface for the WinCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINCRITICALSECTION_H__615CBF55_A54D_4A43_B28E_C4B91C59D3AC__INCLUDED_)
#define AFX_WINCRITICALSECTION_H__615CBF55_A54D_4A43_B28E_C4B91C59D3AC__INCLUDED_

#include <WINDOWS.H>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class WinCriticalSection
{
public:
	WinCriticalSection();
	void Eter() {EnterCriticalSection(&Section);};
	void Leave(){LeaveCriticalSection(&Section);};
	virtual ~WinCriticalSection();
private:

	CRITICAL_SECTION Section;
};

class Mutex
{
public:
	inline Mutex(WinCriticalSection& CriticalSection)
	{
		this->CriticalSection = &CriticalSection;
		CriticalSection.Eter();
	}
	inline ~Mutex()
	{
		CriticalSection->Leave();
	}

private:
	WinCriticalSection* CriticalSection;

};

#endif // !defined(AFX_WINCRITICALSECTION_H__615CBF55_A54D_4A43_B28E_C4B91C59D3AC__INCLUDED_)
