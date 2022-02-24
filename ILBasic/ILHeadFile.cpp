#include "ILHeadFile.h"
#include "ILRef.h"
using namespace std;

ILHeadFile::ILHeadFile()
{
    this->objType = ILObject::TypeHeadFile;
}

ILHeadFile::ILHeadFile(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeHeadFile;
}

ILHeadFile::~ILHeadFile()
{
}


void ILHeadFile::release()
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
