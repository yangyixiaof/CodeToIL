#include "ILOutput.h"
#include "ILRef.h"
using namespace std;

ILOutput::ILOutput()
{
    this->objType = ILObject::TypeOutput;
}

ILOutput::ILOutput(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeOutput;
}

ILOutput::~ILOutput()
{
}

ILOutput* ILOutput::clone(const ILObject* parent) const
{
	ILOutput* ret = new ILOutput();
	ret->objType = this->objType;
	ret->name = this->name;
	ret->type = this->type;
	ret->isAddress = this->isAddress;
	ret->arraySize = this->arraySize;
	ret->defaultValue = this->defaultValue;
	ret->sourceStr = this->sourceStr;
	for(int i = 0; i < this->refs.size(); i++)
	{
		ret->refs.push_back(this->refs[i]->clone(ret));
	}
	ret->parent = const_cast<ILObject*>(parent);
    return ret;
}

void ILOutput::release()
{
	for(int i = 0;i < refs.size();i++)
	{
		refs[i]->release();
        delete refs[i];
	}
    refs.clear();
    //comment = true;
	//delete this;
}
