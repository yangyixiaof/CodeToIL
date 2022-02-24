#include "StmtDefault.h"

StmtDefault::StmtDefault()
{
    this->type = Statement::Default;
}

StmtDefault::StmtDefault(const Statement* parent)
{
    this->type = Statement::Default;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtDefault::~StmtDefault()
{
}

