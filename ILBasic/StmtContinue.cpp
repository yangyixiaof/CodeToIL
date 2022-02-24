#include "StmtContinue.h"

StmtContinue::StmtContinue()
{
    this->type = Statement::Continue;
}

StmtContinue::StmtContinue(const Statement* parent)
{
    this->type = Statement::Continue;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtContinue::~StmtContinue()
{
}

