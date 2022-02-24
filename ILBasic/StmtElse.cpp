#include "StmtElse.h"

StmtElse::StmtElse()
{
    this->type = Statement::Else;
}

StmtElse::StmtElse(const Statement* parent)
{
    this->type = Statement::Else;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtElse::~StmtElse()
{
}

