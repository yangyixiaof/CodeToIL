#include "StmtAssemble.h"

StmtAssemble::StmtAssemble()
{
    this->type = Statement::Assemble;
}

StmtAssemble::StmtAssemble(const Statement* parent)
{
    this->type = Statement::Assemble;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtAssemble::~StmtAssemble()
{
}

