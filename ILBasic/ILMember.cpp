#include "ILMember.h"
using namespace std;

ILMember::ILMember()
{
    this->objType = ILObject::TypeMember;
}

ILMember::ILMember(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeMember;
}

ILMember::~ILMember()
{
}

void ILMember::release()
{
    //comment = true;
	//delete this;
}
