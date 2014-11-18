// VideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "H264SDKExample.h"
#include "VideoDlg.h"
#include <sstream>
#include "common/Console.h"


// VideoDlg dialog

IMPLEMENT_DYNAMIC(VideoDlg, CDialog)
VideoDlg::VideoDlg(const std::string& basic_str, CWnd* pParent /*=NULL*/)
	: CDialog(VideoDlg::IDD, pParent),
	m_bsic_string(basic_str)
{
}

VideoDlg::~VideoDlg()
{
}

void VideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BOOL VideoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//dDraw.InitDirectDraw( m_hWnd, false );
	dDraw.InitDirectDraw( m_hWnd, false );
	bkgndBrush.CreateSolidBrush( dDraw.GetBkGroundColor() );

	return TRUE;

}

BEGIN_MESSAGE_MAP(VideoDlg, CDialog)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_ON_STAT, OnUpdateStatistic)

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// VideoDlg message handlers

void VideoDlg::OnMove(int x, int y)
{
	dDraw.DDrawOnMove( x, y );
}

void VideoDlg::OnSize(UINT nType, int cx, int cy)
{
	dDraw.DDrawOnSize( nType, cx, cy );
}

BOOL VideoDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	this->GetClientRect( &rect );
	pDC->FillRect( &rect, &bkgndBrush );
	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}

void VideoDlg::OnDraw(CDC* pDC)
{
	dDraw.DDrawOnPaint();
}

void VideoDlg::DrawFrame(Frame* frame)
{


	PRESENTATION_ITEM pi;
	pi.Y = frame->Y;
	pi.U = frame->U;
	pi.V = frame->V;

	pi.i_data_len0 = frame->width;
	pi.i_data_len1 = frame->height;
	pi.i_data_len2 = frame->strideY;
	pi.i_other_data0 = (frame->format == yuv_420) ? 1 : 2;

	setWidth(frame->width);
	setHeight(frame->height);

	dDraw.BlitBitmap( &pi );

}

void VideoDlg::onStatistic(const std::string& str)
{
	std::string * msg = new std::string(str);
	this->PostMessage(WM_ON_STAT, (WPARAM)msg);
}

LRESULT VideoDlg::OnUpdateStatistic(WPARAM wParam, LPARAM lParam)
{
	std::string * msg = (std::string*)(wParam);
	std::ostringstream os;

	os << "(" << getWidth() << "x" << getHeight() << ") " << *msg;
	SetWindowText(os.str().c_str());
	delete msg;
	return 0;
}

void VideoDlg::OnOK()
{

}
void VideoDlg::OnCancel()
{

}

// VideoDlg message handlers
