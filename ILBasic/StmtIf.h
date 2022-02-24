#pragma once
#include "Statement.h"
class StmtIf:
	public Statement
{
public:
    StmtIf();
    explicit StmtIf(const Statement* parent);
    ~StmtIf() override;

	Expression* condition = nullptr;
	std::vector<Statement*> statements;
};