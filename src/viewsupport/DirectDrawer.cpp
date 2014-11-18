//////////////////////////////////////////////////////////////////////////
// DirectDrawer.cpp
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <memory.h>
#include <stdio.h>
#include "DirectDrawer.h"
//#include "ColorSpace/colorspace.h"

//======================================================================================
#define FRAME_SHIFT_WIDTH 0
#define FRAME_SHIFT_HEIGHT 0
#define USED_OVL_TYPE 0

//======================================================================================
CDirectDrawer::CDirectDrawer():
m_csdll(ColorSpaceDll::instance())
{
	g_pDD             = NULL; 
	g_pDDSPrimary     = NULL; 
	g_pDDSOffscr      = NULL; 
	g_pDDSBack        = NULL; 
	g_OverlayFX;
	g_dwOverlayFlags  = 0;
	fUseOverlay = false;
	fEXCLUSIVE = false;

	h_size_wnd_ddraw = 0;
	v_size_wnd_ddraw = 0;

	overlay_surface_type = -1;
	hDDrawWindow = NULL;
}


//======================================================================================
CDirectDrawer::~CDirectDrawer()
{
	CloseDirectDraw();
}

//======================================================================================
void CDirectDrawer::ShowMessage(int n, char * message, char * title)
{
}

//======================================================================================
void CDirectDrawer::GetDDrawSizes(unsigned int & h, unsigned int & v)
{
	h = h_size_wnd_ddraw;
	v = v_size_wnd_ddraw;
}

//======================================================================================
BOOL CDirectDrawer::InitDirectDraw( HWND hWnd, bool bTryOverlay )
{
	HRESULT hres;

	if(!hWnd)
	{
		ShowMessage(0, "InitDirectDraw: Invalid Window Handle", "Error");
		return FALSE;
	}

	hDDrawWindow = hWnd;

	
	hres = DirectDrawCreateEx(NULL, (void**)&g_pDD, IID_IDirectDraw7, NULL);

	if(hres != DD_OK)
	{
		ShowMessage(0, "InitDirectDraw: DirectDrawCreateEx", "Error");
		return FALSE;
	}

	if( fEXCLUSIVE )
	{
		hres = g_pDD->SetCooperativeLevel( hWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
		if(hres != DD_OK)
		{
			ShowMessage(0, "InitDirectDraw: SetCooperativeLevel", "Error");
			g_pDD->Release();
			return FALSE;
		}

		hres = g_pDD->SetDisplayMode( 1024, 768, 32, 0, 0 );	

		if(hres != DD_OK)
		{
			ShowMessage(0, "SetDisplayMode", "Error");
			return FALSE;
		}
	}
	else
	{
		hres = g_pDD->SetCooperativeLevel( hWnd, DDSCL_NORMAL);

		if(hres != DD_OK)
		{
			ShowMessage(0, "InitDirectDraw: SetCooperativeLevel", "Error");
			g_pDD->Release();
			return FALSE;
		}
	}

	//colorspace_init( 1 ); // 0 - none, 1 - mmx, 2 - xmm
	CreateSurfaces( bTryOverlay );

	return TRUE;
}

	//======================================================================================
void CDirectDrawer::CloseDirectDraw(void)
{
	if(g_pDD)
	{
		if( fEXCLUSIVE ) 
		g_pDD->RestoreDisplayMode();

		g_pDD->Release();
		g_pDD = NULL;
	}

	if(g_pDDSPrimary)
	{
		g_pDDSPrimary->Release();
		g_pDDSPrimary = NULL;
	}

	if(g_pDDSOffscr)
	{
		g_pDDSOffscr->Release();
		g_pDDSOffscr = NULL;
	}

}

//======================================================================================
DWORD CDirectDrawer::ConvertGDIColor(COLORREF dwGDIColor, LPDIRECTDRAWSURFACE7 pdds)
{
	if(pdds == NULL) 
		return 0;

	COLORREF       rgbT;
	HDC            hdc;
	DWORD          dw = CLR_INVALID;
	DDSURFACEDESC2 ddsd;
	HRESULT        hr;

  //  Use GDI SetPixel to color match for us
	if((dwGDIColor != CLR_INVALID) && (pdds->GetDC(&hdc) == DD_OK))
	{
		rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
		SetPixel(hdc, 0, 0, dwGDIColor);       // Set our value
		pdds->ReleaseDC(hdc);
	}

	// Now lock the surface so we can read back the converted color
	ddsd.dwSize = sizeof(ddsd);
	hr = pdds->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);

	if(hr == DD_OK)
	{
		dw = *(DWORD *)ddsd.lpSurface; 
		if(ddsd.ddpfPixelFormat.dwRGBBitCount < 32) // Mask it to bpp
			dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;

		pdds->Unlock(NULL);
	}

	//  Now put the color that was there back.
	if((dwGDIColor != CLR_INVALID) && (pdds->GetDC(&hdc) == DD_OK))
	{
		SetPixel(hdc, 0, 0, rgbT);
		pdds->ReleaseDC(hdc);
	}

	return dw;
}

