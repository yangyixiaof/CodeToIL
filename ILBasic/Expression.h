#pragma once
#include "Token.h"


class Expression
{
public:

    Expression();
    Expression(bool isExpression, Token::Type type, std::string tokenStr, const Expression* parent);
    ~Expression();

	bool isExpression;
	Token::Type type;
	std::string expressionStr;
	std::string tokenStr;
	std::vector<Expression*> childExpressions;
	Expression* parentExpression;
	void replace(const Expression* srcExpression);
	void updateExpressionStr();
	void release();

    static std::vector<std::string> operatorRightStrList;
    static std::vector<std::string> operatorMemberStrList;
    static std::vector<std::string> operatorLeftStrList;
    static std::vector<std::string> operatorAddressStrList;
    static std::vector<std::string> operatorBinocularStrList;
    static std::vector<std::string> operatorAssignStrList;

	Expression* clone() const;

    bool isContainAssign() const;
};