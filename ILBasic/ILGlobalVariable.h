#pragma once
#include <string>
#include <vector>
#include "ILObject.h"


class Expression;
class ILRef;
class ILGlobalVariable:
	public ILObject
{
public:
    ILGlobalVariable();
    explicit ILGlobalVariable(const ILObject* parent);
    ~ILGlobalVariable() override;

	//����
	std::string name;
	std::string type;
	int isAddress = 0;
	std::vector<int> arraySize;
	Expression* defaultValue = nullptr;
    bool isStatic = false;
    int align = 0;
    bool isVolatile = false;
    bool isConst = false;

	//�ӽڵ�
	std::vector<ILRef*> refs;

	void release() override;
};

