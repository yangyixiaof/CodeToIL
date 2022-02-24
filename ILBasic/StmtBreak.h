#pragma once
#include "Statement.h"
class StmtBreak:
	public Statement
{
public:
    StmtBreak();
    explicit StmtBreak(const Statement* parent);
    ~StmtBreak() override;


};