#pragma once
#include "Statement.h"
class StmtDoWhile:
	public Statement
{
public:
    StmtDoWhile();
    explicit StmtDoWhile(const Statement* parent);
    ~StmtDoWhile() override;

	Expression* condition = nullptr;
	std::vector<Statement*> statements;
};