#pragma once
#include <string>
#include <vector>
#include "ILObject.h"


class ILEnumValue;
class ILRef;
class ILEnumDefine:
	public ILObject
{
public:
    ILEnumDefine();
    explicit ILEnumDefine(const ILObject* parent);
    ~ILEnumDefine() override;

	//����
	std::string name;

	//�ӽڵ�
	std::vector<ILRef*> refs;
	std::vector<ILEnumValue*> enumValues;

	void release() override;
};

