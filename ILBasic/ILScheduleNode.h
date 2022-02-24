#pragma once
#include "ILObject.h"


class ILRef;
class ILScheduleNode:
	public ILObject
{
public:
    ILScheduleNode();
    ~ILScheduleNode() override;
};

