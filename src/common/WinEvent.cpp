#include "stdafx.h"
#include "winevent.h"

WinEvent::WinEvent(BOOL InitialState)
{
	Event = CreateEvent(NULL, FALSE, InitialState, NULL);
}

BOOL WinEvent::SetEvent()
{
	return ::SetEvent(Event);
}

void WinEvent::DropEvent()
{
	if (Event)
		::WaitForSingleObject(Event,0);
}

DWORD WinEvent::Wait(DWORD dwMilliseconds )
{
	return ::WaitForSingleObject(Event,dwMilliseconds);
}

WinEvent::~WinEvent(void)
{
	CloseHandle(Event);
}

WinEvent::operator HANDLE() const
{
	return Event;
}

WinEvent::WinEvent(const WinEvent&)
{

}

WinEvent& WinEvent::operator=(const WinEvent& rEvent)
{
	return *this;
}