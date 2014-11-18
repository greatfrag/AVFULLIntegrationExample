// CamOptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "H264SDKExample.h"
#include "CamOptDlg.h"
#include ".\camoptdlg.h"
#include "./network/ipaddress.h"
#include "./sdk_dll/AV2000Types.h"
#include <sstream>

using namespace std;

// CamOptDlg dialog

IMPLEMENT_DYNAMIC(CamOptDlg, CDialog)
CamOptDlg::CamOptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CamOptDlg::IDD, pParent)
{
}

CamOptDlg::~CamOptDlg()
{
}

void CamOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_ipaddr);
	DDX_Control(pDX, IDC_DESCRIPTION_EDIT, m_descrEdit);
	DDX_Control(pDX, IDC_LOCK_CHECK, m_lockIPcheck);
	DDX_Control(pDX, IDC_FD_BUTTON, m_fdButton);
}


BEGIN_MESSAGE_MAP(CamOptDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_FD_BUTTON, OnBnClickedFdButton)
END_MESSAGE_MAP()


// CamOptDlg message handlers

BOOL CamOptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (cam->bIPConflict)
	{
		m_descrEdit.EnableWindow(FALSE);
		m_fdButton.EnableWindow(FALSE);
		m_lockIPcheck.EnableWindow(FALSE);
	}

	if (cam->model == AV8180 || cam->model == AV8185 || cam->model == AV8360 || cam->model == AV8365)
	{
		m_descrEdit.EnableWindow(FALSE);
		m_lockIPcheck.EnableWindow(FALSE);
	}

	IpAddress ipaddr(cam->ip);
	const unsigned char * ipc = ipaddr.getAddress();
	m_ipaddr.SetAddress( ipc[0], ipc[1],ipc[2],ipc[3]);

	m_descrEdit.SetWindowText(cam->description.c_str());
	m_lockIPcheck.SetCheck(cam->iplocked ? BST_CHECKED : BST_UNCHECKED);

	return TRUE;

}

void CamOptDlg::OnBnClickedOk()
{

	bool sf_needed = false;


	unsigned char b[4];
	std::ostringstream os;
	m_ipaddr.GetAddress(b[0],b[1],b[2],b[3]);
	os << (int)b[0] << "." << (int)b[1] << "." << (int)b[2] << "." << (int)b[3] ;
	string new_ip = os.str();

	
	bool locked = (m_lockIPcheck.GetCheck() == BST_CHECKED);
	if (new_ip!=cam->ip && (cam->iplocked && locked))
	{
		MessageBox("Please, unlock IP first.","");
	}
	else if (new_ip!=cam->ip)	
	{
		if (isUsedIP(new_ip))
		{
			int res = MessageBox("This IP is already in use. Are you sure?","",MB_YESNO);
			if (res!=IDYES) return;
		}
	}





	
	if (cam->iplocked != locked)
	{
		if (!lockCamIP(*cam,locked))
		{
			OnOK();
			return;
		}

		sf_needed = true;
	}

	char text[256];
	m_descrEdit.GetWindowText(text, sizeof(text));
	string texts(text);
	if (texts!=cam->description)
	{
		if (!setCamDescription(*cam,texts))
		{
			OnOK();
			return;
		}
		sf_needed = true;
	}

	if (sf_needed)
	{
		Sleep(50U);
		setCamSaveToFlash(*cam);
	}


	if (new_ip!=cam->ip && !cam->iplocked)
	{
		changeCamIP(*cam,new_ip);
		Sleep(200);
		setCamSaveToFlash(*cam);
	}

	OnOK();
}

void CamOptDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CamOptDlg::OnBnClickedFdButton()
{
	int res = MessageBox("Are you sure?","",MB_YESNO);
	if (res!=IDYES) return;

	setCamFactoryDefault(*cam);

	OnOK();
}
