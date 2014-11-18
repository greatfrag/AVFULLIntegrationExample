#if !defined(_WinEvent_H_)
#define _WinEvent_H_

#include <WINDOWS.H>
class WinEvent
{
public:
	WinEvent(BOOL InitialState = false);
	BOOL SetEvent();
	void DropEvent();
	DWORD Wait(DWORD dwMilliseconds = INFINITE); // time-out interval // see WaitForSingleObject
	operator HANDLE() const; 
	virtual ~WinEvent();
private:
	WinEvent(const WinEvent&);
	WinEvent& operator=(const WinEvent& rEvent); 

	HANDLE Event;
};
#endif//_WinEvent_H_