#pragma once
#include <string>
#include <vector>
#include "ILObject.h"


class ILHeadFile;
class ILMacro;
class ILTypeDefine;
class ILUtilityFunction;
class ILEnumDefine;
class ILStructDefine;
class ILGlobalVariable;
class ILFunction;
class ILFile:
	public ILObject
{
public:
    ILFile();
    explicit ILFile(const ILObject* parent);
    ~ILFile() override;
	//属性
	std::string name;
    bool isHeadFile = false;
    bool isIndependent = false;
	
	//子节点
	std::vector<ILHeadFile*> headFiles;
	std::vector<ILMacro*> macros;
	std::vector<ILTypeDefine*> typeDefines;
	std::vector<ILEnumDefine*> enumDefines;
	std::vector<ILStructDefine*> structDefines;
	std::vector<ILGlobalVariable*> globalVariables;
	std::vector<ILUtilityFunction*> utilityFunctions;
	std::vector<ILFunction*> functions;

	void release() override;
};

