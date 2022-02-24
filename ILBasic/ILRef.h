#pragma once
#include <string>
#include "ILObject.h"


class ILRef:
	public ILObject
{
public:
    ILRef();
    explicit ILRef(const ILObject* parent);
    ~ILRef() override;

	//����
	std::string actor;
	std::string path;

	ILRef* clone(const ILObject* parent);
	void release() override;
};

