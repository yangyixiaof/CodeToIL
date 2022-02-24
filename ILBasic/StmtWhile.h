#pragma once
#include "Statement.h"
class StmtWhile:
	public Statement
{
public:
    StmtWhile();
    explicit StmtWhile(const Statement* parent);
    ~StmtWhile() override;

	Expression* condition = nullptr;
	std::vector<Statement*> statements;
};

