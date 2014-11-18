#ifndef Semaphore_h_2055
#define Semaphore_h_2055


class Semaphore
{
public:
	Semaphore()
	{
		m_sem = CreateSemaphore(NULL,0,10000,NULL);
	}

	inline void wait(int time = INFINITE)
	{
		WaitForSingleObject(m_sem, time );
	}

	inline void setUp()
	{
		ReleaseSemaphore(m_sem,1,NULL);
	}

	~Semaphore()
	{
		CloseHandle(m_sem);
	}
private:
	HANDLE m_sem;
};


#endif //Semaphore_h_2055