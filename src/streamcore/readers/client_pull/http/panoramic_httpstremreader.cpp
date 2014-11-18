#include "stdafx.h"

#include "panoramic_httpstremreader.h"


int PanoramicHTTPStreamreader::getSensorNumber()
{
	unsigned int pos;

	// find ETag:Channel=
	pos = contain_subst(m_data, 50, (char*)"Channel=", sizeof((char*)"Channel="));
	if (pos != -1)
	{
		return (int)atoi(&m_data[pos+8])-1;
	}
	else
	return 0;
}

/**/