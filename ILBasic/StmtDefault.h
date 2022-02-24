#pragma once
#include "Statement.h"
class StmtDefault:
	public Statement
{
public:
    StmtDefault();
    explicit StmtDefault(const Statement* parent);
    ~StmtDefault() override;


};