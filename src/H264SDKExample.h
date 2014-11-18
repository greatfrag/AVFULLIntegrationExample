// H264SDKExample.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CH264SDKExampleApp:
// See H264SDKExample.cpp for the implementation of this class
//

class CH264SDKExampleApp : public CWinApp
{
public:
	CH264SDKExampleApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CH264SDKExampleApp theApp;