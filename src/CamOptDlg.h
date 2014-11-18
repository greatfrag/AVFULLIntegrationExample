#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <string>
#include "./discovery/discovery.h"

// CamOptDlg dialog

class CamOptDlg : public CDialog
{
	DECLARE_DYNAMIC(CamOptDlg)
public:
	CamOptDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CamOptDlg();

// Dialog Data
	enum { IDD = CAM_PROPS_DIALOG };


protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CIPAddressCtrl m_ipaddr;
	CEdit m_descrEdit;
	CButton m_lockIPcheck;
	CButton m_fdButton;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedFdButton();

public:
	CameraInfo* cam;

};
