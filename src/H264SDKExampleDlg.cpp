// H264SDKExampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "H264SDKExample.h"
#include "H264SDKExampleDlg.h"
#include ".\h264sdkexampledlg.h"
#include "./common/Console.h"
#include "./viewsupport/colorspace.h"
#include "./cameramodel/cameramodel.h"
#include "./streamcore/readers/client_pull/tftp/ss_tftpstremreader.h"
#include "./streamcore/readers/client_pull/http/httpstremreader.h"
#include "./streamcore/readers/client_pull/http/panoramic_httpstremreader.h"
#include "./streamcore/readers/client_pull/tftp/panoramic_tftpstremreader.h"
#include "./streamcore/readers/client_pull/tftp/panoramic_h264_tftpstremreader.h"
#include "./streamcore/readers/server_push/http/ss_push_httpstremreader.h"
#include "./streamcore/readers/server_push/http/panoramic_push_httpstremreader.h"
#include "./network/socket.h"
#include "./network/ipaddress.h"
#include "network/simple_http_client.h"
#include "regdialog.h"
#include <iostream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


bool isImplimented(int model, ConnectionType ct)
{
	switch(model)
	{
	case AV1300:
	case AV1305:
	case AV2100:
	case AV2105:
	case AV2805:
	case AV3100:
	case AV3105:
	case AV5100:
	case AV5105:
	case AV3130:
	case AV3135:
	case AV10005:
	{
			switch (ct)
			{
			case CT_TFTP:
			case CT_HTTP10Pull:
			case CT_HTTPPush:
				return true;
			default:
				return false;
			}
		}
		break;



	case AV8180:
	case AV8185:
	case AV8360:
	case AV8365:

	case AV12186:
	case AV12366:
	case AV20185:
	case AV20365:

		{
			switch (ct)
			{
			case CT_TFTP:
			case CT_HTTP10Pull:
			case CT_HTTPPush:
				return true;
			default:
				return false;
			}
		}
		break;


	default:
		return false;
	}
}

std::string ConnectionTypeToString(ConnectionType conn )
{
	switch(conn)
	{
	case CT_TFTP:
		return "TFTP:";
	case CT_HTTP10Pull:
		return"HTTP(1.0) client pull:";
	case CT_HTTP11Pull:
	    return"HTTP(1.0) client pull:";
	case CT_HTTPPush:
		return "HTTP server push:";
	case CT_RTSP:
		return "RTSP:";
	default:
	    return "";
	}
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog 
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CH264SDKExampleDlg dialog



CH264SDKExampleDlg::CH264SDKExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CH264SDKExampleDlg::IDD, pParent),
	m_regDlg(this)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDB_GREEN);

}

void CH264SDKExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_H264, m_h264);
	DDX_Control(pDX, IDC_MJPEG, m_mjpeg);
	DDX_Control(pDX, IDC_Q_SLIDER, m_Qslider);
	DDX_Control(pDX, IDOK, m_videoStartButton);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Control(pDX, IDC_TFT_NUM, m_ClientNumEdit);
	DDX_Control(pDX, IDC_RES_COMBO, m_resolutionEdit);
	DDX_Control(pDX, IDC_COMBO1, m_RateControl);
	DDX_Control(pDX, IDC_RES_COMBO2, m_resFullHalfedit);
	DDX_Control(pDX, IDC_CONN_TYPE_COMBO, m_ConnType);
	DDX_Control(pDX, IDC_FPS_COMBO, m_FPSEdit);
	DDX_Control(pDX, IDC_DN_AUTO, m_dnAuto);
	DDX_Control(pDX, IDC_DN_DAY, m_dnDay);
	DDX_Control(pDX, IDC_DN_NIGHT, m_dnNight);
	DDX_Control(pDX, IDC_LIGHT_50, m_light50);
	DDX_Control(pDX, IDC_LIGHT_60, m_light60);
	DDX_Control(pDX, IDC_CAPTURE_EN_CHK, m_capture_en_chk);
	DDX_Control(pDX, IDC_CAPTURE_BTN, m_capture_btn);
	DDX_Control(pDX, IDC_CAPTURE_NUM_EDIT, m_capture_num_edit);
	DDX_Control(pDX, IDC_CHECK_DAY_BINNING, m_dayBinningCheck);
	DDX_Control(pDX, IDC_CHECK_NIGHT_BINNING, m_nightBinningCheck);
	DDX_Control(pDX, IDC_CHECK_1080P_MODE, m_1080pModeCheck);
	DDX_Control(pDX, IDC_CAPTURE_EN_CHK2, m_doublescan_chk);
	DDX_Control(pDX, IDC_CAPTURE_EN_CHK3, m_scaledimage_chk);
	DDX_Control(pDX, IDC_CAPTURE_EN_CHK4, m_flexiblecropping_chk);
	DDX_Text(pDX, IDC_Q_TEXT, m_Qtext);
}

