#include "StmtLocalVariable.h"

StmtLocalVariable::StmtLocalVariable()
{
    this->type = Statement::LocalVariable;
}

StmtLocalVariable::StmtLocalVariable(const Statement* parent)
{
    this->type = Statement::LocalVariable;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtLocalVariable::~StmtLocalVariable()
{
}

