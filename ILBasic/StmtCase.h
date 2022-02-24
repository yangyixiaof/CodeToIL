#pragma once
#include "Statement.h"
class StmtCase:
	public Statement
{
public:
    StmtCase();
    explicit StmtCase(const Statement* parent);
    ~StmtCase() override;

	Expression* value = nullptr;
};