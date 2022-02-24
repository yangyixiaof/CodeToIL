#pragma once
#include "Statement.h"
class StmtReturn:
	public Statement
{
public:
    StmtReturn();
    explicit StmtReturn(const Statement* parent);
    ~StmtReturn() override;

	Expression* expression = nullptr;
};