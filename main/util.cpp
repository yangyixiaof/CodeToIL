#include "util.h"
#include <string>
#include <vector>


#include "../Common/Utility.h"

int process(std::string& typeStr, int curPos)
{
	while (curPos < typeStr.length())
	{
		char c = typeStr[curPos];
		if (c == '\n')
		{
			typeStr.replace(curPos, 1, 1,' ');
			curPos++;
		}
		else if (c == '}')
		{
			return curPos+1;
		}
		else if (c == '{')
		{
			curPos = process(typeStr, curPos+1);
		}
		else {
			curPos++;
		}
	}
	return curPos;
}

std::string processStruct(std::string str)
{
	std::string typeStr = str;
	int index = 0;
	while (index < typeStr.length())
	{
		char c = typeStr[index];
		if (c == '{')
		{
			index=process(typeStr, index+1);
		}
		index++;
	}
	return typeStr;
}

