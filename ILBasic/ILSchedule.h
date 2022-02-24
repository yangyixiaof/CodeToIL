#pragma once
#include <vector>
#include "ILObject.h"


class ILScheduleNode;
class ILLocalVariable;
class ILSchedule:
	public ILObject
{
public:
    ILSchedule();
    explicit ILSchedule(const ILObject* parent);
    ~ILSchedule() override;

	//属性
	std::vector<ILLocalVariable*> localVariables;
	std::vector<ILScheduleNode*> scheduleNodes;

	void release() override;
};

