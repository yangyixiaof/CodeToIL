#include "ILExpressionParser.h"
#include <iostream>
#include "ILCCodeParser.h"
#include "Expression.h"
using namespace std;

int ILExpressionParser::parseExpression(const string& expressionStr, Expression** ret)
{
	Statement tempStmt;
	ILCCodeParser iLCCodeParser;
	string expressionStrFull = expressionStr+";";
	int res = iLCCodeParser.parseCCode(expressionStrFull, &tempStmt);

	if(res < 0)
		return res;

	*ret = tempStmt.childStatements[0]->innerExpressions[0];
	(*ret)->parentExpression = nullptr;
	(*ret)->updateExpressionStr();

	if(expressionStr != (*ret)->expressionStr)
	{
        //(*ret)->release();
        //*ret = nullptr;

		return 0;
	    //如果expressionStr代码比较紧凑，表达式解析就应该返回和输入代码一致的代码
	    //考虑到输入未格式化的代码，则返回0，否则应该返回-ILCCodeParser::ErrorExpressionParseError;
	}

	return 1;
}

Expression* ILExpressionParser::parseExpression(std::string expressionStr)
{
    Expression* ret = nullptr;
    if(parseExpression(expressionStr, &ret) < 0)
        return nullptr;
    return ret;
}

