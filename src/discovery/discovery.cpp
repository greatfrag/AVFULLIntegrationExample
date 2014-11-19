#include "discovery.h"
#include "cam_attributes_geter.h"
#include "cam_searcher.h"

#include <iphlpapi.h>

#include <list>
#include <algorithm>
#include <sstream>
#include "../network/socket.h"
#include "../network/simple_tftp_client.h"
#include "../network/ipaddress.h"

using namespace std;


bool getreg(SimpleTFTPClient& client, int page, int reg, unsigned int &val);
bool check_video(SimpleTFTPClient& client, int model);


void ClearArp(const string& ip)
{
	MIB_IPNETTABLE* mtb = 0;
	unsigned long ulSize = 0;
	GetIpNetTable(0, &ulSize, false);


	mtb = new MIB_IPNETTABLE[ ulSize/sizeof(MIB_IPNETTABLE) + 1];

	if(NO_ERROR != GetIpNetTable(mtb, &ulSize, false))
		return;


	GetIpNetTable(mtb, &ulSize, TRUE);


	in_addr addr;
	int i = 0;
	for(i = 0; i < mtb->dwNumEntries; i++)
	{
		addr.S_un.S_addr = mtb->table[i].dwAddr;

		string wip;

		wip = (inet_ntoa(addr));
		if(wip == ip && NO_ERROR == DeleteIpNetEntry(&mtb->table[i]))
		{
			int old_size = mtb->dwNumEntries;

			GetIpNetTable(mtb, &ulSize, FALSE);

			if (mtb->dwNumEntries == old_size)
			{
				break;
			}

			i = 0;

		}
	}

	delete[] mtb;
}



//============================================================================================================


CameraList findAllCameras()
{
	const int number_os_serchers = 80;
	int i;

	CamSearcher camsercher[number_os_serchers];

    // XXX: start multiple threads to find cameras
	for (i = 0; i < number_os_serchers; ++i)
		camsercher[i].start();

	while(1)
	{
		Sleep(5); // to avoid 100% one core cpu usage
		bool need_break = true;

        // XXX: check if all threads are done
		for (i = 0; i < number_os_serchers; ++i)
			if (camsercher[i].isRuning())
			{
				need_break = false;
				break;
			}

			if (need_break) break;
	}



	//now all streams stoped; need to free thread handles 
	for (i = 0; i < number_os_serchers; ++i)
		camsercher[i].stop();


	CameraList camListMerged;


	for (i = 0; i < number_os_serchers; ++i)
	{
		CameraList tempList = camsercher[i].getCamList();
		CameraList::iterator it = tempList.begin();
		for (;it!=tempList.end();++it)
		{
			CameraInfo cam = it->second;
			camListMerged[cam.mac] = cam;
		}


	}

	return camListMerged;

}

void updateCamInfo(CameraPointerList& cams)
{

	const int max_threads = 50;
	const int cams_amount = cams.size();
	const int threads_amount = min(max_threads, cams_amount);

	int cams_left = cams_amount;
	int threads_left;

	CamAttrGetter** thread = new CamAttrGetter*[threads_amount];
	CameraPointerList* plist = new CameraPointerList[threads_amount];
	
	CameraPointerList::iterator it = cams.begin();

	for (threads_left = threads_amount; threads_left > 0; threads_left--)
	{
		int cams_per_this_thread = cams_left/threads_left;
		

		for(int i = 0; i < cams_per_this_thread; i++)
		{
			string mac = (it->second)->mac;
			plist[threads_left-1][mac] = (it->second);
			++it;
		}

		thread[threads_left-1] = new CamAttrGetter(&(plist[threads_left-1]));
		cams_left -= cams_per_this_thread;
		
	}


	int i;
	for (i = 0; i < threads_amount; ++i)
		thread[i]->start();


	while(1)
	{
		Sleep(5); // to avoid 100% one core cpu usage
		bool need_break = true;

		for (i = 0; i < threads_amount; ++i)
			if (thread[i]->isRuning())
			{
				need_break = false;
				break;
			}

			if (need_break) break;
	}


	for (i = 0; i < threads_amount; ++i)
	{
		thread[i]->stop();
		delete thread[i];
	}

	delete[] thread;
	delete[] plist;
}

void clearCamAlerts(CameraPointerList& cams)
{
	CameraPointerList::iterator it;
	for (it = cams.begin(); it!=cams.end();++it)
		it->second->alert.clear();
}

void checkConflicts(CameraPointerList& cams)
{
	typedef std::list<CameraInfo*> ShortList;
	ShortList::iterator short_iter;

	typedef std::map<std::string, ShortList > ip_list;
	ip_list ipl;
	ip_list::iterator ip_it;

	CameraPointerList::iterator cams_it;
	

	// count each ip
	for (cams_it = cams.begin(); cams_it!=cams.end();++cams_it)
	{
		CameraInfo& info = *(cams_it->second);
		ShortList* sl = &ipl[info.ip];  // find the item list in the table using ip
		sl->push_back(&info);   // add to the end of the item list

	}

	
	for (ip_it = ipl.begin(); ip_it != ipl.end(); ++ip_it)
	{
		ShortList* sl = &(ip_it->second);
		if (sl->size() > 1) // if we have more than one such ip
		{
			for (cams_it = cams.begin(); cams_it!=cams.end();++cams_it)
			{
				CameraInfo& info = *(cams_it->second);
				if (info.ip == ip_it->first) // this camera has IP collision
				{
					short_iter = sl->begin();

					string mac1 = (*short_iter)->mac;
					++short_iter;

					string mac2 = (*short_iter)->mac;

					ostringstream os;
					os << "IP address collision with ";

					if (info.mac == mac1)
						os << mac2;
					else
						os << mac1;


					info.alert.push_back(os.str());
					info.bIPConflict = true;
				}
			}

		}
	}
}


