#include "StmtIf.h"

StmtIf::StmtIf()
{
    this->type = Statement::If;
}

StmtIf::StmtIf(const Statement* parent)
{
    this->type = Statement::If;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtIf::~StmtIf()
{
}

