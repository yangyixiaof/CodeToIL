#pragma once
#include "Statement.h"
class StmtContinue:
	public Statement
{
public:
    StmtContinue();
    explicit StmtContinue(const Statement* parent);
    ~StmtContinue() override;


};