BEGIN_MESSAGE_MAP(CH264SDKExampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_H264, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_MJPEG, OnBnClickedRadio3)
//	ON_WM_MOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOVE()
	ON_WM_MOVE()
//	ON_WM_CLOSE()
	ON_CBN_SELENDOK(IDC_RES_COMBO, OnCbnSelendokResCombo)
	ON_CBN_SELENDOK(IDC_COMBO1, OnCbnSelendokCombo1)
	ON_CBN_SELENDOK(IDC_RES_COMBO2, OnCbnSelendokResCombo2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_Q_SLIDER, OnNMReleasedcaptureQSlider)
	ON_CBN_SELENDOK(IDC_FPS_COMBO, OnCbnSelendokFpsCombo)
	ON_BN_CLICKED(IDC_DN_AUTO, OnBnClickedDnAuto)
	ON_BN_CLICKED(IDC_DN_DAY, OnBnClickedDnDay)
	ON_BN_CLICKED(IDC_DN_NIGHT, OnBnClickedDnNight)
	ON_BN_CLICKED(IDC_LIGHT_50, OnBnClickedLight50)
	ON_BN_CLICKED(IDC_LIGHT_60, OnBnClickedLight60)
	ON_BN_CLICKED(IDC_FIND_BTN, OnBnClickedFindBtn)
	ON_MESSAGE(WM_ON_NEW_IP , OnNewIP)
	
	ON_BN_CLICKED(IDC_CAPTURE_EN_CHK, OnBnClickedCaptureEnChk)
	ON_BN_CLICKED(IDC_CAPTURE_BTN, OnBnClickedCaptureBtn)
	ON_BN_CLICKED(IDC_CHECK_DAY_BINNING, OnBnClickedCheckDayBinning)
	ON_BN_CLICKED(IDC_CHECK_NIGHT_BINNING, OnBnClickedCheckNightBinning)
	ON_BN_CLICKED(IDC_CHECK_1080P_MODE, OnBnClickedCheck1080pMode)
	ON_BN_CLICKED(IDC_REGISTER_BUTTON, OnBnClickedRegisterButton)
	ON_BN_CLICKED(IDC_CAPTURE_EN_CHK2, &CH264SDKExampleDlg::OnBnClickedCaptureEnChk2)
	ON_BN_CLICKED(IDC_CAPTURE_EN_CHK3, &CH264SDKExampleDlg::OnBnClickedCaptureEnChk3)
	ON_BN_CLICKED(IDC_CAPTURE_EN_CHK4, &CH264SDKExampleDlg::OnBnClickedCaptureEnChk4)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CH264SDKExampleDlg message handlers


void Allocate(char** pbuffer, unsigned long* psize)
{
	*psize = *psize * 2;
	*pbuffer = new char[*psize];
}
//----------------------------------------------------------------
void Deallocate(char* buffer)
{
	delete[] buffer;
}
//----------------------------------------------------------------


