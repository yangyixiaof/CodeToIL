#pragma once
#include "ILScheduleNode.h"


class Expression;

class ILBranch :
    public ILScheduleNode
{
public:
    ILBranch();
    explicit ILBranch(const ILObject* parent);
    ~ILBranch() override;

	//����
	enum BranchType{
		TypeIf,
		TypeElse,
		TypeElseIf,
		TypeFor,
		TypeWhile,
		TypeUnknown,
	};
	BranchType type = TypeUnknown;
	Expression* condition = nullptr;

	//�ӽڵ�
	std::vector<ILScheduleNode*> scheduleNodes;
	std::vector<ILRef*> refs;

	void release() override;

    bool isInnerEmpty();
};

