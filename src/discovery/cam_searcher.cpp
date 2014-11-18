#include "cam_searcher.h"
#include "../network/ipaddress.h"
#include "../network/socket.h"
#pragma comment(lib, "Iphlpapi.lib")


using namespace std;


void MACToString (
			 char*    pszBuffer,
			 unsigned char*    lpbMAC
			 )
{
	pszBuffer[0] = ('\0');

	for (DWORD i = 0; i < 6; i++)
		sprintf (pszBuffer + strlen (pszBuffer), ("%02X-"), lpbMAC[i]);

	if (strlen (pszBuffer) > 0)
		pszBuffer[strlen (pszBuffer) - 1] = ('\0');

} 


void CamSearcher::run()
{
	
	m_camList.clear();



	UDPSocket socket;

	//socket.setLocalAddressAndPort("192.168.1.124");

	socket.setTimeOut(300);

	string basic_str = "Arecont_Vision-AV2000";
	char c = 1;
	basic_str+=c;

	int basic_len = basic_str.length();

	try
	{
		socket.sendTo(basic_str.c_str(),basic_len, "255.255.255.255", 69); // send broadcast to find cams
	}
	catch(...)
	{
		return ; // return empty list
	}

	unsigned long t0 = timeGetTime();
	

	// looking for replays 
	while(1)
	{
			char data[1024];
			

			string camip = "";
			unsigned short camport = 69;

			int bytesRcvd = 0;
			try
			{
				bytesRcvd = socket.recvFrom(data, sizeof(data), camip , camport);
			}
			catch(SocketException &e)
			{
				
			}

			if (bytesRcvd < basic_len + 10)
			{
				if (timeGetTime() - t0 > m_max_delay) break;// if timeout; 
				continue; // whis is not av cam
			}

			if (memcmp(data, "Arecont_Vision-AV2000", basic_len-1 )!=0)
				continue; // this responde id not from arecont camera
			


			unsigned char mac[6];
			memcpy(mac,data + basic_len,6);


			CameraInfo camInfo;
			camInfo.ip = camip;
			
			char mac_c[50];

			MACToString(mac_c,mac);
			camInfo.mac  = mac_c;
			memcpy(camInfo.macc,mac,6);

			int shift = basic_len + 10;

			if (bytesRcvd > shift + 5)
			{
				camInfo.firmware = (unsigned char)data[shift] * 256 + (unsigned char)data[shift+1]; //2
				camInfo.model = (unsigned char)data[shift+2] * 256 + (unsigned char)data[shift+3]; //4
				camInfo.iplocked = (data[shift+4]==1);//5

				int data_left = bytesRcvd - shift - 5;
				if (data_left>0)
				{
					shift+=5;
					char descr_c[256];
					memset(descr_c,0,sizeof(descr_c));
					memcpy(descr_c,data+shift, data_left);
					camInfo.description = descr_c;
					camInfo.description_updated = true;
				}
			}
			m_camList[camInfo.mac] = camInfo;
			
	}

}


//==========================================


