//////////////////////////////////////////////////////////////////////////
// DirectDrawer.h
//////////////////////////////////////////////////////////////////////////
#ifndef __DDRAW_DEC_H__
#define __DDRAW_DEC_H__

#include "GeneralTypes.h"
#include "colorspace.h"
#include <wingdi.h>


//=== for VS 7.1====
//#include <ddraw.h> 

// please remember!!! to built this application with VS 2005; 
//you might need to install directxsdk // http://www.google.com/search?hl=en&sa=X&oi=spell&resnum=0&ct=result&cd=1&q=directx+sdk&spell=1



// please try this 
#include "ddraw.h"  //for VS 2005 or 2008


// and also please check Linker=> additional directoties settings

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "ddraw.lib")

//==============================================================================
class CDirectDrawer
{
public:
	enum { DD_BORDER = 2 };
	CDirectDrawer();
	~CDirectDrawer();
	BOOL InitDirectDraw( HWND hWnd, bool bTryOverlay );
	void CloseDirectDraw();
	void BlitBitmap( const PRESENTATION_ITEM* pcPresentItem );
	void DDrawOnPaint();
	void DDrawOnSize(UINT state, int cx, int cy);
	void DDrawOnMove(int x, int y);
	void	 GetDDrawSizes(unsigned int & h, unsigned int & v);
	COLORREF GetBkGroundColor();
	void	 GetDisplayMode( int& iDisplayCx, int& iDisplayCy, int& iFrameRate );

private:
	BOOL CreateSurfaces(bool);
	void ShowMessage(int n, char * message, char * title);
	DWORD ConvertGDIColor(COLORREF dwGDIColor, LPDIRECTDRAWSURFACE7 pdds);
	bool GetDestRect(RECT * prcDst);
	bool CreateOverlaySurface(unsigned int width, unsigned int height, int type);
	bool CreateOffscreenSurface(unsigned int width, unsigned int height);
	void AdjustSizeForHardwareLimits(RECT * prcSrc, RECT * prcDst);

	static const COLORREF crDDrawBackgroundColor;
	LPDIRECTDRAW7        g_pDD;			// объект DirectDraw
	LPDIRECTDRAWSURFACE7 g_pDDSPrimary;	// первичная поверхность DirectDraw
	LPDIRECTDRAWSURFACE7 g_pDDSOffscr;	// внеэкранная поверхность
	LPDIRECTDRAWSURFACE7 g_pDDSBack;	// backbuffer внеэкранной поверхности
	DDOVERLAYFX          g_OverlayFX;
	DWORD                g_dwOverlayFlags;
	bool fUseOverlay;
	bool fEXCLUSIVE;
	DDCAPS drivercaps;
	DDCAPS helcaps;
// размеры видимой части изображения
// размеры изображения в окне 
	unsigned int h_size_wnd_ddraw;
	unsigned int v_size_wnd_ddraw;
// тип оверлейной поверхности
	int overlay_surface_type;
	HWND hDDrawWindow;

	ColorSpaceDll& m_csdll;
};
#endif // __DDRAW_DEC_H__
