#include "StmtWhile.h"

StmtWhile::StmtWhile()
{
    this->type = Statement::While;
}

StmtWhile::StmtWhile(const Statement* parent)
{
    this->type = Statement::While;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtWhile::~StmtWhile()
{
}

