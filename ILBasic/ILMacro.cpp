#include "ILMacro.h"
#include "ILRef.h"
using namespace std;

ILMacro::ILMacro()
{
    this->objType = ILObject::TypeMacro;
}

ILMacro::ILMacro(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeMacro;
}

ILMacro::~ILMacro()
{
}


void ILMacro::release()
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
