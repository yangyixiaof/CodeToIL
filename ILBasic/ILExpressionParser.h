#pragma once
#include <string>


class Expression;

class ILExpressionParser
{
public:
	//CodeParser�����Ҫ���ܣ�������c����ת��Ϊ�м�����XML��ʽ
	static int parseExpression(const std::string& expressionStr, Expression** ret);

    static Expression* parseExpression(std::string expressionStr);

};
