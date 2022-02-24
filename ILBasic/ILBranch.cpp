#include "ILBranch.h"

#include "ILCalculate.h"
#include "ILScheduleNode.h"
#include "ILRef.h"
using namespace std;


ILBranch::ILBranch()
{
    this->objType = ILObject::TypeBranch;
}

ILBranch::ILBranch(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeBranch;
}

ILBranch::~ILBranch()
{
}


void ILBranch::release()
{
	for(int i = 0;i < scheduleNodes.size();i++)
	{
		scheduleNodes[i]->release();
        delete scheduleNodes[i];
	}
	for(int i = 0;i < refs.size();i++)
	{
		refs[i]->release();
        delete refs[i];
	}
    scheduleNodes.clear();
    refs.clear();

    //comment = true;
	//delete this;
}

bool ILBranch::isInnerEmpty()
{
    for(int i = this->scheduleNodes.size() - 1; i >= 0; i--)
    {
        if(this->scheduleNodes[i]->objType == ILObject::TypeBranch)
        {
            ILBranch* branch = static_cast<ILBranch*>(this->scheduleNodes[i]);
            if(!branch->isInnerEmpty())
                return false;
        }
        else if(this->scheduleNodes[i]->objType == ILObject::TypeCalculate)
        {
            ILCalculate* claculate = static_cast<ILCalculate*>(this->scheduleNodes[i]);
            if(!claculate->statements.childStatements.empty())
                return false;
        }
    }
    return true;
}
