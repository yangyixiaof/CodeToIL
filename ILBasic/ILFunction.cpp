#include "ILFunction.h"

#include "ILRef.h"
#include "ILInput.h"
#include "ILOutput.h"
#include "ILSchedule.h"
using namespace std;


map<string, ILFunction::Type> ILFunction::functionTypeStrToTypeMap = {
    pair<string, ILFunction::Type>("ProgramInit", ILFunction::ProgramInit),
    pair<string, ILFunction::Type>("ProgramExit", ILFunction::ProgramExit),
    pair<string, ILFunction::Type>("FunctionInit", ILFunction::FunctionInit),
    pair<string, ILFunction::Type>("FunctionExec", ILFunction::FunctionExec),
    pair<string, ILFunction::Type>("FunctionExit", ILFunction::FunctionExit),
    pair<string, ILFunction::Type>("IterationEnd", ILFunction::IterationEnd),
    pair<string, ILFunction::Type>("IterationStart", ILFunction::IterationStart),
};

map<ILFunction::Type, string> ILFunction::functionTypeToTypeStrMap = {
    pair<ILFunction::Type, string>(ILFunction::ProgramInit, "ProgramInit"),
    pair<ILFunction::Type, string>(ILFunction::ProgramExit, "ProgramExit"),
    pair<ILFunction::Type, string>(ILFunction::FunctionInit, "FunctionInit"),
    pair<ILFunction::Type, string>(ILFunction::FunctionExec, "FunctionExec"),
    pair<ILFunction::Type, string>(ILFunction::FunctionExit, "FunctionExit"),
    pair<ILFunction::Type, string>(ILFunction::IterationEnd, "IterationEnd"),
    pair<ILFunction::Type, string>(ILFunction::IterationStart, "IterationStart"),
    pair<ILFunction::Type, string>(ILFunction::FunctionUnknown, ""),
};


ILFunction::ILFunction()
{
    this->objType = ILObject::TypeFunction;
}

ILFunction::ILFunction(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeFunction;
}

ILFunction::~ILFunction()
{
}


void ILFunction::release()
{
	for(int i = 0;i < refs.size();i++)
	{
		refs[i]->release();
        delete refs[i];
	}
	for(int i = 0;i < inputs.size();i++)
	{
		inputs[i]->release();
        delete inputs[i];
	}
	for(int i = 0;i < outputs.size();i++)
	{
		outputs[i]->release();
        delete outputs[i];
	}
    refs.clear();
    inputs.clear();
    inputs.clear();
    if(schedule)
    {
        schedule->release();
        delete schedule;
        schedule = nullptr;
    }
    //comment = true;
	//delete this;
}
