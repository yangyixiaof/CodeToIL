#pragma once
#include <string>
#include <vector>


class ILObject;
class Expression;

class Statement
{
public:


	enum Type{
	    If, Else, ElseIf, While, 
		DoWhile, For, Continue, Switch, Case, 
		Default, Break, Return, BraceStatement, 
		LocalVariable, ExpressionWithSemicolon,
        BatchCalculation, Assemble,
        Unknown,
	};

    
    Statement();
    Statement(Type type, const Statement* parent);
    Statement(Type type, int depth, const Statement* parent);
    explicit Statement(Type type);
    explicit Statement(const Statement* parent);
    virtual ~Statement();

	static std::vector<std::string> statementTypeName;
	Type type = Unknown;
	std::vector<Statement*> childStatements;
	std::vector<Expression*> innerExpressions;
	int depth = -1;
	Statement* parentStatement = nullptr;

	Statement* getNextStatement() const;

	Statement* getPreviousStatement() const;

    //获得派生类Statement的表达式，比如if的条件表达式
    Expression* getValueExpression() const;
    
    std::vector<Statement*>* getStatementList() const;

	void print() const;
	virtual void release();

    ILObject* parentILObject = nullptr;

    std::vector<void*> getDomain() const;
    
};
