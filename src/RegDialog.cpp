// RegDialog.cpp : implementation file
//

#include "stdafx.h"
#include "H264SDKExample.h"
#include "RegDialog.h"
#include ".\regdialog.h"
#include "network\simple_http_client.h"
#include <iostream>
#include <sstream>
#include "H264SDKExampleDlg.h"

using namespace std;



// CRegDialog dialog

IMPLEMENT_DYNAMIC(CRegDialog, CDialog)
CRegDialog::CRegDialog(CH264SDKExampleDlg* maindlg, CWnd* pParent /*=NULL*/)
	: CDialog(CRegDialog::IDD, pParent),
	m_maindlg(maindlg)
{
}

CRegDialog::~CRegDialog()
{
}

void CRegDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REGEDIT, m_regEdit);
	DDX_Control(pDX, IDC_LIST1, m_lbHistory);
}


BEGIN_MESSAGE_MAP(CRegDialog, CDialog)
	ON_BN_CLICKED(IDREAD, OnBnClickedRead)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CRegDialog message handlers


void setRegister(string ip, int page, int num, int val)
{
	ostringstream os;
	os << "setreg?page=" << page << "&reg=" << num << "&val=" << val;

	SimpleHTTPClient http(ip);

	http.setRequestLine(os.str());

	http.openStream();
}


int getRegister(string ip, int page, int num)
{
	ostringstream os;
	os << "getreg?page=" << page << "&reg=" << num;

	SimpleHTTPClient http(ip);

	http.setRequestLine(os.str());

	http.openStream();

	char c_response[200];
	int result_size =  http.read(c_response,sizeof(c_response));

	if (result_size <0)
		return 0;


	string resp(c_response, 150);



	int index = resp.find('=');
	if (index==string::npos)
		return 0;

	resp = resp.substr(index+1);

	return atoi(resp.c_str());
	
}



void CRegDialog::OnBnClickedRead()
{
	CString temp;
	m_regEdit.GetWindowText(temp);

	const char* binText = (LPCTSTR)temp;
	m_regTex.setText(binText);

	string ip = m_maindlg->getIP();

	for (int i = 0; i < m_regTex.size(); ++i)
	{
		
        
		RegLine& rl = m_regTex.getLineAt(i);

		ostringstream os;

		switch(rl.command)
		{
		case RegLine::CT_write:
			setRegister(ip, rl.page, rl.reg, rl.val);
			os << "Reg written: page = "   << rl.page << " reg = " << rl.reg << " val = " << rl.val;
			m_lbHistory.AddString(os.str().c_str());
			break;

		case RegLine::CT_read:
			rl.val = getRegister(ip, rl.page, rl.reg);
			os << "Reg read: page = "   << rl.page << " reg = " << rl.reg << " val = " << rl.val;
			m_lbHistory.AddString(os.str().c_str());

		case RegLine::CT_writeRead:
			setRegister(ip, rl.page, rl.reg, rl.val);
			os << "Reg written: page = "   << rl.page << " reg = " << rl.reg << " val = " << rl.val;
			rl.val = getRegister(ip, rl.page, rl.reg);
			os << " Readback val = " << rl.val;
			m_lbHistory.AddString(os.str().c_str()); 
			break; 

		default:
		    break;
		}
	}

	//if ()

	m_regEdit.SetWindowText(m_regTex.getText().c_str());

	
	
}

void CRegDialog::OnOK()
{
	int n = 0;
}
void CRegDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	m_lbHistory.ResetContent();
}
