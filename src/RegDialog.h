#pragma once
#include "afxwin.h"
#include "RegText.h"


// CRegDialog dialog
class CH264SDKExampleDlg;

class CRegDialog : public CDialog
{
	DECLARE_DYNAMIC(CRegDialog)

public:
	CRegDialog(CH264SDKExampleDlg* maindlg, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegDialog();

// Dialog Data
	enum { IDD = IDD_REG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void OnOK();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRead();
	CEdit m_regEdit;

private:
	RegText m_regTex;
	CH264SDKExampleDlg* m_maindlg;
public:
	CListBox m_lbHistory;
	afx_msg void OnBnClickedButton1();
};
