#ifndef Runnable_h_
#define Runnable_h_

#include <windows.h>
#include "WinEvent.h"

class Runnable
{
public:
	Runnable();
	virtual void start();
	virtual void stop();

	virtual void pleaseToexit();
	
	bool isRuning() const;

	virtual bool needExit() const; 
	


	//never mind created thread or not.
	//never mind is it working or not
	//use the following values
	/*THREAD_PRIORITY_ABOVE_NORMAL
	THREAD_PRIORITY_BELOW_NORMAL
	THREAD_PRIORITY_HIGHEST
	THREAD_PRIORITY_IDLE  //-15
	THREAD_PRIORITY_LOWEST
	THREAD_PRIORITY_NORMAL
	THREAD_PRIORITY_TIME_CRITICAL */
	void setThreadPriority(int priority);

	virtual ~Runnable();
protected:
	virtual void run() = 0;
	
private:
	Runnable(const Runnable&) {};
	Runnable& operator = (Runnable&){};

	HANDLE m_Stream;
	static unsigned __stdcall StreamFunc(void *);
	WinEvent m_joinEvent;
	int mPriority;

	bool m_isRuning;
	volatile bool m_needExit;
	void join();

};

#endif //Runnable_h_