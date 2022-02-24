#pragma once
#include <string>
#include <vector>
#include "ILObject.h"


class ILMember;
class ILRef;
class ILEnumValue:
	public ILObject
{
public:
    ILEnumValue();
    explicit ILEnumValue(const ILObject* parent);
    ~ILEnumValue() override;

	// Ù–‘
	std::string name;
    std::string value;

	void release() override;
};

