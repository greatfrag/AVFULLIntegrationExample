// H264SDKExampleDlg.h : header file
//

#pragma once
#include "streamcore/streamviewer.h"
#include "streamcore/readers/streamreader.h"
#include "VideoDlg.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "sbutton.h"
#include <vector>
#include "DiscoveryDlg.h"
#include "regdialog.h"

class CRegDialog;

#define WM_ON_NEW_IP (WM_USER + 7) 

struct Display
{
	VideoDlg* dlg;
	StreamViewer* str_view;	
};

enum ConnectionType {CT_TFTP = 0, CT_HTTP10Pull = 1, CT_HTTP11Pull = 2, CT_HTTPPush = 3, CT_RTSP = 4};


// CH264SDKExampleDlg dialog
class CH264SDKExampleDlg : public CDialog
{
// Construction
public:
	CH264SDKExampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_H264SDKEXAMPLE_DIALOG };

	std::string getIP() ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNewIP(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	std::string getCamIp(CIPAddressCtrl &) ;
	ConnectionType getConnectionType() ;

	int getResolutionWidth();
	int getResolutionHeight();
	IMAGE_RESOLUTION getResolution();

	void setResolution(int model);
	void chekResolution();
	int getBitrate();
	int getFps();

	bool getCamInfo();
private:
	typedef std::vector<Display> displays;
	typedef std::vector<Streamreader*> readers;
private:

	displays m_displays;
	readers m_readers;

	CButton m_h264;
	CButton m_mjpeg;
	CSliderCtrl m_Qslider;

	bool started;
	int sdk_model;
	int sdk_version;
	ConnectionType connectionType;
	int number_of_sensors;

	AVCodecType m_codec_type;
	DN_modes m_dnMode;
	Light_modes m_lightMode;

	CComboBox m_ConnType;

	DiscoveryDlg m_discoveryDlg;

	CRegDialog m_regDlg;
	

public:

	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedOk();


	void OnCancel();

	SButton m_videoStartButton;
	CIPAddressCtrl m_ip;
	CEdit m_ClientNumEdit;
	CComboBox m_resolutionEdit;
	afx_msg void OnCbnSelendokResCombo();
	CComboBox m_RateControl;
	afx_msg void OnCbnSelendokCombo1();
	CComboBox m_resFullHalfedit;
	afx_msg void OnCbnSelendokResCombo2();
	afx_msg void OnBnClickedButton1();
private:
	
public:
	afx_msg void OnNMReleasedcaptureQSlider(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_FPSEdit;
	afx_msg void OnCbnSelendokFpsCombo();
protected:
	CButton m_dnAuto;
	CButton m_dnDay;
	CButton m_dnNight;
	CButton m_light50;
	CButton m_light60;
public:
	afx_msg void OnBnClickedDnAuto();
	afx_msg void OnBnClickedDnDay();
	afx_msg void OnBnClickedDnNight();
	afx_msg void OnBnClickedLight50();
	afx_msg void OnBnClickedLight60();
	afx_msg void OnBnClickedFindBtn();
private:
	CButton m_capture_en_chk;
	CButton m_doublescan_chk;
	CButton m_scaledimage_chk;
	CButton m_flexiblecropping_chk;
	CButton m_capture_btn;
	CEdit m_capture_num_edit;

	
public:
	afx_msg void OnBnClickedCaptureEnChk();
	afx_msg void OnBnClickedCaptureBtn();
	afx_msg void OnBnClickedCaptureEnChk2();
	CButton m_dayBinningCheck;
private:
	CButton m_nightBinningCheck;
	CButton m_1080pModeCheck;
public:
	afx_msg void OnBnClickedCheckDayBinning();
	afx_msg void OnBnClickedCheckNightBinning();
	afx_msg void OnBnClickedCheck1080pMode();
	afx_msg void OnBnClickedRegisterButton();
	afx_msg void OnBnClickedCaptureEnChk3();
	afx_msg void OnBnClickedCaptureEnChk4();
};
