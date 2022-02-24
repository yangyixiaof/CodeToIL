#include "ILStructDefine.h"
#include "ILRef.h"
#include "ILMember.h"
using namespace std;

ILStructDefine::ILStructDefine()
{
    this->objType = ILObject::TypeStructDefine;
}

ILStructDefine::ILStructDefine(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeStructDefine;
}

ILStructDefine::~ILStructDefine()
{
}

void ILStructDefine::release()
{
	for(int i = 0;i < refs.size();i++)
	{
		refs[i]->release();
        delete refs[i];
	}
	for(int i = 0;i < members.size();i++)
	{
		members[i]->release();
        delete members[i];
	}
    refs.clear();
    members.clear();
    //comment = true;
	//delete this;

}

ILMember* ILStructDefine::getMemberByName(string name)
{
	for (int i = 0; i < this->members.size(); i++)
	{
	    if (this->members[i]->name == name)
	        return this->members[i];
	}
	return nullptr;
}

ILMember* ILStructDefine::getMemberByIndex(int index)
{
	if(index >= 0 && index < this->members.size())
		return this->members[index];
	return nullptr;
}