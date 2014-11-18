#if !defined(AFX_SBUTTON_H__68A9D681_35A3_11D5_9650_E1BABEF7180B__INCLUDED_)
#define AFX_SBUTTON_H__68A9D681_35A3_11D5_9650_E1BABEF7180B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SButton.h : header file
//

typedef BOOL (FAR WINAPI *Type_TrackMouseEvent)(LPTRACKMOUSEEVENT);

#define ON_WM_MOUSELEAVE() \
	{ WM_MOUSELEAVE, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)())&OnMouseLeave },

/////////////////////////////////////////////////////////////////////////////
// SButton window

class SButton : public CButton
{
	struct SBITMAPINFO {
		BITMAPINFOHEADER    bmiHeader;
		DWORD				bmiColors[256];			
		
		operator BITMAPINFO() { return *this; }
		operator LPBITMAPINFO() { return (LPBITMAPINFO)this; }
		operator LPBITMAPINFOHEADER() { return (LPBITMAPINFOHEADER)this; }	
	};
	
	HBITMAP BWBitmap( HBITMAP hColorBM, BITMAPINFOHEADER &BMInfo );

public:
	SButton();
	virtual ~SButton();

// Attributes
	bool IsPushed(); // �������� ������ �� �������
	void SetPushed( bool bPushed=true ); // ������� ������� �������/�������
	void SetImagePosition( int nImPos ); // SBTN_LEFT or SBTN_TOP
	
	void SetStyleFlat( bool bFlat=true );
	void SetStyleHotText( bool bHot=true );
	void SetStyleToggle( bool bToggle=true ); // ������� � 2 �����������( ������/������ )

	bool SetImages( DWORD dwResourceID, DWORD dwResourceID_BW=NULL, bool bHotImage=true, UINT uType=IMAGE_ICON/*or IMAGE_BITMAP*/ );
	void SetFrameWidth(int nWidth);
	void ShowFocus(bool bShow=true);
		
	inline DWORD BWColor( DWORD b, DWORD g, DWORD r );
		
	enum { NO_FRAME, THIN_FRAME, THICK_FRAME, IMGPOS_LEFT, IMGPOS_TOP };
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(SButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	//}}AFX_MSG
	afx_msg void OnMouseLeave();

	DECLARE_MESSAGE_MAP()

private:

	bool	m_bPushed;      // ��� Toggle button �������� ������ �� �������
	int     m_nWidthFrame;	// ������� 3D-�����
// Styles
	bool	m_bFlat;
	bool	m_bToggle;
	bool	m_bShowFocus;
	bool	m_bHotText;
// Image data
	HANDLE	m_hImage;		// ������� �����������
	HANDLE	m_hImageBW;		// �� �����������
	UINT	m_uTypeImage;   // ��� �����������(Icon or Bitmap)
	int		m_nImPos;		// ������� �����������: ��� ������� ��� �����
	int		m_nImHeight;
	int		m_nImWidht;

	bool	m_bMouseInside; // ����� ������ �������

	static Type_TrackMouseEvent TrackMouseEvent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SBUTTON_H__68A9D681_35A3_11D5_9650_E1BABEF7180B__INCLUDED_)
