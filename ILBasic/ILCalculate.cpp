#include "ILCalculate.h"
#include "ILRef.h"
#include "ILInput.h"
#include "ILOutput.h"
#include "Statement.h"
using namespace std;

ILCalculate::ILCalculate()
{
    this->objType = ILObject::TypeCalculate;
}

ILCalculate::ILCalculate(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeCalculate;
}

ILCalculate::~ILCalculate()
{
}


string ILCalculate::getRefActorType() const
{
	if(refs.empty())
		return "";
    return refs[0]->actor;
}

void ILCalculate::release()
{
	for(int i = 0;i < refs.size();i++)
	{
		refs[i]->release();
        delete refs[i];
	}
	for(int i = 0;i < inputs.size();i++)
	{
		inputs[i]->release();
        delete inputs[i];
	}
	for(int i = 0;i < outputs.size();i++)
	{
		outputs[i]->release();
        delete outputs[i];
	}
	for(int i = 0;i < statements.childStatements.size();i++)
	{
		statements.childStatements[i]->release();
        delete statements.childStatements[i];
	}
	statements.childStatements.clear();
    refs.clear();
    inputs.clear();
    outputs.clear();
    
    //comment = true;
	//delete this;
}
