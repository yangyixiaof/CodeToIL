#pragma once
#include "Statement.h"
class StmtElse:
	public Statement
{
public:
    StmtElse();
    explicit StmtElse(const Statement* parent);
    ~StmtElse() override;

	std::vector<Statement*> statements;
};