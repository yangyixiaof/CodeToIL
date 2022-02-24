#include "ILLocalVariable.h"

#include "Expression.h"
#include "ILRef.h"
using namespace std;

ILLocalVariable::ILLocalVariable()
{
    this->objType = ILObject::TypeLocalVariable;
}

ILLocalVariable::ILLocalVariable(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeLocalVariable;
}

ILLocalVariable::~ILLocalVariable()
{

}


void ILLocalVariable::release()
{
	for(int i = 0;i < refs.size();i++)
	{
		refs[i]->release();
        delete refs[i];
	}
    refs.clear();
    if(defaultValue)
    {
        defaultValue->release();
        delete defaultValue;
        defaultValue = nullptr;
    }
    //comment = true;
	//delete this;
}