BOOL CH264SDKExampleDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	CConsole::getInstance().setLogFile("log.txt");
	CConsole::getInstance().Print("====starting=====");

	
	if (!ColorSpaceDll::instance().isinited())
		MessageBox("colorspace.dll not loaded");


	//================================================

	m_ip.SetAddress(10,10,25,14);

	m_videoStartButton.SetStyleFlat(false);
	//m_videoStartButton.SetImagePosition(SButton::IMGPOS_LEFT);
	m_videoStartButton.SetStyleToggle(true);
	//m_videoStartButton.SetImages(IDB_GREEN, NULL, false, IMAGE_BITMAP );


	sdk_model = 2000;
	CameraModel::init();
	m_resolutionEdit.AddString("  640x480");
	m_resolutionEdit.AddString("1280x1024");
	m_resolutionEdit.AddString("1600x1200");
	m_resolutionEdit.AddString("1920x1080");
	m_resolutionEdit.AddString("2048x1536");
	m_resolutionEdit.AddString("2596x1944");
	m_resolutionEdit.AddString("3648x2752");
	m_resolutionEdit.SelectString(0, "1600x1200");

	m_resFullHalfedit.AddString("Full");
	m_resFullHalfedit.AddString("Half");
	m_resFullHalfedit.SelectString(0, "Full");

	m_ConnType.AddString("TFTP(client pull)+");
	m_ConnType.AddString("HTTP 1.0(client pull)+");
	m_ConnType.AddString("HTTP 1.1(client pull)");
	m_ConnType.AddString("HTTP (server push)+");
	m_ConnType.AddString("RTSP (server push)");
	m_ConnType.SelectString(0, "TFTP(client pull)");


	m_FPSEdit.AddString(" 1");
	m_FPSEdit.AddString(" 5");
	m_FPSEdit.AddString("10");
	m_FPSEdit.AddString("15");
	m_FPSEdit.AddString("MAX");
	m_FPSEdit.SelectString(0, "MAX");


	m_RateControl.AddString("1 Mbs/s");
	m_RateControl.AddString("2 Mbs/s");
	m_RateControl.AddString("3 Mbs/s");
	m_RateControl.AddString("4 Mbs/s");
	m_RateControl.AddString("Off");
	m_RateControl.SelectString(0, "Off");



	m_ClientNumEdit.SetWindowText("1");


	m_capture_btn.EnableWindow(FALSE);
	m_capture_num_edit.EnableWindow(FALSE);
	m_capture_num_edit.SetWindowText("1");


	//================================================ 
	AV2000F::instance().Initialization();

	AV2000F::instance().pSetAllocateFunction(Allocate);
	AV2000F::instance().pSetDeallocateFunction(Deallocate);
	


	m_dnAuto.SetCheck(TRUE);
	m_h264.SetCheck(BST_CHECKED);
	m_light60.SetCheck(TRUE);
	int startQ = 10;
	m_Qslider.SetRangeMin(0);
	m_Qslider.SetRangeMax(21);
	m_Qslider.SetPos(10);
	m_codec_type = H264_CODEC;
	m_Qtext.Format(_T("%d"), 37-startQ);
	m_dnMode = AV_DN_Auto;
	m_lightMode = AV_HZ_60;

	srand((unsigned)time(0)); 

	//m_discoveryDlg = new DiscoveryDlg();
	m_discoveryDlg.Create(IDD_FIND_FIALOG, this);
	m_regDlg.Create(CRegDialog::IDD, this);

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CH264SDKExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CH264SDKExampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CH264SDKExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

std::string CH264SDKExampleDlg::getCamIp(CIPAddressCtrl & ip) 
{
	unsigned char b[4];
	std::ostringstream os;
	ip.GetAddress(b[0],b[1],b[2],b[3]);
	os << (int)b[0] << "." << (int)b[1] << "." << (int)b[2] << "." << (int)b[3] ;

	return os.str();
}



int srandom()
{
	unsigned int random_integer = rand(); 
	return random_integer%1000 ;
}


bool CH264SDKExampleDlg::getCamInfo()
{
	int client_num = 1;
	unsigned char  buff[2];
	unsigned long val;  

	
	AV2000F::instance().CreateClient(client_num);
	AV2000F::instance().pSetClientIp(client_num, getCamIp(m_ip).c_str());

	int ret = AV2000F::instance().pUpdateVersion(client_num);
	if (ret!=1) 
	{
		AV2000F::instance().DestroyClient(client_num);
		return false;
	}

	int new_sdk_model = AV2000F::instance().pModel(client_num);
	if (new_sdk_model!=sdk_model)
		setResolution(new_sdk_model);

	sdk_model = new_sdk_model;
	sdk_version = AV2000F::instance().pVersion(client_num);

	AV2000F::instance().pGetRegister(client_num, 0x42, buff);
	val = buff[0]*256 + buff[1];
	bool dn_switch_avalable = (AV3130 == sdk_model) || (AV3135 == sdk_model) || ((val == 222) & 0x01);
	

	AV2000F::instance().DestroyClient(client_num);


	std::ostringstream os;
	os << "CameraModel=" << sdk_model << "  FirmWare=" << sdk_version;
	SetWindowText(os.str().c_str());


	if (!dn_switch_avalable)
	{
		m_dnAuto.EnableWindow(FALSE);
		m_dnDay.EnableWindow(FALSE);
		m_dnNight.EnableWindow(FALSE);
	}

	if (!CameraModel::isH264support(sdk_model))
	{
		m_mjpeg.SetCheck(TRUE);
		m_h264.SetCheck(FALSE);
		m_h264.EnableWindow(FALSE);
		OnBnClickedRadio3();
	}

	number_of_sensors  = CameraModel::getNumberOfSensors(sdk_model);
	connectionType = getConnectionType();

	if (!isImplimented(sdk_model, connectionType))
	{
		MessageBox("This connection type for this model in not implemented in this example yet.\n Please try to get a new one.\n Thanks!");
		m_videoStartButton.SetPushed(FALSE);
		AV2000F::instance().DestroyClient(client_num);
		return false;
	}

	return true;
}


