#ifndef ThreadQueue_h_2236
#define ThreadQueue_h_2236 

#include "WinCriticalSection.h"
#include "Semaphore.h"
#include <queue>
#include <limits>

template< typename _T >
class ThreadQueue
{
public:
	typedef typename std::queue<_T>::size_type size_type;

	//ThreadQueue( size_type maxSize = std::numeric_limits< size_type >::max() ):
	ThreadQueue( size_type maxSize = 100000 ):
	m_maxSize( maxSize )
	{

	}

	bool push(const _T& val) 

	{ 
		Mutex mutex (m_cs);

		if ( m_queue.size()>=m_maxSize )
			return false;

		m_queue.push(val); 
		m_sem.setUp();

		return true;

	}

	
	bool pop(_T& val, int time = INFINITE)
	{

		m_sem.wait(time);

		Mutex mutex (m_cs);
		if( !m_queue.empty() ) 
		{
			val = m_queue.front();  
			m_queue.pop();
			return true; 
		}

		return false;

		//return _T();// bad result


	}

	void clear()
	{
        Mutex mutex (m_cs);
		while (!m_queue.empty()) 
		{
			delete m_queue.front();  
			m_queue.pop();
		}
	}

	int size() const
	{
		Mutex mutex (m_cs);
		return m_queue.size();
	}

	int maxSize() const
	{
		return m_maxSize;
	}

	~ThreadQueue()
	{
		clear();
	}
private:
	std::queue<_T> m_queue;//synchronizeed
	size_type m_maxSize;
	mutable WinCriticalSection m_cs;
	Semaphore m_sem;

};

#endif //ThreadQueue_h_2236