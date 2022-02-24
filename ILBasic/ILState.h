#pragma once
#include <string>
#include "ILObject.h"


class ILState:
	public ILObject
{
public:
    ILState();
    explicit ILState(const ILObject* parent);
    ~ILState() override;
	// Ù–‘
	std::string name;
	bool isInitialState = false;
	bool isFinalState = false;

	void release() override;
};

