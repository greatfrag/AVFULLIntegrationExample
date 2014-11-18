#include "stdafx.h"
#include ".\runnable.h"
#include <process.h>   


Runnable::Runnable()
:m_needExit(false),
m_isRuning(false),
m_Stream(NULL),
mPriority(THREAD_PRIORITY_NORMAL)
{
}

Runnable::~Runnable(void)
{
	//ASSERT(!isRuning()); // fogort to call stop in derived class????!!!
}

void Runnable::stop()
{
	pleaseToexit();
	join();
}

void Runnable::start()
{
	if (m_Stream)
		return; // already runing

	
	unsigned int dwID;
	m_needExit = false;
	m_isRuning = true;
	m_Stream = (HANDLE)_beginthreadex(NULL, 0, StreamFunc, this, 0, &dwID);
	SetThreadPriority(m_Stream, mPriority);
}

void Runnable::setThreadPriority(int priority)
{
	if (!isRuning()) return;

	if (m_Stream)
		SetThreadPriority(m_Stream, priority);
	mPriority = priority;
}

unsigned __stdcall Runnable::StreamFunc(void *This)
{
	
	Runnable* This_ = reinterpret_cast<Runnable*>(This);
	This_->run();
	This_->m_isRuning = false;
	This_->m_joinEvent.SetEvent();
	return 0;
}

bool Runnable::isRuning() const
{
	return m_isRuning;
}

void Runnable::pleaseToexit()
{
	m_needExit = true;
}

bool Runnable::needExit() const
{
	return m_needExit;
}

void Runnable::join()
{
	if (m_Stream)
	{
		m_joinEvent.Wait();
		CloseHandle(m_Stream);
		m_Stream = NULL;
	}
	
}