void CH264SDKExampleDlg::OnBnClickedOk()
{
	if (m_videoStartButton.IsPushed())
	{
		if (!getCamInfo())
			return;



		char temp[256];
		m_ClientNumEdit.GetWindowText(temp, sizeof(temp));
		int clients  = atoi(temp);


		if (clients > 8)
		{
			MessageBox("Sorry. This kind of camera supports only 8 streams.");
			return;
		}


		for (int i = 0; i < clients ; ++i) // maximum 8 clients
		{

			Streamreader *reader = 0;

			switch(connectionType)
			{
			case CT_TFTP:

				if (number_of_sensors>1)// panoramic camera
					reader = new PanoramicTFTPStreamreader(sdk_model, getCamIp(m_ip), i+1);
				else
					reader = new SSTFTPStreamreader(sdk_model,getCamIp(m_ip), i+1);

				break;


			case CT_HTTP10Pull:
				if (number_of_sensors>1)// panoramic camera
					reader = new PanoramicHTTPStreamreader(sdk_model, getCamIp(m_ip));
				else
				reader = new SSHTTPStreamreader(sdk_model, getCamIp(m_ip).c_str());
				break;

			case CT_HTTPPush:
				if (number_of_sensors>1)// panoramic camera
					reader = new PanoramicHTTPPushStreamreader(sdk_model, getCamIp(m_ip).c_str());
				else
				reader = new SSHTTPPushStreamreader(sdk_model, getCamIp(m_ip).c_str());
				break;

			}


			StreamParams params;

			params.codecType = m_codec_type;
			params.dn_mode = m_dnMode;
			params.light = m_lightMode;
			params.quality = m_Qslider.GetPos();
			params.resolution = getResolution();
			params.stream_id = srandom()+1;
			params.top = params.left = 0;
			params.width = getResolutionWidth();
			params.height = getResolutionHeight();
			params.bitrate = getBitrate();
			params.fps = getFps();
			params.capture_enable = 0;
			params.capture_trigger = 0;
			params.fmversion = sdk_version;
			params.doublescan = m_doublescan_chk.GetCheck()?1:0;
			params.scaledimage = m_scaledimage_chk.GetCheck();
			if (clients > 1) params.client = i+1;
			else params.client = 0;

			reader->setStreamParams(params);

			int max_sensor = number_of_sensors;

			int sensor;
			for (sensor = 0; sensor  < max_sensor;++sensor)
			{

				Display display;
				display.dlg = new VideoDlg(ConnectionTypeToString(connectionType));
				display.dlg->Create(VIDEO_DIALOG);
				int width = 400;
				display.dlg->SetWindowPos(this, sensor*width, width*3/4*i, width,width*3/4,0);

				display.dlg->ShowWindow(SW_NORMAL);

				display.str_view = new StreamViewer(*display.dlg);

				reader->setDataQueue(sensor, display.str_view->getFrameQuue(), display.dlg);

				// start to display 
				display.str_view->start();

				m_displays.push_back(display);
			}

			// start to read frames from camera
			reader->start();
			m_readers.push_back(reader);


		}

		m_ConnType.EnableWindow(FALSE);
		m_videoStartButton.SetWindowText("Stop");

		if(m_ConnType.GetCurSel() == 4)
		{
			m_scaledimage_chk.SetCheck(FALSE);
			m_scaledimage_chk.EnableWindow(FALSE);
		}
	}
	else
	{

		for (readers::iterator rit = m_readers.begin(); rit!=m_readers.end();++rit)
		{
			Streamreader* preader = *rit;
			preader->stop();
			delete preader;
		}

		m_readers.clear();

		for (displays::iterator it = m_displays.begin(); it!=m_displays.end(); ++it)
		{
			Display display = *it;
			display.str_view->stop();
			if (display.dlg->m_hWnd)
				display.dlg->DestroyWindow();

			delete display.str_view;
			delete display.dlg;
		}

		m_displays.clear();


		m_h264.EnableWindow(TRUE);
		m_videoStartButton.SetWindowText("Start");
		m_ConnType.EnableWindow(TRUE);

		m_dnAuto.EnableWindow(TRUE);
		m_dnDay.EnableWindow(TRUE);
		m_dnNight.EnableWindow(TRUE);

		m_scaledimage_chk.EnableWindow(TRUE);
	}
}


