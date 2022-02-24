#pragma once
#include "Statement.h"
class StmtAssemble:
	public Statement
{
public:
    StmtAssemble();
    explicit StmtAssemble(const Statement* parent);
    ~StmtAssemble() override;

    
    class StmtAssembleInput
    {
    public:
	    std::string name;
	    std::string type;
	    int isAddress = 0;
	    std::vector<int> arraySize;
        Expression* defaultValue = nullptr;
    };

    class StmtAssembleOutput
    {
    public:
	    std::string name;
	    std::string type;
	    int isAddress = 0;
	    std::vector<int> arraySize;
        Expression* defaultValue = nullptr;
    };

    std::vector<StmtAssembleInput> inputs;
    std::vector<StmtAssembleOutput> outputs;

    std::vector<std::string> expressions;
};