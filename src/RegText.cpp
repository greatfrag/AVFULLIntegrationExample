#include "StdAfx.h"
#include ".\regtext.h"
#include <iostream>
#include <sstream>


using namespace std;

static unsigned char charTohex(char val)
{
	switch(val)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'a':
	case 'A':
		return 10;
	case 'b':
	case 'B':
		return 11;
	case 'c':
	case 'C':
		return 12;
	case 'd':
	case 'D':
		return 13;
	case 'e':
	case 'E':
		return 14;
	case 'f':
	case 'F':
		return 15;

	}

	return 0;
}

static char byteToChar(unsigned char val)
{
	switch(val)
	{
	case 0:
		return '0'; 
	case 1:
		return '1'; 
	case 2:
		return '2'; 
	case 3:
		return '3'; 
	case 4:
		return '4'; 
	case 5:
		return '5'; 
	case 6:
		return '6'; 
	case 7:
		return '7'; 

	case 8:
		return '8'; 
	case 9:
		return '9'; 
	case 10:
		return 'a'; 
	case 11:
		return 'b'; 

	case 12:
		return 'c'; 
	case 13:
		return 'd'; 
	case 14:
		return 'e'; 
	case 15:
		return 'f'; 
	default:
		return 0;


	}

	return 0;
}

string intToHex(unsigned int v)
{
	char result[100];
	sprintf(result, "%X", v);
	return result;
}

unsigned int hexToInt(const string hex)
{
	if (hex.length()<1)
		return 0;

	unsigned int x;    
	std::stringstream ss; 
	ss << std::hex << hex; 
	ss >> x;

	return x;
}


string nextWord(string& in)
{
	int pos = in.find_first_not_of(" \t");

	if (pos == string::npos)
		return "";

	in = in.substr(pos);

    pos = in.find_first_of(" \t");

	string result;

	if (pos==string::npos)
	{
		result = in;
		in = "";
		return result;
	}

	result = in.substr(0,pos);

	in = in.substr(pos);

	return result;

}

string nextLine(string& in)
{
	int pos = in.find("\r\n");

	string result;

	if (pos == string::npos)
	{
		result = in;
		in = "";
		return result;
	}

	result = in.substr(0, pos);

	in = in.substr(pos+2);

	return result;

}

//=================================================================
std::string RegLine::toString() const
{
	string result;
	ostringstream os;

	switch(command)
	{
	case CT_comment:
		result = "#";
		result += comment;
		return result;

	case CT_write:
		os << "W " << intToHex(page) << " " << intToHex(reg) << " " << intToHex(val);
		return os.str();
		

	case CT_read:
		os << "R " << intToHex(page) << " " << intToHex(reg) << " " << intToHex(val);
		return os.str();

	case CT_writeRead:
		os << "WR " << intToHex(page) << " " << intToHex(reg) << " " << intToHex(val);
		return os.str();


	}
}

void RegLine::fromString(const std::string& line)
{
	string line_low = line;

	for(int i=0; i<line_low.size();++i)
		line_low[i] = tolower(line_low[i]);

	
	string cmd = nextWord(line_low);

	if (cmd[0]=='#')
	{
		command = CT_comment;
		comment = cmd.substr(1) +  line_low;
		return;
	}
	else if( cmd == "r")
	{
		command = CT_read;
	}
	else if( cmd == "w")
	{
		command = CT_write;
	}
	else if( cmd == "wr")
	{
		command = CT_writeRead;
	}

	else
	{
		command = CT_comment;
		comment = "";
		return;
	}

    page = hexToInt(nextWord(line_low));
	reg = hexToInt(nextWord(line_low));

	if (command == CT_write)
		val = hexToInt(nextWord(line_low));

	
	else if (command == CT_writeRead)
		val = hexToInt(nextWord(line_low));

}

//=================================================================


RegText::RegText()
{
}

RegText::~RegText()
{
}


void RegText::setText(const std::string& str)
{
	m_lines.clear();
	
	string text = str;

	while (text.length())
	{
		string line = nextLine(text);
		RegLine rt;
		rt.fromString(line);
		m_lines.push_back(rt);

	}

	return;
}

std::string RegText::getText() const
{
	string result;
	for (int i = 0; i < m_lines.size(); ++i)
	{
		result += m_lines[i].toString();
		result+="\r\n";
	}

	return result;
}

int RegText::size() const
{
	return m_lines.size();
}

RegLine& RegText::getLineAt(int i)
{
	return m_lines[i];
}

//====================================================================