//======================================================================================
BOOL CDirectDrawer::CreateSurfaces(bool fTryOverlay)
{
	HRESULT hres;
	DDSURFACEDESC2  ddsd;

	if(g_pDDSPrimary)
	{
		g_pDDSPrimary->Release();
		g_pDDSPrimary = NULL;
	}

	if(g_pDDSOffscr)
	{
		g_pDDSOffscr->Release();
		g_pDDSOffscr = NULL;
	}

	//  horizontal_size_value_ddraw = 0;
	//  vertical_size_value_ddraw   = 0;
	fUseOverlay = false;
	
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	hres = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);

	if(hres != DD_OK)
	{
		ShowMessage(0, "CreateSurfaces: Primary CreateSurface", "Error");
		return FALSE;
	}

	// Get driver capabilities to determine overlay support.
	ZeroMemory(&drivercaps, sizeof(drivercaps));
	drivercaps.dwSize = sizeof(drivercaps);
	ZeroMemory(&helcaps, sizeof(helcaps));
	helcaps.dwSize = sizeof(helcaps);
	g_pDD->GetCaps(&drivercaps, &helcaps);
	// Does the driver support overlays in the current mode? 
	// The DirectDraw emulation layer does not support overlays
	// so overlay related APIs will fail without hardware support.

	if(drivercaps.dwCaps & DDCAPS_OVERLAY)
	{
		// Make sure it supports stretching (scaling)
		if(drivercaps.dwCaps & DDCAPS_OVERLAYSTRETCH)
			ShowMessage(0, "DirectDraw support overlay stretching !", "Message");
		else
			ShowMessage(0, "DirectDraw NOT support overlay stretching !", "Error");

		if(fTryOverlay) 
		{
			fUseOverlay = true;
		}
	}
	else
		ShowMessage(0, "DirectDraw NOT supports overlay at all !!!", "Error");
	/*
	if(drivercaps.dwFXCaps & DDFXCAPS_BLTARITHSTRETCHY)
		ShowMessage(0, "HAL: DDFXCAPS_BLTARITHSTRETCHY OK", 0);

	if(helcaps.dwFXCaps & DDFXCAPS_BLTARITHSTRETCHY)
		ShowMessage(0, "HEL: DDFXCAPS_BLTARITHSTRETCHY OK", 0);
	/**/

	
	DWORD dwNum = 0;
	hres = g_pDD->GetFourCCCodes(&dwNum, NULL);

	if(hres != DD_OK)
	{
		ShowMessage(0, "CreateSurfaces: GetFourCCCodes error 1", "Error");
		return FALSE;
	}

	/*
	if(dwNum > 0)
	{
		DWORD * pdwCodes = new DWORD[dwNum];

		if(!pdwCodes)
			ShowMessage(0, "FourCC codes array creation fail!", NULL);
		else
		{
			hres = g_pDD->GetFourCCCodes(&dwNum, pdwCodes);
			if(hres != DD_OK)
			{
				ShowMessage(0, "CreateSurfaces: GetFourCCCodes error 2", "Error");
				return FALSE;
			}
			char sz[256];
			sprintf(sz, "DirectDraw supports %u FourCC codes:", dwNum);
			ShowMessage(0, sz, NULL);
			for(DWORD i = 0; i < dwNum; i++)
			{
				sprintf(sz, "  %c%c%c%c", pdwCodes[i]&255, (pdwCodes[i]>>8)&255, (pdwCodes[i]>>16)&255, (pdwCodes[i]>>24)&255);
				ShowMessage(0, sz, NULL);
			}
		}
	}
	/**/

	if(fUseOverlay)
	{
		// Setup effects structure
		ZeroMemory(&g_OverlayFX, sizeof(g_OverlayFX));
		g_OverlayFX.dwSize = sizeof(g_OverlayFX);
		//g_OverlayFX.dwDDFX = DDOVERFX_ARITHSTRETCHY | DDOVERFX_MIRRORLEFTRIGHT | DDOVERFX_MIRRORUPDOWN;

		// Setup overlay flags.
		g_dwOverlayFlags = DDOVER_SHOW;
	}

	/**/
	if(fUseOverlay && (drivercaps.dwCKeyCaps & DDCKEYCAPS_DESTOVERLAY))
	{
		// Using a color key will clip the overlay 
		// when the mouse or other windows go on top of us. 
		// The color key can be any color, but a near black (not exactly) allows
		// the cursor to move around on the window without showing off the
		// color key, and also clips windows with exactly black text.
		DWORD dw = ConvertGDIColor(crDDrawBackgroundColor, g_pDDSPrimary);
		g_OverlayFX.dckDestColorkey.dwColorSpaceLowValue  = dw;
		g_OverlayFX.dckDestColorkey.dwColorSpaceHighValue = dw;
		g_dwOverlayFlags |= DDOVER_DDFX | DDOVER_KEYDESTOVERRIDE;
	}
	else if(!fEXCLUSIVE)
	{
	
		LPDIRECTDRAWCLIPPER pClipper = NULL;
		hres = g_pDD->CreateClipper(0, &pClipper, NULL);
		if(hres != DD_OK)
		{
			ShowMessage(0, "CreateSurfaces: CreateClipper", "Error");
			return FALSE;
		}

		hres = pClipper->SetHWnd(0, hDDrawWindow);
		if(hres != DD_OK)
		{
			ShowMessage(0, "CreateSurfaces: SetHwnd", "Error");
			pClipper->Release();
			return FALSE;
		}

		hres = g_pDDSPrimary->SetClipper(pClipper);
		if(hres != DD_OK)
		{
			ShowMessage(0, "CreateSurfaces: SetClipper", "Error");
			pClipper->Release();
			return FALSE;
		}

		pClipper->Release();
	}
	/**/
	return TRUE;
}

