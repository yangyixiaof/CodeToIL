#pragma once
#include <string>
#include <vector>
#include <map>

#include "ILObject.h"

class ILRef;
class ILInput;
class ILOutput;
class ILSchedule;
class ILState;
class ILFunction:
	public ILObject
{
public:
    ILFunction();
    explicit ILFunction(const ILObject* parent);
    ~ILFunction() override;

	//属性
	std::string name;
    std::string returnType;
    bool isStatic = false;

	enum Type{
		ProgramInit,
		ProgramExit,
		IterationStart,
		IterationEnd,
		FunctionInit,
		FunctionExec,
		FunctionExit,
		FunctionUnknown,
	};
	Type type = FunctionUnknown;
	
	//子节点
	std::vector<ILRef*> refs;
	std::vector<ILInput*> inputs;
	std::vector<ILOutput*> outputs;
	std::vector<ILState*> states;
	ILSchedule* schedule = nullptr;

	void release() override;

    static std::map<std::string, ILFunction::Type> functionTypeStrToTypeMap;
    static std::map<ILFunction::Type, std::string> functionTypeToTypeStrMap;
};

