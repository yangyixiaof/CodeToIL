#pragma once
#include <string>
#include <vector>
#include "ILObject.h"


class ILRef;
class ILHeadFile:
	public ILObject
{
public:
    ILHeadFile();
    explicit ILHeadFile(const ILObject* parent);
    ~ILHeadFile() override;

	//����
	std::string name;
	
	//�ӽڵ�
	std::vector<ILRef*> refs;

	void release() override;
};