//======================================================================================
void CDirectDrawer::GetDisplayMode( int& iDisplayCx, int& iDisplayCy, int& iFrameRate )
{
	DDSURFACEDESC2 display_mode;
	display_mode.dwFlags =  DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | 
							DDSD_PIXELFORMAT | DDSD_PITCH;
    HRESULT hres = g_pDD->GetDisplayMode( &display_mode );
    if(hres != DD_OK)
    {
      ShowMessage(0, "SetDisplayMode", "Error");
      return;
    }
	iDisplayCx = display_mode.dwWidth;
	iDisplayCx = display_mode.dwHeight;
	iFrameRate = 0;
//	iFrameRate = display_mode.dwFrameRate;
}

//======================================================================================
bool CDirectDrawer::GetDestRect(RECT * prcDst)
{
	RECT rect;
	GetWindowRect( hDDrawWindow, &rect );
	int i_width = rect.right - rect.left;
	int i_height = rect.bottom - rect.top;
	float h_Dem = (float)i_width/( (float)h_size_wnd_ddraw );
	float v_Dem = (float)i_height/((float)v_size_wnd_ddraw );
	float dem = ( h_Dem > v_Dem ) ? v_Dem : h_Dem;
	*prcDst = rect;
	int i_h_rest = i_width - (int)( h_size_wnd_ddraw*dem+1 );
	int i_v_rest = i_height - (int)( v_size_wnd_ddraw*dem+1 );
	rect.left	+= i_h_rest/2;
	rect.top	+= i_v_rest/2;
	rect.right	-= i_h_rest/2;
	rect.bottom	-= i_v_rest/2;
	*prcDst = rect;
	return true;
}