void CH264SDKExampleDlg::OnBnClickedRadio2()
{
	m_codec_type = H264_CODEC;

	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.codecType = H264_CODEC;
		reader->setStreamParams(params);

	}

	m_mjpeg.SetCheck(FALSE);
	m_Qtext.Format(_T("%d"), 37-m_Qslider.GetPos());
	UpdateData(FALSE);
}




void CH264SDKExampleDlg::OnBnClickedRadio3()
{
	m_codec_type = JPEG_CODEC;

	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.codecType = JPEG_CODEC;
		reader->setStreamParams(params);
	}

	m_h264.SetCheck(FALSE);
	m_Qtext.Format(_T("%d"), m_Qslider.GetPos());
	UpdateData(FALSE);
}



void CH264SDKExampleDlg::OnCancel()
{
	for (readers::iterator rit = m_readers.begin(); rit!=m_readers.end();++rit)
	{
		Streamreader* preader = *rit;
		preader->stop();
		delete preader;
	}

	m_readers.clear();

	for (displays::iterator it = m_displays.begin(); it!=m_displays.end(); ++it)
	{
		Display display = *it;
		display.str_view->stop();
		if (display.dlg->m_hWnd)
			display.dlg->DestroyWindow();

		delete display.str_view;
		delete display.dlg;
	}

	m_displays.clear();

	m_discoveryDlg.DestroyWindow();

	m_regDlg.DestroyWindow();

	

	//delete m_discoveryDlg;

	CDialog::OnCancel();
}

int CH264SDKExampleDlg::getBitrate()
{
	int currSel = m_RateControl.GetCurSel();
	switch(currSel) 
	{
	case 0:
		return 1024;
	case 1:
		return 2*1024;
	case 2:
		return 3*1024;
	case 3:
		return 4*1024;
	case 4:
		return 0;

	}
	return 0;

}

IMAGE_RESOLUTION CH264SDKExampleDlg::getResolution()
{
	int currSel = m_resFullHalfedit.GetCurSel();
	switch(currSel) 
	{
	case 0:
		return imFULL;
	case 1:
		return imHALF;
	}
    
	return imFULL;

}

ConnectionType CH264SDKExampleDlg::getConnectionType()
{
	int currSel = m_ConnType.GetCurSel();
	switch(currSel) 
	{
	case 0:
		return CT_HTTPPush;
	case 1:
		return CT_HTTP10Pull;
	case 2:
		return CT_HTTP11Pull;
	case 3:
		return CT_RTSP;
	case 4:
		return CT_TFTP;
	default:
		return CT_TFTP;

	}


}

int CH264SDKExampleDlg::getResolutionWidth()
{
	int currSel = m_resolutionEdit.GetCurSel();
	switch(currSel) 
	{
	case 0:
		return 640;
	case 1:
		return 1280;
	case 2:
		return 1600;
	case 3:
		return 1920;
	case 4:
		return 2048;
	case 5:
		return 2596;
	case 6:
		return 3648;

	}
}

int CH264SDKExampleDlg::getResolutionHeight()
{
	int currSel = m_resolutionEdit.GetCurSel();
	switch(currSel) 
	{
	case 0:
		return 480;
	case 1:
		return 1024;
	case 2:
		return 1200;
	case 3:
		return 1080;
	case 4:
		return 1536;
	case 5:
		return 1944;
	case 6:
		return 2752;

	}

}

