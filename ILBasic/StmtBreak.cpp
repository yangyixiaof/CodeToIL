#include "StmtBreak.h"

StmtBreak::StmtBreak()
{
    this->type = Statement::Break;
}

StmtBreak::StmtBreak(const Statement* parent)
{
    this->type = Statement::Break;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtBreak::~StmtBreak()
{
}