//======================================================================================
bool CDirectDrawer::CreateOverlaySurface(unsigned int width, unsigned int height, int type)
{
	HRESULT        hres;
	DDSURFACEDESC2 ddsd;
	DDSCAPS2       ddscaps;

	if(!g_pDD)
	{
		ShowMessage(0, "CreateOverlaySurface: DDraw object absent", "Error");
		return false;
	}

	if(g_pDDSOffscr)
	{
		g_pDDSOffscr->Release();
	}

	// Setup the overlay surface's attributes in the surface descriptor
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize            = sizeof(ddsd);
	ddsd.dwSize            = sizeof(ddsd);
	ddsd.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | 
							DDSD_PIXELFORMAT | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps    = DDSCAPS_OVERLAY | DDSCAPS_VIDEOMEMORY | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
	ddsd.dwBackBufferCount = 1;
	ddsd.dwWidth           = width;
	ddsd.dwHeight          = height;
	/**/
	if(type == 0)
	{
		ddsd.ddpfPixelFormat.dwFlags  = DDPF_FOURCC;
		ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y','V','1','2');
		//ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('U','Y','V','Y');
		//ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y','U','Y','2');
		//ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('I','Y','U','V');//-
		//ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y','U','V','2');-
		//ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y','U','V','Y');-
		//ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y','V','U','9');
		//ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('V','4','2','2');- 
		//V422
		
	}
	else
	{
		ddsd.ddpfPixelFormat.dwFlags  = DDPF_RGB;
		ddsd.ddpfPixelFormat.dwYUVBitCount = 32;
	}
	/*
	ddsd.ddpfPixelFormat.dwFlags  = DDPF_YUV;
	ddsd.ddpfPixelFormat.dwYUVBitCount = 24;
	//ddsd.ddpfPixelFormat.dwYBitMask    = ddsd.ddpfPixelFormat.dwRBitMask;
	//ddsd.ddpfPixelFormat.dwUBitMask    = ddsd.ddpfPixelFormat.dwGBitMask;
	//ddsd.ddpfPixelFormat.dwVBitMask    = ddsd.ddpfPixelFormat.dwBBitMask;
	/**/

	// Attempt to create the surface with theses settings
	hres = g_pDD->CreateSurface(&ddsd, &g_pDDSOffscr, NULL);
	if(hres != DD_OK)
	{
		ShowMessage(0, "CreateOverlaySurface: CreateSurface overlay", "Error");
		return false;
	}

	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	hres = g_pDDSOffscr->GetAttachedSurface(&ddscaps, &g_pDDSBack);
	if(hres != DD_OK)
	{
		ShowMessage(0, "CreateOverlaySurface: GetAttachedSurface", "Error");
		return false;
	}

	ShowMessage(0, "CreateOverlaySurface OK", "Message");

	return true;
}

//======================================================================================
bool CDirectDrawer::CreateOffscreenSurface(unsigned int width, unsigned int height)
{
	HRESULT        hres;
	DDSURFACEDESC2 ddsd;
	//DDSCAPS2       ddscaps;


	if(g_pDDSOffscr)
	{
		hres = g_pDDSOffscr->Release();
		if(hres != DD_OK)
		{
			ShowMessage(0, "CreateOffscreenSurface: Release offscreen surface", "Error");
			return false;
		}
		g_pDDSOffscr = NULL;
	}

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;
	ddsd.dwHeight = height;
	ddsd.dwWidth  = width;
	hres = g_pDD->CreateSurface(&ddsd, &g_pDDSOffscr, NULL);

	if(hres != DD_OK)
	{
		ShowMessage(0, "CreateOffscreenSurface: OffscreenLocalVideo CreateSurface", "Error");

		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | DDSCAPS_NONLOCALVIDMEM;
		hres = g_pDD->CreateSurface(&ddsd, &g_pDDSOffscr, NULL);
		if(hres != DD_OK)
		{
			ShowMessage(0, "CreateOffscreenSurface: OffscreenNonLocalVideo CreateSurface", "Error");

			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			hres = g_pDD->CreateSurface(&ddsd, &g_pDDSOffscr, NULL);

			if(hres != DD_OK)
			{
				ShowMessage(0, "CreateOffscreenSurface: OffscreenSystem CreateSurface", "Error");
				return false;
			}
		}
	}

	ShowMessage(0, "CreateOffscreenSurface OK", "Message");

	return true;
}

//======================================================================================