void CH264SDKExampleDlg::setResolution(int model)
{
	switch(model)
	{
	case AV1300:
	case AV1305:
		m_resolutionEdit.SelectString(0, "1280x1024");
		break;

	case AV2100:
	case AV2105:
	case AV8180:
	case AV8360:
	case AV8185:
	case AV8365:

		m_resolutionEdit.SelectString(0, "1600x1200");
		break;

	case AV2805:
		m_resolutionEdit.SelectString(0, "1920x1080");
		break;


	case AV3100:
	case AV3105:
	case AV12186:
	case AV12366:
		m_resolutionEdit.SelectString(0, "2048x1536");
		break;

	case AV5100:
	case AV5105:
		m_resolutionEdit.SelectString(0, "2596x1944");
		break;


	case AV3130:
		m_resolutionEdit.SelectString(0, "2048x1536");
		break;

	case AV10005:
		m_resolutionEdit.SelectString(0, "3648x2752");
		//m_resolutionEdit.SelectString(0, "1600x1200");
		break;

	default:
		m_resolutionEdit.SelectString(0, "1280x1024");


	}
}

void CH264SDKExampleDlg::chekResolution()
{
	int curr_width = getResolutionWidth();
	if (curr_width > CameraModel::getMaxSensorWidth(sdk_model))
		setResolution(sdk_model);
}





void CH264SDKExampleDlg::OnCbnSelendokResCombo()
{
	chekResolution();	

	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.width = getResolutionWidth();
		params.height= getResolutionHeight();
		reader->setStreamParams(params);

	}

}

void CH264SDKExampleDlg::OnCbnSelendokCombo1()
{
	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.bitrate = getBitrate();
		reader->setStreamParams(params);


	}
	
}

void CH264SDKExampleDlg::OnCbnSelendokResCombo2()
{
	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;
		

		StreamParams params = reader->getStreamParam();
		params.resolution = getResolution();
		reader->setStreamParams(params);

	}
}

void CH264SDKExampleDlg::OnBnClickedButton1()
{
	std::ostringstream os;
	os << "explorer http://" << getCamIp(m_ip);

	WinExec(os.str().c_str(), SW_NORMAL);
	
}

void CH264SDKExampleDlg::OnNMReleasedcaptureQSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.quality= m_Qslider.GetPos();
		reader->setStreamParams(params);

	}
	
	*pResult = 0;
}

int CH264SDKExampleDlg::getFps()
{
	int currSel = m_FPSEdit.GetCurSel();
	switch(currSel) 
	{
	case 0:
		return 1;
	case 1:
		return 5;
	case 2:
		return 10;
	case 3:
		return 15;
	case 4:
		return 0;
	}

}


void CH264SDKExampleDlg::OnCbnSelendokFpsCombo()
{
	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.fps= getFps();
		reader->setStreamParams(params);
	}
}



void CH264SDKExampleDlg::OnBnClickedDnAuto()
{
	m_dnMode = AV_DN_Auto;
	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.dn_mode = m_dnMode;
		reader->setStreamParams(params);
	}

	m_dnDay.SetCheck(FALSE);
	m_dnNight.SetCheck(FALSE);


}

void CH264SDKExampleDlg::OnBnClickedDnDay()
{
	m_dnMode = AV_DN_Day;
	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.dn_mode = m_dnMode;
		reader->setStreamParams(params);
	}

	m_dnAuto.SetCheck(FALSE);
	m_dnNight.SetCheck(FALSE);

}

void CH264SDKExampleDlg::OnBnClickedDnNight()
{
	m_dnMode = AV_DN_Night;
	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.dn_mode = m_dnMode;
		reader->setStreamParams(params);
	}

	m_dnAuto.SetCheck(FALSE);
	m_dnDay.SetCheck(FALSE);

}

void CH264SDKExampleDlg::OnBnClickedLight50()
{
	m_lightMode = AV_HZ_50;
	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.light = m_lightMode;
		reader->setStreamParams(params);
	}

	m_light60.SetCheck(FALSE);
	
}

void CH264SDKExampleDlg::OnBnClickedLight60()
{
	m_lightMode = AV_HZ_60;
	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.light = m_lightMode;
		reader->setStreamParams(params);
	}

	m_light50.SetCheck(FALSE);

}

void CH264SDKExampleDlg::OnBnClickedFindBtn()
{
	m_discoveryDlg.Clear();
	m_discoveryDlg.ShowWindow(SW_NORMAL);
}

LRESULT CH264SDKExampleDlg::OnNewIP(WPARAM wParam, LPARAM lParam)
{
	string* ips = (string*)wParam;
	IpAddress addr(*ips);
	const unsigned char* addrc = addr.getAddress();

	m_ip.SetAddress(addrc[0],addrc[1],addrc[2],addrc[3]);

	delete ips;
	return 0;
}


