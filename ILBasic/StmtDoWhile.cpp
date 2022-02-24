#include "StmtDoWhile.h"

StmtDoWhile::StmtDoWhile()
{
    this->type = Statement::DoWhile;
}

StmtDoWhile::StmtDoWhile(const Statement* parent)
{
    this->type = Statement::DoWhile;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtDoWhile::~StmtDoWhile()
{
}