void CDirectDrawer::BlitBitmap( const PRESENTATION_ITEM* pcPI )
{
  DDSURFACEDESC2 ddsd;
  HRESULT hres;

//----------------------------------------------------

	if(!IsWindowVisible(hDDrawWindow)) return;


	if(!g_pDDSPrimary)
	{
		ShowMessage(0, "BlitBitmap: No primary surface", "Error");
		return;
	}


	int i_h_size, i_v_size;
	i_h_size = pcPI->i_data_len0;
	i_v_size = pcPI->i_data_len1;


	if( ( v_size_wnd_ddraw != (uint)i_v_size ) ||
		( h_size_wnd_ddraw != (uint)i_h_size ) ||
		( overlay_surface_type != USED_OVL_TYPE ) )
	{
		bool b;
		if(fUseOverlay)
		{
			b = CreateOverlaySurface( i_h_size, i_v_size, USED_OVL_TYPE );
			if(!b)
			{
				if(CreateSurfaces(false))
					b = CreateOffscreenSurface( i_h_size, i_v_size);
			}
		}
		else
			b = CreateOffscreenSurface( i_h_size, i_v_size);


		if(b)
		{
			overlay_surface_type	= USED_OVL_TYPE;
			v_size_wnd_ddraw		= i_v_size;
			h_size_wnd_ddraw		= i_h_size;
		}
	}


	if(!g_pDDSOffscr)
	{
		ShowMessage(0, "BlitBitmap: No offscreen surface", "Error");
		return;
	}


	if(g_pDDSPrimary->IsLost() || g_pDDSOffscr->IsLost())
	{
		hres = g_pDD->RestoreAllSurfaces();
		if(hres != DD_OK)
		{
			ShowMessage(0, "BlitBitmap: Surfaces Restore", "Error");
			CreateSurfaces(fUseOverlay);
			return;
		}
	}

	LPDIRECTDRAWSURFACE7 g_pDDS = (fUseOverlay) ? g_pDDSBack : g_pDDSOffscr;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	hres = g_pDDS->Lock(0, &ddsd, DDLOCK_WAIT, 0);

	if(hres != DD_OK)
	{
		ShowMessage(0, "BlitBitmap: Lock", "Error");
		return;
	}

	if(fUseOverlay)
	{
		if(USED_OVL_TYPE == 0)
		{
			char * p1 = (char *)ddsd.lpSurface;
			char * p2 = (char *)pcPI->Y;
			int h = i_h_size;
			int h_stride = pcPI->i_data_len2;
			//int chroma_h = (i_h_size>>1)*pcPI->i_other_data0;
			int v = i_v_size;
			int l = ddsd.lPitch;
			int i;
			for(i = 0; i < v; i++, p1 += l, p2 += h_stride ) memcpy(p1, p2, h);
			p2 = (char *)pcPI->V;

			h /= 2;
			v /= 2;
			l /= 2;

			int chroma_h = h_stride/2;

			if (pcPI->i_other_data0==1)
			{
				for(i = 0; i < v; i++, p1 += l, p2 += chroma_h) memcpy(p1, p2, h);
				p2 = (char *)pcPI->U;
				for(i = 0; i < v; i++, p1 += l, p2 += chroma_h) memcpy(p1, p2, h);
			}
			else
			{
				for(i = 0; i < v; i++, p1 += l, p2 += 2*chroma_h) memcpy(p1, p2, h);
				p2 = (char *)pcPI->U;
				for(i = 0; i < v; i++, p1 += l, p2 += 2*chroma_h) memcpy(p1, p2, h);
			}


		}
		else
		{
			int uv_stride = ( pcPI->i_data_len2>>1) * pcPI->i_other_data0;
			//yv12_to_rgb32( (byte*)ddsd.lpSurface, ddsd.lPitch/4, pcPI->Y, pcPI->U, pcPI->V, 
			//			   pcPI->i_data_len2, uv_stride, i_h_size, i_v_size );
		}
	}
	else
	{
				
		int uv_stride = ( pcPI->i_data_len2>>1) * pcPI->i_other_data0;

		if((ddsd.ddpfPixelFormat.dwRGBBitCount == 16) && (ddsd.ddpfPixelFormat.dwGBitMask == 0x03E0))
			m_csdll.yuv12_2_rgb555((byte*)ddsd.lpSurface, ddsd.lPitch/2, pcPI->Y, pcPI->U, pcPI->V, i_h_size, uv_stride, i_h_size, i_v_size);

		else if((ddsd.ddpfPixelFormat.dwRGBBitCount == 16) && (ddsd.ddpfPixelFormat.dwGBitMask == 0x07E0))
			m_csdll.yuv12_2_rgb565((byte*)ddsd.lpSurface, ddsd.lPitch/2, pcPI->Y, pcPI->U, pcPI->V, i_h_size, uv_stride, i_h_size, i_v_size);

		else if(ddsd.ddpfPixelFormat.dwRGBBitCount == 24)
			m_csdll.yuv12_2_rgb24((byte*)ddsd.lpSurface, ddsd.lPitch/3, pcPI->Y, pcPI->U, pcPI->V, i_h_size, uv_stride, i_h_size, i_v_size);

		else if(ddsd.ddpfPixelFormat.dwRGBBitCount == 32)
		//yv12_to_rgb32((byte*)ddsd.lpSurface, ddsd.lPitch/4, pcPI->Y, pcPI->U, pcPI->V, i_h_size, uv_stride, i_h_size, i_v_size);
			m_csdll.yuv12_2_rgb32((byte*)ddsd.lpSurface, ddsd.lPitch/4, pcPI->Y, pcPI->U, pcPI->V, pcPI->i_data_len2, uv_stride, i_h_size, i_v_size);
		/**/
	}

  
	hres = g_pDDS->Unlock(0);
	if(hres != DD_OK)
	{
		ShowMessage(0, "BlitBitmap: Unlock", "Error");
		return;
	}

	if(fUseOverlay) if(g_pDDSOffscr->Flip(NULL, DDFLIP_WAIT) != DD_OK)
	{
		ShowMessage(0, "BlitBitmap: Flip", "Error");
		return;
	}

	DDrawOnPaint();
}

