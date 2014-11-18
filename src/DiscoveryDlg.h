#pragma once
#include "afxcmn.h"
#include <string>
#include "./discovery/discovery.h"
class CameraInfo;


// DiscoveryDlg dialog

class DiscoveryDlg : public CDialog
{
	DECLARE_DYNAMIC(DiscoveryDlg)

public:
	DiscoveryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DiscoveryDlg();
	void Clear();

// Dialog Data
	enum { IDD = IDD_FIND_FIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	

	DECLARE_MESSAGE_MAP()

private:
	void addCamera(int index, const CameraInfo& info, std::string warning);

	CameraList camList;
public:
	CListCtrl m_CamList;
	afx_msg void OnBnClickedFindBtn();
	afx_msg void OnBnClickedModifybtn();

	
	afx_msg void OnNMDblclkCamlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
};
