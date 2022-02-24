#include "ILInput.h"
#include "ILRef.h"
using namespace std;

ILInput::ILInput()
{
    this->objType = ILObject::TypeInput;
}

ILInput::ILInput(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeInput;
}

ILInput::~ILInput()
{
}


ILInput* ILInput::clone(const ILObject* parent) const
{
	ILInput* ret = new ILInput();
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

void ILInput::release()
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