//======================================================================================
// Desc: Checks and corrects all boundries for alignment and stretching
void CDirectDrawer::AdjustSizeForHardwareLimits(RECT * prcSrc, RECT * prcDst)
{
  // Setup effects structure
  // Make sure the coordinates fulfill the stretching requirements.  Often
  // the hardware will require a certain ammount of stretching to do
  // overlays. This stretch factor is held in dwMinOverlayStretch as the
  // stretch factor multiplied by 1000 (to keep an accuracy of 3 decimal
  // places).
  DWORD dwXRatio = (prcDst->right - prcDst->left) * 1000 / (prcSrc->right - prcSrc->left);
  DWORD dwYRatio = (prcDst->bottom - prcDst->top) * 1000 / (prcSrc->bottom - prcSrc->top);
  if( (drivercaps.dwCaps & DDCAPS_OVERLAYSTRETCH) && 
      (drivercaps.dwMinOverlayStretch)            && 
      (dwXRatio < drivercaps.dwMinOverlayStretch) )
  {
      // Window is too small
      prcDst->right = prcDst->left +
        ((prcSrc->right - prcSrc->left)*(drivercaps.dwMinOverlayStretch + 1))/1000;
      if(!fEXCLUSIVE) prcDst->right += 2*GetSystemMetrics(SM_CXSIZEFRAME);
  }

  if( (drivercaps.dwCaps & DDCAPS_OVERLAYSTRETCH) && 
      (drivercaps.dwMaxOverlayStretch)            && 
      (dwXRatio > drivercaps.dwMaxOverlayStretch) )
  {
      // Window is too large
      prcDst->right = prcDst->left +
        ((prcSrc->bottom - prcSrc->top)*(drivercaps.dwMaxOverlayStretch + 999))/1000;
      if(!fEXCLUSIVE) prcDst->right += 2*GetSystemMetrics(SM_CXSIZEFRAME);
  }

  // Recalculate the ratio's for the upcoming calculations
  dwXRatio = (prcDst->right  - prcDst->left) * 1000 / (prcSrc->right  - prcSrc->left);
  dwYRatio = (prcDst->bottom - prcDst->top)  * 1000 / (prcSrc->bottom - prcSrc->top);

  // Check to make sure we're within the screen's boundries, if not then fix
  // the problem by adjusting the source rectangle which we draw from.
  if(prcDst->left < 0)
  {
    prcSrc->left = -prcDst->left * 1000 / dwXRatio;
    prcDst->left = 0;
  }

  if(prcDst->right > GetSystemMetrics(SM_CXSCREEN))
  {
    prcSrc->right = (prcSrc->right  - prcSrc->left) -
      ((prcDst->right - GetSystemMetrics(SM_CXSCREEN))*1000/dwXRatio);
    prcDst->right = GetSystemMetrics(SM_CXSCREEN);
  }

  if(prcDst->bottom > GetSystemMetrics(SM_CYSCREEN))
  {
    prcSrc->bottom = (prcSrc->bottom - prcSrc->top) -
      ((prcDst->bottom - GetSystemMetrics(SM_CYSCREEN))*1000/dwYRatio);
    prcDst->bottom = GetSystemMetrics(SM_CYSCREEN);
  }

  if(prcDst->top < 0)
  {
    prcSrc->top = -prcDst->top * 1000 / dwYRatio;
    prcDst->top = 0;
  }

  // Make sure the coordinates fulfill the alignment requirements
  // these expressions (x & -y) just do alignment by dropping low order bits...
  // so to round up, we add first, then truncate.
  if( (drivercaps.dwCaps & DDCAPS_ALIGNBOUNDARYSRC) && 
      (drivercaps.dwAlignBoundarySrc) )
  {
      prcSrc->left = (prcSrc->left + drivercaps.dwAlignBoundarySrc / 2) & 
                      -(signed) (drivercaps.dwAlignBoundarySrc);
  }

  if( (drivercaps.dwCaps & DDCAPS_ALIGNSIZESRC) && 
      (drivercaps.dwAlignSizeSrc) )
  {
      prcSrc->right = prcSrc->left + (prcSrc->right - prcSrc->left + drivercaps.dwAlignSizeSrc / 2) & 
                       -(signed) (drivercaps.dwAlignSizeSrc);
  }

  if( (drivercaps.dwCaps & DDCAPS_ALIGNBOUNDARYDEST) && 
      (drivercaps.dwAlignBoundaryDest) )
  {
      prcDst->left = ( prcDst->left + drivercaps.dwAlignBoundaryDest / 2 ) & 
                     -(signed) (drivercaps.dwAlignBoundaryDest);
  }

  if( (drivercaps.dwCaps & DDCAPS_ALIGNSIZEDEST) && 
      (drivercaps.dwAlignSizeDest) )
  {
      prcDst->right = prcDst->left + (prcDst->right - prcDst->left) & 
                      -(signed) (drivercaps.dwAlignSizeDest);
  }
}

