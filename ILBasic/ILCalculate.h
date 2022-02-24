#pragma once
#include <string>
#include <vector>

#include "ILScheduleNode.h"
#include "Statement.h"


class ILRef;
class ILInput;
class ILOutput;
class ILBatchCalculation;
class ILCalculate :
    public ILScheduleNode
{
public:
    ILCalculate();
    explicit ILCalculate(const ILObject* parent);
    ~ILCalculate() override;

	//属性
	std::string name;
	bool isOptimized = false;
	
	//子节点
	std::vector<ILRef*> refs;
	std::vector<ILInput*> inputs;
	std::vector<ILOutput*> outputs;
	Statement statements;
    
	std::vector<ILBatchCalculation*> batchCalculations;

	std::string getRefActorType() const;

	void release() override;
};

