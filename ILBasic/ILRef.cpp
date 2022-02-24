#include "ILRef.h"
using namespace std;

ILRef::ILRef()
{
    this->objType = ILObject::TypeRef;
}

ILRef::ILRef(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeRef;
}

ILRef::~ILRef()
{
}

ILRef* ILRef::clone(const ILObject* parent)
{
	ILRef* ret = new ILRef();
	ret->objType = this->objType;
	ret->actor = this->actor;
	ret->path = this->path;
	ret->parent = const_cast<ILObject*>(parent);
    return ret;
}

void ILRef::release()
{
    //comment = true;
	//delete this;
}
