#pragma once
#include <string>
#include <vector>
#include "ILObject.h"


class Expression;
class ILOutput;
class ILRef;
class ILInput:
	public ILObject
{
public:
    ILInput();
    explicit ILInput(const ILObject* parent);
    ~ILInput() override;

	//����
	std::string name;
	std::string type;
	int isAddress = 0;
	std::vector<int> arraySize;
	//std::string defaultValue;
    Expression* defaultValue = nullptr;
	std::string sourceStr;
	//ILOutput* source;
    bool isConst = false;
	
	//�ӽڵ�
	std::vector<ILRef*> refs;

	ILInput* clone(const ILObject* parent) const;
	void release() override;
};

