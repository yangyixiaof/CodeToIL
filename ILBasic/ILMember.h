#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "ILObject.h"


class ILMember:
	public ILObject
{
public:
    ILMember();
    explicit ILMember(const ILObject* parent);
    ~ILMember() override;
	//����
	std::string name;
	std::string type;
	int isAddress = 0;
	std::vector<int> arraySize;
    int align = 0;
    bool isVolatile = false;
    bool isConst = false;
    
	//�ӽڵ�
	std::vector<ILRef*> refs;

	void release() override;
};

