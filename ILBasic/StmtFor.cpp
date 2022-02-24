#include "StmtFor.h"

StmtFor::StmtFor()
{
    this->type = Statement::For;
}

StmtFor::StmtFor(const Statement* parent)
{
    this->type = Statement::For;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtFor::~StmtFor()
{
}

