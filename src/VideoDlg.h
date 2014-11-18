#pragma once
#include "viewsupport/DirectDrawer.h"
#include "./frameinfo/frame.h"
#include "./viewsupport/FrameDrawer.h"
#include "./common/WinCriticalSection.h"
#include <string>


#define WM_ON_STAT (WM_USER + 1)
// VideoDlg dialog

class VideoDlg : public CDialog , public Drawer
{
	DECLARE_DYNAMIC(VideoDlg)

public:
	VideoDlg(const std::string& basic_str, CWnd* pParent = NULL);   // standard constructor
	void DrawFrame(Frame* frame);
	virtual void onStatistic(const std::string& str);
	
	virtual ~VideoDlg();

// Dialog Data
	enum { IDD = VIDEO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnUpdateStatistic(WPARAM wParam, LPARAM lParam);

	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view


private:
	CDirectDrawer dDraw;
	CBrush bkgndBrush;
	std::string m_bsic_string;

	//=================
	int m_width;
	int m_height;
	mutable WinCriticalSection cs;

	int getWidth() const{Mutex mutex(cs); return m_width;};
	void setWidth(int w){Mutex mutex(cs); m_width=w;}

	int getHeight() const{Mutex mutex(cs); return m_height;};
	void setHeight(int h){Mutex mutex(cs); m_height=h;}

	//=================


};
