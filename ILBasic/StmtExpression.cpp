#include "StmtExpression.h"

StmtExpression::StmtExpression()
{
    this->type = Statement::ExpressionWithSemicolon;
}

StmtExpression::StmtExpression(const Statement* parent)
{
    this->type = Statement::ExpressionWithSemicolon;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtExpression::~StmtExpression()
{
}

