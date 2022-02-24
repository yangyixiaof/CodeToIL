#include "StmtReturn.h"

StmtReturn::StmtReturn()
{
    this->type = Statement::Return;
}

StmtReturn::StmtReturn(const Statement* parent)
{
    this->type = Statement::Return;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtReturn::~StmtReturn()
{
}

