#include "StmtSwitch.h"

StmtSwitch::StmtSwitch()
{
    this->type = Statement::Switch;
}

StmtSwitch::StmtSwitch(const Statement* parent)
{
    this->type = Statement::Switch;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtSwitch::~StmtSwitch()
{
}

