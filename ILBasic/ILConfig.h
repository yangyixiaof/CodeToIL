#pragma once
#include <string>
#include "ILObject.h"


class ILConfig:
	public ILObject
{
public:
    ILConfig();
    explicit ILConfig(const ILObject* parent);
    ~ILConfig() override;

	int iterationCount = -1;
	std::string mainInitFunction;
	std::string mainExecFunction;
	std::string mainCompositeState;
	
};