bool isUsedIP(const std::string& ip)
{
	ClearArp(ip);
	HRESULT hr;
	IPAddr  ipAddr;
	ULONG   pulMac[2];
	ULONG   ulLen;

	ipAddr = inet_addr (ip.c_str());
	memset (pulMac, 0xff, sizeof (pulMac));
	ulLen = 6;

	hr = SendARP (ipAddr, 0, pulMac, &ulLen);
	

	if (ulLen==0)
		return false;

	return true;

	/*
	size_t i, j;
	char szMac[100];
	PBYTE pbHexMac = (PBYTE) pulMac;

	//
	// Convert the binary MAC address into human-readable
	//
	for (i = 0, j = 0; i < ulLen - 1; ++i) {
		j += sprintf (szMac + j, "%02X:", pbHexMac[i]);
	}

	sprintf (szMac + j, "%02X", pbHexMac[i]);

	/**/
}

void changeCamIP(CameraInfo& cam, const std::string& new_ip)
{
	UDPSocket socket;
	string basic_str = "Arecont_Vision-AV2000";
	char c = 2;
	basic_str+=c;

	char data[1024];


	int shift = basic_str.length();
	memcpy(data,basic_str.c_str(),shift);
	memcpy(data + shift, cam.macc, 6);

	IpAddress new_ip_addr(new_ip);
	memcpy(data + shift + 6, new_ip_addr.getAddress(),4);

	socket.sendTo(data,shift+10, "255.255.255.255", 69);
	socket.sendTo(data,shift+10, "255.255.255.255", 69);

	ClearArp(new_ip);
	ClearArp(cam.ip);

	cam.ip = new_ip;
}

bool lockCamIP(CameraInfo& cam, bool lock)
{
	if (cam.iplocked == lock) return true; // already in this state

	SimpleTFTPClient tftp(cam.ip, 50,3);
	unsigned int val;
	
	if (!getreg(tftp,3,0x80,val)) return false;
	if (lock)
		val = val | 0x08;
	else
		val = val & 0x87;

	if (!tftp.write_reg(127,3)) return false;
	if (!tftp.write_reg(112,123)) return false;
	if (!tftp.write_reg(0x80,val)) return false;

	cam.iplocked = lock;

	return true;

}

bool setCamDescription(CameraInfo& cam, const std::string& descr)
{
	if (cam.description==descr) return true;
	SimpleTFTPClient tftp(cam.ip, 50,3);
	unsigned int val;

	if (!tftp.write_reg(127,3)) return false;

	if (!tftp.write_reg(143,0)) return false;
	int j=0;
	for(unsigned int i=0; i<min((descr.length()+1)/2, 32); i++)
	{
		unsigned int sum = ((unsigned)descr[2*i] << 8) & ~0x0FF;
		if(2*i+1 <= descr.length())
			sum |= descr[2*i+1] & 0x0FF;
		sum &= 0x0FFFF;

		if (!tftp.write_reg((143+i),sum)) return false;
		j = min(31, i + 1);
	}

	if (!tftp.write_reg((143+j),0)) return false;
	/**/



	cam.description = descr;
	return true;
	
}

bool setCamSaveToFlash(CameraInfo& cam)
{
	SimpleTFTPClient tftp(cam.ip, 50,3);

	if (!tftp.write_reg(127,3)) return false;

	if (!tftp.write_reg(112,123)) return false;
	if (!tftp.write_reg(113,0)) return false;


}

bool setCamFactoryDefault(CameraInfo& cam)
{
	SimpleTFTPClient tftp(cam.ip, 50,3);

	if (!tftp.write_reg(127,3)) return false;

	if (!tftp.write_reg(0x70,123)) return false;
	tftp.write_reg(0x7E,0x80);

	cam.description = "";
	cam.iplocked = false;

	return true;

}

bool checkCamVideo(CameraInfo& cam)
{
	SimpleTFTPClient tftp(cam.ip, 50,3);
	return check_video(tftp,cam.model);
}

CameraPointerList convertCamList(CameraList& in)
{
	CameraPointerList out;
	CameraList::iterator it;

	for (it = in.begin(); it != in.end(); ++it)
	{
		CameraInfo& info = it->second;
		out[info.mac] = &info;
	}

    return out;	
}

void combineCamList(CameraPointerList& dst, CameraPointerList& addition)
{
	CameraPointerList::iterator it;
	for (it = addition.begin(); it != addition.end(); ++it)
	{
		CameraInfo* info = it->second;
		dst[info->mac] = info;
	}

}