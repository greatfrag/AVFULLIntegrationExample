//---------------------------------------------------------------------------

#pragma hdrstop
//---------------------------------------------------------------------------
//#include <stdafx.h>
#include "AVLoadDll.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
EDll::EDll(const char* in_msg, int in_code)
   : code(in_code)
{
   int n = strlen(in_msg);
   if(n > 255)
      n = 255;
   strncpy(msg, in_msg, n);
   msg[n] = '\0';
}
//---------------------------------------------------------------------------
const char* EDll::what() const throw()
{
   return msg;
}
//---------------------------------------------------------------------------
int EDll::get_code() const
{
   return code;
}
//---------------------------------------------------------------------------

AV2000F::AV2000F()
   : hClientDll(0)
{
	pCreateClient = 0;
	pDestroyClient = 0;
   pMaxClients = 0;
	pGetLastClientError = 0;
	pSetClientIp = 0;
	pGetClientIp = 0;
   pSetClientPort = 0;
	pGetClientPort = 0;
   pSetClientTimeout = 0;
   pGetClientTimeout = 0;
   pSetRegister = 0;
   pGetRegister = 0;
   pSetSingleCapture = 0;
   pGetSingleCapture = 0;
   pSetCalibrateFlash = NULL;
   pGetCalibrationNumber = NULL;
   pSetAuxIO = NULL;
   pGetAuxIO = NULL;

	pGetImage = 0;
   pGetImage2 = 0;
   pGetWindowImage = 0;
   pGetWindowImageQ = NULL;
   pGetDefaultImage = 0;
	pSetAV2000Parameter = 0;
	pGetAV2000Parameter = 0;
   pFactoryDefault = 0;
   pPermanently = 0;
   pUpdateVersion = 0;
   pModel = 0;
   pVersion = 0;
   pRevision = 0;
   pMini = 0;
   pSetCustomMode = 0;
   pGetCustomMode = 0;

	pSetClientBuffer = 0;
	pSetAllocateFunction = 0;
	pSetDeallocateFunction = 0;
   pReinitedFunction = 0;

   pFindCameras = 0;
   pGetCameras = 0;
   pSetCameraIp = 0;
   pGetLastSetError = 0;

   pDefineReceivePacket = 0;
   pDefineSendAck = 0;
   pDefineInfoSetRegister = 0;
   pDefineInfoGetRegister = 0;

   pGetIrisStatus = 0;
   pCheckCamera = 0;
   pIsBlackWhite = 0;
   pGetMotionArray = 0;
   pGetLastPacket = 0;
   pGetSockets = 0;
   pSetChannelNumber = 0;
}
//---------------------------------------------------------------------------
AV2000F::~AV2000F()
{
   /*
   */
   //if(hClientDll)      ::FreeLibrary(hClientDll);
}
//---------------------------------------------------------------------------


