#pragma once
#include <string>
#include <vector>
#include "ILScheduleNode.h"


class Expression;
class ILRef;
class ILLocalVariable :
    public ILScheduleNode
{
public:
    ILLocalVariable();
    explicit ILLocalVariable(const ILObject* parent);
    ~ILLocalVariable() override;

	//����
	std::string name;
	std::string type;
	int isAddress = 0;
	std::vector<int> arraySize;
	//std::string defaultValue;
    int align = 0;
    bool isVolatile = false;
    bool isConst = false;
    Expression* defaultValue = nullptr;

	//�ӽڵ�
	std::vector<ILRef*> refs;

	void release() override;
};

