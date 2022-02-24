#pragma once
#include "Statement.h"
class StmtElseIf:
	public Statement
{
public:
    StmtElseIf();
    explicit StmtElseIf(const Statement* parent);
    ~StmtElseIf() override;

	Expression* condition = nullptr;
	std::vector<Statement*> statements;
};