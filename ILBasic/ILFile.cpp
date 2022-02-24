#include "ILFile.h"
#include "ILHeadFile.h"
#include "ILMacro.h"
#include "ILTypeDefine.h"
#include "ILStructDefine.h"
#include "ILGlobalVariable.h"
#include "ILUtilityFunction.h"
#include "ILFunction.h"
using namespace std;

ILFile::ILFile()
{
    this->objType = ILObject::TypeFile;
}

ILFile::ILFile(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeFile;
}

ILFile::~ILFile()
{
}


void ILFile::release()
{
	for(int i = 0;i < headFiles.size();i++)
	{
		headFiles[i]->release();
        delete headFiles[i];
	}
	for(int i = 0;i < macros.size();i++)
	{
		macros[i]->release();
        delete macros[i];
	}
	for(int i = 0;i < typeDefines.size();i++)
	{
		typeDefines[i]->release();
        delete typeDefines[i];
	}
	for(int i = 0;i < structDefines.size();i++)
	{
		structDefines[i]->release();
        delete structDefines[i];
	}
	for(int i = 0;i < globalVariables.size();i++)
	{
		globalVariables[i]->release();
        delete globalVariables[i];
	}
	for(int i = 0;i < utilityFunctions.size();i++)
	{
		utilityFunctions[i]->release();
        delete utilityFunctions[i];
	}
	for(int i = 0;i < functions.size();i++)
	{
		functions[i]->release();
        delete functions[i];
	}
    headFiles.clear();
    macros.clear();
    typeDefines.clear();
    structDefines.clear();
    globalVariables.clear();
    utilityFunctions.clear();
    functions.clear();
    //comment = true;
	//delete this;
}
