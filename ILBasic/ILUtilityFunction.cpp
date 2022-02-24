#include "ILUtilityFunction.h"
#include "ILRef.h"
using namespace std;

ILUtilityFunction::ILUtilityFunction()
{
    this->objType = ILObject::TypeUtilityFunction;
}

ILUtilityFunction::ILUtilityFunction(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeUtilityFunction;
}

ILUtilityFunction::~ILUtilityFunction()
{
}

void ILUtilityFunction::release()
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
