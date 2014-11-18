//////////////////////////////////////////////////////////////////////
// GeneralTypes.h
//////////////////////////////////////////////////////////////////////
#if !defined(__GENARAL_APPLICATION_TYPES_H__INCLUDED__)
#define __GENARAL_APPLICATION_TYPES_H__INCLUDED__

#include "GeneralDefines.h"
#include <vector>

//=========================================================
typedef unsigned char byte;
typedef unsigned int  uint;
typedef unsigned __int64 uint64;
typedef unsigned long ulong;
typedef std::vector<int> vec_int;

//=========================================================
struct TRANSPORT_BUFFER
{
	bool bFree;
	byte ucBuffer[ TRANSPORT_BUFFER_LENGTH ];
	uint uiLength;
};

//=========================================================
//										|		  VIDEO			|         AUDIO				|

// CHROMA FORMAT : 0 - reserved, 1 - 420, 2 - 422, 3 - 444  
// FRAME TYPE	 : 0 - reserved, 1 - I,   2 - P,   3 - B  
struct PRESENTATION_ITEM
{
	bool			b_free;			//	
	int				i_time_stamp;   //		in milliseconds
	byte* Y;						//  |	Y					|	data ptr				| 
	byte* U;						//	|	U					|	reserved				|
	byte* V;						//	|	V					|	reserved				|
	int				i_data_len0;	//  |	h size				|	buffer length			|
	int				i_data_len1;	//  |	v size				|	reserved				|
	int				i_data_len2;	//  |	h presentation size	|	reserved				|
	int				i_data_len3;	//  |	v presentation size |	reserved				|
	int				i_other_data0;	//  |	chroma format		|	frequency				|
	int				i_other_data1;	//  |	frame type			|	channels number			|
	int				i_other_data2;	//  |	reserved			|	reserved				|
};

//====================================================================
#endif //__GENARAL_APPLICATION_TYPES_H__INCLUDED__
