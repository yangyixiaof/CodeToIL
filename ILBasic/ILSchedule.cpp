#include "ILSchedule.h"
#include "ILLocalVariable.h"
#include "ILScheduleNode.h"
using namespace std;

ILSchedule::ILSchedule()
{
    this->objType = ILObject::TypeSchedule;
}

ILSchedule::ILSchedule(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeSchedule;
}

ILSchedule::~ILSchedule()
{
}

void ILSchedule::release()
{
	for(int i = 0;i < localVariables.size();i++)
	{
		localVariables[i]->release();
        delete localVariables[i];
	}
	for(int i = 0;i < scheduleNodes.size();i++)
	{
		scheduleNodes[i]->release();
        delete scheduleNodes[i];
	}
    localVariables.clear();
    scheduleNodes.clear();
    //comment = true;
	//delete this;
}
