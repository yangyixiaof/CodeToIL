#pragma once
#include "Statement.h"
class StmtSwitch:
	public Statement
{
public:
    StmtSwitch();
    explicit StmtSwitch(const Statement* parent);
    ~StmtSwitch() override;

	Expression* condition = nullptr;
	std::vector<Statement*> statements;
};