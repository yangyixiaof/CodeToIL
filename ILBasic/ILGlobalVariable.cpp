#include "ILGlobalVariable.h"
#include "ILRef.h"
using namespace std;

ILGlobalVariable::ILGlobalVariable()
{
    this->objType = ILObject::TypeGlobalVariable;
}

ILGlobalVariable::ILGlobalVariable(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeGlobalVariable;
}

ILGlobalVariable::~ILGlobalVariable()
{
}


void ILGlobalVariable::release()
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
