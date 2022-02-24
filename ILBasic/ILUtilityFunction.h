#pragma once
#include <string>
#include <vector>
#include "ILObject.h"


class ILRef;
class ILUtilityFunction:
	public ILObject
{
public:
    ILUtilityFunction();
    explicit ILUtilityFunction(const ILObject* parent);
    ~ILUtilityFunction() override;

	//����
	std::string name;

	//�ӽڵ�
	std::vector<ILRef*> refs;

	void release() override;
};

