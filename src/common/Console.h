// Console.h: interface for the CConsole class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSOLE_H__B2CDA5CB_E0D9_41B1_BC7D_796607F46DC3__INCLUDED_)
#define AFX_CONSOLE_H__B2CDA5CB_E0D9_41B1_BC7D_796607F46DC3__INCLUDED_


#include <sstream>
#include <string>
#include <fstream>
#include <time.h>
#include "WinCriticalSection.h"


class CConsole
{
public:
	static CConsole & getInstance();

	void setLogFile(const char*);
	void Print(const std::string& );
	void Print(const std::ostringstream& );

	void doVisualPrint();

private:
	CConsole();
	~CConsole();
	std::ofstream m_Stream;
	WinCriticalSection m_StreamMutex;
	bool m_visual;
};


#endif // !defined(AFX_CONSOLE_H__B2CDA5CB_E0D9_41B1_BC7D_796607F46DC3__INCLUDED_)
