#include "StmtElseIf.h"

StmtElseIf::StmtElseIf()
{
    this->type = Statement::ElseIf;
}

StmtElseIf::StmtElseIf(const Statement* parent)
{
    this->type = Statement::ElseIf;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtElseIf::~StmtElseIf()
{
}

