// DiscoveryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "H264SDKExample.h"
#include "DiscoveryDlg.h"
#include "./network/ipaddress.h"
#include "CamOptDlg.h"




#include <sstream>
 #include <Mmsystem.h>
#include ".\discoverydlg.h"

using namespace std;

// DiscoveryDlg dialog

IMPLEMENT_DYNAMIC(DiscoveryDlg, CDialog)
DiscoveryDlg::DiscoveryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DiscoveryDlg::IDD, pParent)
{
}

DiscoveryDlg::~DiscoveryDlg()
{
}

void DiscoveryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMLIST, m_CamList);
}


BEGIN_MESSAGE_MAP(DiscoveryDlg, CDialog)
	ON_BN_CLICKED(ID_FIND_BTN, OnBnClickedFindBtn)
	ON_BN_CLICKED(IDC_MODIFYBTN, OnBnClickedModifybtn)
	ON_NOTIFY(NM_DBLCLK, IDC_CAMLIST, OnNMDblclkCamlist)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


BOOL DiscoveryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CamList.InsertColumn(0, "Index",LVCFMT_LEFT, 50);
	m_CamList.InsertColumn(1, "Mac",LVCFMT_LEFT, 140);
	m_CamList.InsertColumn(2, "IP",LVCFMT_LEFT, 120);
	m_CamList.InsertColumn(3, "Model/Firmware",LVCFMT_LEFT, 120);
	m_CamList.InsertColumn(4, "Description",LVCFMT_LEFT, 140);
	m_CamList.InsertColumn(5, "Warning",LVCFMT_LEFT, 230);

	m_CamList.SetExtendedStyle( m_CamList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	return TRUE;
}

// DiscoveryDlg message handlers

void DiscoveryDlg::addCamera(int index, const CameraInfo& info, string warning)
{

	string index_s;
	string model_s;
	string ip_s = info.ip; 
	if (info.iplocked) ip_s+="*";

	{
		ostringstream os;
		os << index;
		index_s = os.str();
	}

	{
		ostringstream os;
		os << "AV";

		int add = info.mega_dome ? 50 : 0;

		os << info.model  + add;

		if (info.iris_present) os << "AI";
		if (info.switch_present) os << "DN";

		os << "/" << info.firmware;
		model_s = os.str();
	}



	int nIndex = m_CamList.InsertItem(index,index_s.c_str());
	
	m_CamList.SetItemText(nIndex,1,info.mac.c_str());
	m_CamList.SetItemText(nIndex,2,ip_s.c_str());
	m_CamList.SetItemText(nIndex,3,model_s.c_str());
	m_CamList.SetItemText(nIndex,4,info.description.c_str());

	ostringstream os;
	for (AlertList::const_iterator it = info.alert.begin(); it!= info.alert.end(); ++it)
		os << (*it) << ";";

	m_CamList.SetItemText(nIndex,5,os.str().c_str());

}

void DiscoveryDlg::Clear()
{
	m_CamList.DeleteAllItems ();
}

void DiscoveryDlg::OnBnClickedFindBtn()
{

	//m_CamList.SetItemState(0,LVIS_SELECTED, LVIS_SELECTED);

	unsigned long t0 = timeGetTime();

	// before each search we should update loacal ip info 
	IpAddress::getLocalIpAddresses(true);
	
	// XXX: find all cameras and store into the camList
	camList = findAllCameras();

    // XXX: clear Warnings and check conflicts and raise Warnings
	clearCamAlerts(convertCamList(camList));
	checkConflicts(convertCamList(camList));

	updateCamInfo(convertCamList(camList));

	unsigned long tc = timeGetTime();
	{
		ostringstream os;
		os << "TotalSerch time = " << tc-t0 << " ms.";
		SetWindowText(os.str().c_str());
	}


	Clear();

	CameraList::iterator it = camList.begin();
	int index = 0;
	for (;it!=camList.end();++it)
	{
		CameraInfo cam = it->second;

        // add cameras to the display
		addCamera(index, cam, "");

		++index;
	}


}

void DiscoveryDlg::OnBnClickedModifybtn()
{
	
	POSITION pos = m_CamList.GetFirstSelectedItemPosition();
	if (pos==NULL)
	{
		MessageBox("Nothing is selected");
		return;
	}

	int nItem = m_CamList.GetNextSelectedItem(pos);
	char mac[256];

	m_CamList.GetItemText(nItem,1,mac,sizeof(mac));
	string macs = mac;

	CameraInfo& cam = camList[macs];


	CamOptDlg dlg;

	dlg.cam = &cam;
	dlg.DoModal();

	Clear();

	clearCamAlerts(convertCamList(camList));
	checkConflicts(convertCamList(camList));

	CameraList::iterator it = camList.begin();
	int index = 0;
	for (;it!=camList.end();++it)
	{
		CameraInfo cam = it->second;

		addCamera(index, cam, "");

		++index;
	}

}

void DiscoveryDlg::OnNMDblclkCamlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	OnBnClickedModifybtn();
	*pResult = 0;
}

void DiscoveryDlg::OnBnClickedButton1()
{
	POSITION pos = m_CamList.GetFirstSelectedItemPosition();
	if (pos==NULL)
	{
		MessageBox("Nothing is selected");
		return;
	}

	int nItem = m_CamList.GetNextSelectedItem(pos);
	char mac[256];

	m_CamList.GetItemText(nItem,1,mac,sizeof(mac));
	string macs = mac;

	CameraInfo& cam = camList[macs];

    string* str = new string(cam.ip);

	GetParent()->PostMessage(WM_USER+7, (WPARAM)str);

	OnOK();
}