void CH264SDKExampleDlg::OnBnClickedCaptureEnChk()
{
	bool checked = m_capture_en_chk.GetCheck();
	
	m_capture_btn.EnableWindow(checked);
	m_capture_num_edit.EnableWindow(checked);

	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.capture_enable = checked;
		reader->setStreamParams(params);
	}

}

void CH264SDKExampleDlg::OnBnClickedCaptureBtn()
{
	char temp[256];
	unsigned short trigger_num;

	m_capture_num_edit.GetWindowText(temp, sizeof(temp));
	trigger_num  = atoi(temp);


	if (trigger_num > 15)
	{
		MessageBox("Sorry. The camera supports only up to 15 triggers at a time.");
		return;
	}

	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.capture_trigger = trigger_num;
		reader->setStreamParams(params);
	}
}

std::string CH264SDKExampleDlg::getIP() 
{
	return getCamIp(m_ip);
}

void CH264SDKExampleDlg::OnBnClickedCheckDayBinning()
{
	bool checked = m_dayBinningCheck.GetCheck();


	SimpleHTTPClient http_client(getCamIp(m_ip));
	std::ostringstream os;
	if (checked)
		os << "set?day_binning="<< "on";
	else
		os << "set?day_binning="<< "off";


	http_client.setRequestLine(os.str());
	http_client.openStream();
	//bool opened = http_client.isOpened();

	
}

void CH264SDKExampleDlg::OnBnClickedCheckNightBinning()
{
	bool checked = m_nightBinningCheck.GetCheck();

	SimpleHTTPClient http_client(getCamIp(m_ip));
	std::ostringstream os;
	if (checked)
		os << "set?night_binning="<< "on";
	else
		os << "set?night_binning="<< "off";


	http_client.setRequestLine(os.str());
	http_client.openStream();

//	System::Console::WriteLine("checkpoint\n");

}

void CH264SDKExampleDlg::OnBnClickedCheck1080pMode()
{
	bool checked = m_1080pModeCheck.GetCheck();

	SimpleHTTPClient http_client(getCamIp(m_ip));
	std::ostringstream os;
	if (checked)
		os << "set?1080p_mode="<< "on";
	else
		os << "set?1080p_mode="<< "off";


	http_client.setRequestLine(os.str());
	http_client.openStream();

}

void CH264SDKExampleDlg::OnBnClickedRegisterButton()
{
	m_regDlg.ShowWindow(SW_NORMAL);
}


void CH264SDKExampleDlg::OnBnClickedCaptureEnChk2()
{
	// TODO: Add your control notification handler code here
	bool checked = m_doublescan_chk.GetCheck();

	
	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.doublescan = checked;
		reader->setStreamParams(params);

	}

}


void CH264SDKExampleDlg::OnBnClickedCaptureEnChk3()
{
	// TODO: Add your control notification handler code here
	bool checked = m_scaledimage_chk.GetCheck();

	SimpleHTTPClient http_client(getCamIp(m_ip));
	std::ostringstream os;
	if (checked)
		os << "set?scaling="<< "on";
	else
		os << "set?scaling="<< "off";


	http_client.setRequestLine(os.str());
	http_client.openStream();

	
	for (readers::iterator it = m_readers.begin(); it!=m_readers.end(); ++it)
	{
		Streamreader* reader = *it;

		StreamParams params = reader->getStreamParam();
		params.scaledimage = checked;
		reader->setStreamParams(params);

	}

}


void CH264SDKExampleDlg::OnBnClickedCaptureEnChk4()
{
	// TODO: Add your control notification handler code here
	bool checked = m_flexiblecropping_chk.GetCheck();

	SimpleHTTPClient http_client(getCamIp(m_ip));
	std::ostringstream os;
	if (checked)
		os << "set?cropping="<< "on";
	else
		os << "set?cropping="<< "off";


	http_client.setRequestLine(os.str());
	http_client.openStream();
}


void CH264SDKExampleDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar *) &m_Qslider)
	{
	   int value = m_Qslider.GetPos();
	   if (m_codec_type == H264_CODEC) value = 37 - value;
	   m_Qtext.Format(_T("%d"), value);
	   UpdateData(FALSE);
	}
	else
	{
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}
