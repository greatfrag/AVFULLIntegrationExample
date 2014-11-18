#include "./cam_attributes_geter.h"
#include "../network/ipaddress.h"
#include "../network/socket.h"
#include "../network/simple_http_client.h"
#include "../network/simple_tftp_client.h"
#include "../common/WinCriticalSection.h"
#include "../sdk_dll/AVLoadDll.h"
#include <sstream>
#include "discovery.h"
using namespace std;


// this is very simple( must rewrite ) function;
// if cam and PC is in the same network or not
static bool isTheSameNetwork(const string& ip)
{
	IpAddressList localiplist = IpAddress::getLocalIpAddresses(false);

	for (IpAddressList::iterator it = localiplist.begin(); it!=localiplist.end(); ++it)
	{
		IpAddress localAddr = *it;
		IpAddress camAddr(ip.c_str());

        int len = localAddr.getAddressLen();

		const unsigned char* lc = localAddr.getAddress();
		const unsigned char* cc = camAddr.getAddress();
		
		bool the_same = (memcmp(lc,cc,len-1) == 0);

        if (the_same)
			return true;

	}

	return false;
}



bool getreg(SimpleTFTPClient& client, int page, int reg, unsigned int &val)
{
	unsigned char data[100];

	ostringstream os;
	os << "getreg?page=" <<  page << ";reg=" << reg << ";";
	if (client.read(os.str(),(char*)data))
	//if (client.read("h264?res=half;x0=64;y0=160;x1=1984;y1=1376;qp=22;doublescan=1;iframe=0;ssn=1;",(char*)data))
	{
		val = data[0]*256 + data[1];
		return true;
	}
	return false;
}

/*
bool setreg(SimpleTFTPClient& client, int page, int reg, unsigned int val)
{
	unsigned char data[100];

	ostringstream os;
	os << "setreg?page=" <<  page << ";reg=" << reg << ";val=" << val <<";";
	if (client.read(os.str(),(char*)data))
		return true;

	return false;
}
/**/

string get_descrioption(SimpleTFTPClient& client)
{
	char buff[256];
	buff[32] = 0;
	
	for(int i = 0; i < 32; i++)
	{
		unsigned int val;
		if (!getreg(client,3, 143+i, val)) return "";
		buff[2*i] = (val >> 8) & 0x0FF;
		buff[2*i+1] = val & 0x0FF;
		if(buff[2*i] != 0)
		{
			if(buff[2*i+1] == 0)
				break;
		}
		else
			break;
	}

	return buff;

}

bool check_video(SimpleTFTPClient& client, int model)
{
	char* image = new char[1024*1024*2];
	string request = "image?res=half;x0=0;y0=0;x1=1320;y1=1240;doublescan=1;";

	if (model== 8360 && model== 8180)
		request = "image?res=half;quality=15;doublescan=1;";

	if (client.read(request, image))
	{
		delete[] image;
		return true;
	}

	delete[] image;
	return false;
}

void CamAttrGetter::run()
{
	int client_num ;
	unsigned int  val;
	bool readed;

	CameraPointerList::iterator it = m_camlist->begin();

	for (;it!=m_camlist->end();++it)
	{

		CameraInfo* cam = it->second;

		if (cam->bIPConflict) continue; // we should not waste time to access conflict ip;

		if (!isTheSameNetwork(cam->ip))
			continue;

		SimpleTFTPClient tftp(cam->ip, 50,3);

		
		if (cam->model==2000) 
			if (!getreg(tftp,3,122,cam->model)) continue;



		if (cam->firmware==0) 
			if (!getreg(tftp,3,123,cam->firmware)) continue;
			
		
		if (!getreg(tftp,3,0x42,val)) continue;
		cam->switch_present = (AV3130 == cam->model) || (AV3135 == cam->model) || ((val == 222) & 0x01);

		if (!cam->switch_present)
		{
			if (!getreg(tftp,3,0x50,val)) continue;
			cam->iris_present = ((val&2));
		}

		if (!getreg(tftp,3,0x80,val)) continue;
		cam->iplocked = val & 0x08;

		if ((cam->model%10)==5) // h264
		{
			cam->mini = true;
		}
		else
		{
			if (!getreg(tftp,3,0x1A,val)) continue;
			cam->mini = val == 0xAAAA;
		}



		if ( (cam->model%10)==5 && cam->model!=3135)
		{
			// might be megadome 
			if (!getreg(tftp,3,116,val)) continue;
			cam->mega_dome = (val%100) == 55;
		}

		if (!cam->description_updated)// description might be already updated;
		{
			cam->description = get_descrioption(tftp);
			cam->description_updated = true;
		}

		/**/

		//cam->video_checked = check_video(tftp, cam->model);

		
		cam->checked = 1;
		

	}


}
