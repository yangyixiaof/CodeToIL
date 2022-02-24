#include "ILEnumDefine.h"
#include "ILRef.h"
#include "ILEnumValue.h"
using namespace std;

ILEnumDefine::ILEnumDefine()
{
    this->objType = ILObject::TypeEnumDefine;
}

ILEnumDefine::ILEnumDefine(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeEnumDefine;
}

ILEnumDefine::~ILEnumDefine()
{
}

void ILEnumDefine::release()
{
	for(int i = 0;i < refs.size();i++)
	{
		refs[i]->release();
        delete refs[i];
	}
	for(int i = 0;i < enumValues.size();i++)
	{
		enumValues[i]->release();
        delete enumValues[i];
	}
    refs.clear();
    enumValues.clear();
    //comment = true;
	//delete this;

}
