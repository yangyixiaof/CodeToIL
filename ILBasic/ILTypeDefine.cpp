#include "ILTypeDefine.h"
#include "ILRef.h"
using namespace std;

ILTypeDefine::ILTypeDefine()
{
    this->objType = ILObject::TypeTypeDefine;
}

ILTypeDefine::ILTypeDefine(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeTypeDefine;
}

ILTypeDefine::~ILTypeDefine()
{
}

void ILTypeDefine::release()
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
