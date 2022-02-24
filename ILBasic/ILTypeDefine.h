#pragma once
#include <string>
#include <vector>
#include "ILObject.h"


class ILRef;
class ILTypeDefine:
	public ILObject
{
	
public:
    ILTypeDefine();
    explicit ILTypeDefine(const ILObject* parent);
    ~ILTypeDefine() override;

	//属性
	std::string name;
	std::string value;

	//子节点
	std::vector<ILRef*> refs;

	void release() override;
};

