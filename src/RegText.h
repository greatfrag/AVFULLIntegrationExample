#pragma once

#include <string>
#include <vector>

struct RegLine
{
	enum CommandType {CT_comment, CT_read, CT_write, CT_writeRead};
	std::string comment;
	int page;
	int reg;
    int val;
	CommandType  command;

	std::string toString() const;
	void fromString(const std::string& line);
};

class RegText
{
public:
	

	RegText();
	~RegText();

	void setText(const std::string& str);
	std::string getText() const;
	int size() const;
	RegLine& getLineAt(int i) ;
	

	
private:

	std::string m_text;
	std::vector< RegLine> m_lines;
	
private:

	

};
