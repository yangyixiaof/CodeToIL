#include "StmtCase.h"

StmtCase::StmtCase()
{
    this->type = Statement::Case;
}

StmtCase::StmtCase(const Statement* parent)
{
    this->type = Statement::Case;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtCase::~StmtCase()
{
}

