#pragma once
#include <string>


class Expression;

class ILExpressionParser
{
public:
	//CodeParser类的主要功能，即：将c代码转化为中间语言XML格式
	static int parseExpression(const std::string& expressionStr, Expression** ret);

    static Expression* parseExpression(std::string expressionStr);

};
