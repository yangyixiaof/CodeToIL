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

	//����
	std::string name;
	std::string value;

	//�ӽڵ�
	std::vector<ILRef*> refs;

	void release() override;
};