void AV2000F::Initialization()
{
	hClientDll = ::LoadLibrary("AV2000SDK.dll");

	

	if(!hClientDll)
		throw EDll("\"AV2000SDK.dll\" library not found", -1);

	// load functions from dll
	pCreateClient = reinterpret_cast<PTR_CreateClient>(GetProcAddress(hClientDll, "_CreateClient"));
	if(!pCreateClient)
      throw EDll("function \"CreateClient\" not found");

	pDestroyClient = reinterpret_cast<PTR_DestroyClient>(GetProcAddress(hClientDll, "_DestroyClient"));
	if(!pDestroyClient)
      throw EDll("function \"DestroyClient\" not found");

	pMaxClients = reinterpret_cast<PTR_MaxClients>(GetProcAddress(hClientDll, "_MaxClients"));
	if(!pMaxClients)
      throw EDll("function \"_MaxClients\" not found");

	pGetLastClientError = reinterpret_cast<PTR_GetLastClientError>(GetProcAddress(hClientDll, "_GetLastClientError"));
	if(pGetLastClientError == NULL)
      throw EDll("function \"GetLastClientError\" not found");

	pSetClientIp = reinterpret_cast<PTR_SetClientIp>(GetProcAddress(hClientDll, "_SetClientIp"));
	if(pSetClientIp == NULL)
      throw EDll("function \"SetClientIp\" not found");

	pGetClientIp = reinterpret_cast<PTR_GetClientIp>(GetProcAddress(hClientDll, "_GetClientIp"));
	if(pGetClientIp == NULL)
      throw EDll("function \"GetClientIp\" not found");

	pSetClientPort = reinterpret_cast<PTR_SetClientPort>(GetProcAddress(hClientDll, "_SetClientPort"));
	if(!pSetClientPort)
      throw EDll("function \"SetClientPort\" not found");

	pGetClientPort = reinterpret_cast<PTR_GetClientPort>(GetProcAddress(hClientDll, "_GetClientPort"));
	if(pGetClientPort == NULL)
      throw EDll("function \"GetClientPort\" not found");

	pSetClientTimeout = reinterpret_cast<PTR_SetClientTimeout>(GetProcAddress(hClientDll, "_SetClientTimeout"));
	if(pSetClientTimeout == NULL)
      throw EDll("function \"SetClientTimeout\" not found");

	pGetClientTimeout = reinterpret_cast<PTR_GetClientTimeout>(GetProcAddress(hClientDll, "_GetClientTimeout"));
	if(pGetClientTimeout == NULL)
      throw EDll("function \"GetClientTimeout\" not found");

	pSetRegister = reinterpret_cast<PTR_SetAV2000Register>(GetProcAddress(hClientDll, "_SetAV2000Register"));
	if(!pSetRegister)
      throw EDll("function \"_SetAV2000Register\" not found");

	pGetRegister = reinterpret_cast<PTR_GetAV2000Register>(GetProcAddress(hClientDll, "_GetAV2000Register"));
	if(!pGetRegister)
      throw EDll("function \"pGetAV2000Register\" not found");

	pGetSingleCapture = reinterpret_cast<PTR_GetSingleCapture>(GetProcAddress(hClientDll, "_GetSingleCapture"));
	if(pGetSingleCapture == NULL)
      throw EDll("function \"GetSingleCapture\" not found");

	
	pSetSingleCapture = reinterpret_cast<PTR_SetSingleCapture>(GetProcAddress(hClientDll, "_SetSingleCapture"));
	if(pSetSingleCapture == NULL)
      throw EDll("function \"SetSingleCapture\" not found");
	  

	pSetCalibrateFlash = reinterpret_cast<PTR_SetCalibrateFlash>(GetProcAddress(hClientDll, "_SetCalibrateFlash"));
	if(pSetCalibrateFlash == NULL)
      throw EDll("function \"SetCalibrateFlash\" not found");
	  

	pGetCalibrationNumber = reinterpret_cast<PTR_GetCalibrationNumber>(GetProcAddress(hClientDll, "_GetCalibrationNumber"));
	if(pGetCalibrationNumber == NULL)
      throw EDll("function \"GetCalibrationNumber\" not found");
	  

      	pSetAuxIO = reinterpret_cast<PTR_SetAuxIO>(GetProcAddress(hClientDll, "_SetAuxIO"));
	if(pSetAuxIO == NULL)
      throw EDll("function \"SetAuxIO\" not found");

      	pGetAuxIO = reinterpret_cast<PTR_GetAuxIO>(GetProcAddress(hClientDll, "_GetAuxIO"));
	if(pGetAuxIO == NULL)
      throw EDll("function \"GetAuxIO\" not found");
	  

	pGetImage = reinterpret_cast<PTR_GetImage>(GetProcAddress(hClientDll, "_GetImage"));
	if(pGetImage == NULL)
      throw EDll("function \"GetImage\" not found");

	if( !(pGetImage2 = reinterpret_cast<PTR_GetImage2>(GetProcAddress(hClientDll, "_GetImage2"))) )
      throw EDll("function \"GetImage2\" not found");

	pSetAV2000Parameter = reinterpret_cast<PTR_SetAV2000Parameter>(GetProcAddress(hClientDll, "_SetAV2000Parameter"));
	if(pSetAV2000Parameter == NULL)
      throw EDll("function \"SetAV2000Parameter\" not found");


	pGetAV2000Parameter = reinterpret_cast<PTR_GetAV2000Parameter>(GetProcAddress(hClientDll, "_GetAV2000Parameter"));
	if(pGetAV2000Parameter == NULL)
      throw EDll("function \"GetAV2000Parameter\" not found");

	pFactoryDefault = reinterpret_cast<PTR_FactoryDefault>(GetProcAddress(hClientDll, "_FactoryDefault"));
	if(pFactoryDefault == NULL)
      throw EDll("function \"FactoryDefault\" not found");

	pPermanently = reinterpret_cast<PTR_Permanently>(GetProcAddress(hClientDll, "_Permanently"));
	if(!pPermanently)
      throw EDll("function \"Permanently\" not found");

	pUpdateVersion = reinterpret_cast<PTR_UpdateVersion>(GetProcAddress(hClientDll, "_UpdateVersion"));
	if(pUpdateVersion == NULL)
      throw EDll("function \"UpdateVersion\" not found");

	pModel = reinterpret_cast<PTR_Model>(GetProcAddress(hClientDll, "_Model"));
	if(pModel == NULL)
      throw EDll("function \"Model\" not found");

	pVersion = reinterpret_cast<PTR_Version>(GetProcAddress(hClientDll, "_Version"));
	if(pVersion == NULL)
      throw EDll("function \"Version\" not found");

   if( !(pSetCustomMode = reinterpret_cast<PTR_SetCustomMode>(GetProcAddress(hClientDll, "_SetCustomMode"))) )
      throw EDll("function \"SetCustomMode\" not found");

   if( !(pGetCustomMode = reinterpret_cast<PTR_GetCustomMode>(GetProcAddress(hClientDll, "_GetCustomMode"))) )
      throw EDll("function \"GetCustomMode\" not found");

	pSetClientBuffer = reinterpret_cast<PTR_SetClientBuffer>(GetProcAddress(hClientDll, "_SetClientBuffer"));
	if(!pSetClientBuffer)
      throw EDll("function \"SetClientBuffer\" not found");

	pSetAllocateFunction = reinterpret_cast<PTR_SetAllocateFunction>(GetProcAddress(hClientDll, "_SetAllocateFunction"));
	if(!pSetAllocateFunction)
      throw EDll("function \"SetAllocateFunction\" not found");

	pSetDeallocateFunction = reinterpret_cast<PTR_SetDeallocateFunction>(GetProcAddress(hClientDll, "_SetDeallocateFunction"));
	if(!pSetDeallocateFunction)
      throw EDll("function \"SetDeallocateFunction\" not found");

	pReinitedFunction = reinterpret_cast<PTR_SetReinitedFunction>(GetProcAddress(hClientDll, "_SetReinitedFunction"));
	if(!pReinitedFunction)
      throw EDll("function \"SetReinitedFunction\" not found");

	pFindCameras = reinterpret_cast<PTR_FindCameras>(GetProcAddress(hClientDll, "_FindCameras"));
	if(!pFindCameras)
      throw EDll("function \"FindCameras\" not found");

	pGetCameras = reinterpret_cast<PTR_GetCameras>(GetProcAddress(hClientDll, "_GetCameras"));
	if(!pGetCameras)
      throw EDll("function \"GetCameras\" not found");

	pSetCameraIp = reinterpret_cast<PTR_SetCameraIp>(GetProcAddress(hClientDll, "_SetCameraIp"));
	if(!pSetCameraIp)
      throw EDll("function \"SetCameraIp\" not found");

	pGetLastSetError = reinterpret_cast<PTR_GetLastSetError>(GetProcAddress(hClientDll, "_GetLastSetError"));
	if(!pGetLastSetError)
      throw EDll("function \"GetLastSetError\" not found");

   pDefineReceivePacket = reinterpret_cast<PTR_DefineReceivePacket>(GetProcAddress(hClientDll, "_DefineOnReceivePacket"));
	//if(!pDefineReceivePacket)
      //throw EDll("function \"DefineOnReceivePacket\" not found");

   pDefineSendAck = reinterpret_cast<PTR_DefineSendAck>(GetProcAddress(hClientDll, "_DefineOnSendAck"));
	//if(!pDefineSendAck)
      //throw EDll("function \"DefineOnSendAck\" not found");

   pDefineInfoSetRegister = reinterpret_cast<PTR_DefineInfoSetRegister>(GetProcAddress(hClientDll, "_DefineInfoSetRegister"));
	//if(!pDefineInfoSetRegister)
      //throw EDll("function \"DefineInfoSetRegister\" not found");

   pDefineInfoGetRegister = reinterpret_cast<PTR_DefineInfoGetRegister>(GetProcAddress(hClientDll, "_DefineInfoGetRegister"));
	//if(!pDefineInfoGetRegister)
      //throw EDll("function \"DefineInfoGetRegister\" not found");

	if( !(pIsBlackWhite = reinterpret_cast<PTR_IsBlackWhite>(GetProcAddress(hClientDll, "_IsBlackWhite"))) )
      throw EDll("function \"IsBlackWhite\" not found");

	if( !(pGetWindowImage = reinterpret_cast<PTR_GetWindowImage>(GetProcAddress(hClientDll, "_GetWindowImage"))) )
      throw EDll("function \"GetWindowImage\" not found");

	if( !(pGetWindowImageQ = reinterpret_cast<PTR_GetWindowImageQ>(GetProcAddress(hClientDll, "_GetWindowImageQ"))) )
      throw EDll("function \"GetWindowImageQ\" not found");

	if( !(pGetMac = reinterpret_cast<PTR_GetMac>(GetProcAddress(hClientDll, "_GetMac"))) )
      throw EDll("function \"GetMac\" not found");

	if( !(pGetDefaultImage = reinterpret_cast<PTR_GetDefaultImage>(GetProcAddress(hClientDll, "_GetDefaultImage"))) )
      throw EDll("function \"GetDefaultImage\" not found");

	if( !(pRevision = reinterpret_cast<PTR_Revision>(GetProcAddress(hClientDll, "_Revision"))) )
      throw EDll("function \"Revision\" not found");

	if( !(pMini = reinterpret_cast<PTR_Mini>(GetProcAddress(hClientDll, "_Mini"))) )
      throw EDll("function \"Mini\" not found");

	if( !(pGetMotionArray = reinterpret_cast<PTR_GetMotionArray>(GetProcAddress(hClientDll, "_GetMotionArray"))) )
      throw EDll("function \"GetMotionArray\" not found");

	if( !(pGetLastPacket = reinterpret_cast<PTR_GetLastPacket>(GetProcAddress(hClientDll, "_GetLastPacket"))) )
      throw EDll("function \"GetLastPacket\" not found");

	if( !(pGetIrisStatus = reinterpret_cast<PTR_GetIrisStatus>(GetProcAddress(hClientDll, "_GetIrisStatus"))) )
      throw EDll("function \"GetIrisStatus\" not found");

	if( !(pCheckCamera = reinterpret_cast<PTR_CheckCamera>(GetProcAddress(hClientDll, "_CheckCamera"))) )
      throw EDll("function \"CheckCamera\" not found");

	if( !(pGetSockets = reinterpret_cast<PTR_GetSockets>(GetProcAddress(hClientDll, "_GetSockets"))) )
      throw EDll("function \"GetSockets\" not found");


	
	if( !(pGetImageEx = reinterpret_cast<PTR_GetImageEx>(GetProcAddress(hClientDll, "_GetImageEx"))) )
      throw EDll("function \"GetImageEx\" not found");

	if( !(pGetImage2Ex = reinterpret_cast<PTR_GetImage2Ex>(GetProcAddress(hClientDll, "_GetImage2Ex"))) )
      throw EDll("function \"GetImage2Ex\" not found");

	if( !(pGetWindowImageEx = reinterpret_cast<PTR_GetWindowImageEx>(GetProcAddress(hClientDll, "_GetWindowImageEx"))) )
      throw EDll("function \"pGetWindowImageEx\" not found");

	if( !(pGetDefaultImageEx = reinterpret_cast<PTR_GetDefaultImageEx>(GetProcAddress(hClientDll, "_GetDefaultImageEx"))) )
      throw EDll("function \"GetDefaultImageEx\" not found");

	if( !(pGetWindowImageQEx = reinterpret_cast<PTR_GetWindowImageQEx>(GetProcAddress(hClientDll, "_GetWindowImageQEx"))) )
      throw EDll("function \"GetWindowImageQEx\" not found");

	if( !(pSetChannelNumber= reinterpret_cast<PTR_SetChannelNumber>(GetProcAddress(hClientDll, "_SetChannelNumber"))) )
		throw EDll("function \"SetChannelNumber\" not found");

	  /**/

}
//---------------------------------------------------------------------------
