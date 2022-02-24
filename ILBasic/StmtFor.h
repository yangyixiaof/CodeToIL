#pragma once
#include "Statement.h"
class StmtFor:
	public Statement
{
public:
    StmtFor();
    explicit StmtFor(const Statement* parent);
    ~StmtFor() override;

	std::vector<Statement*> initial;
	Expression* condition = nullptr;
	std::vector<Statement*> iterator;
	std::vector<Statement*> statements;
};