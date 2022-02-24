#pragma once
#include <string>
#include <vector>
#include "ILObject.h"


class ILRef;
class ILMacro:
	public ILObject
{
public:
    ILMacro();
    explicit ILMacro(const ILObject* parent);
    ~ILMacro() override;

	//����
	std::string name;
	std::string value;
	
	//�ӽڵ�
	std::vector<ILRef*> refs;

	void release() override;
};