//======================================================================================
void CDirectDrawer::DDrawOnPaint()
{
	if(!g_pDDSPrimary || !g_pDDSOffscr) return;

	RECT rcDst;
	if(!GetDestRect(&rcDst)) return;

	if(fUseOverlay)
	{
		if(rcDst.top == rcDst.bottom)
			g_pDDSOffscr->UpdateOverlay(NULL, g_pDDSPrimary, NULL, DDOVER_HIDE, NULL);
		else
		{
			RECT rcSrc = {0, 0, h_size_wnd_ddraw, v_size_wnd_ddraw};
			AdjustSizeForHardwareLimits(&rcSrc, &rcDst);
			g_pDDSOffscr->UpdateOverlay(&rcSrc, g_pDDSPrimary, &rcDst, g_dwOverlayFlags, &g_OverlayFX);
		}
	}
	else
	{
		if(rcDst.top == rcDst.bottom) return;
		RECT rcSrc = { 0, 0, h_size_wnd_ddraw, v_size_wnd_ddraw};
		g_pDDSPrimary->Blt(&rcDst, g_pDDSOffscr, &rcSrc, DDBLT_WAIT, NULL);
	}
}

//======================================================================================
void CDirectDrawer::DDrawOnSize(UINT state, int cx, int cy)
{
	if((cx > 0) && (cy > 0))
		DDrawOnPaint();
}

//======================================================================================
void CDirectDrawer::DDrawOnMove(int x, int y)
{
	DDrawOnPaint();
}

//======================================================================================
COLORREF CDirectDrawer::GetBkGroundColor( )
{
	return crDDrawBackgroundColor;
}

//======================================================================================
const COLORREF CDirectDrawer::crDDrawBackgroundColor = RGB(10, 0, 10);
