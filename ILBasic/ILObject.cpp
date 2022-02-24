#include "ILObject.h"

#include <algorithm>

#include "Statement.h"
#include "ILFile.h"
#include "ILHeadFile.h"
#include "ILMacro.h"
#include "ILTypeDefine.h"
#include "ILUtilityFunction.h"
#include "ILStructDefine.h"
#include "ILGlobalVariable.h"
#include "ILFunction.h"
#include "ILBranch.h"
#include "ILCalculate.h"
#include "ILEnumDefine.h"
#include "ILEnumValue.h"
#include "ILLocalVariable.h"
#include "ILInput.h"
#include "ILOutput.h"
#include "ILRef.h"
#include "ILSchedule.h"
#include "ILMember.h"

using namespace std;

vector<pair<ILObject::ObjType, ILObject::ObjType>>ILObject::objTypeParentToChild = {
    pair<ObjType, ObjType>(TypeFile, TypeHeadFile),
    pair<ObjType, ObjType>(TypeFile, TypeMacro),
    pair<ObjType, ObjType>(TypeFile, TypeTypeDefine),
    pair<ObjType, ObjType>(TypeFile, TypeStructDefine),
    pair<ObjType, ObjType>(TypeFile, TypeEnumDefine),
    pair<ObjType, ObjType>(TypeFile, TypeGlobalVariable),
    pair<ObjType, ObjType>(TypeFile, TypeUtilityFunction),
    pair<ObjType, ObjType>(TypeFile, TypeFunction),
    pair<ObjType, ObjType>(TypeHeadFile, TypeRef),
    pair<ObjType, ObjType>(TypeMacro, TypeRef),
    pair<ObjType, ObjType>(TypeTypeDefine, TypeRef),
    pair<ObjType, ObjType>(TypeStructDefine, TypeMember),
    pair<ObjType, ObjType>(TypeStructDefine, TypeRef),
    pair<ObjType, ObjType>(TypeEnumDefine, TypeRef),
    pair<ObjType, ObjType>(TypeEnumDefine, TypeEnumValue),
    pair<ObjType, ObjType>(TypeGlobalVariable, TypeRef),
    pair<ObjType, ObjType>(TypeUtilityFunction, TypeRef),
    pair<ObjType, ObjType>(TypeFunction, TypeRef),
    pair<ObjType, ObjType>(TypeFunction, TypeInput),
    pair<ObjType, ObjType>(TypeFunction, TypeOutput),
    pair<ObjType, ObjType>(TypeFunction, TypeSchedule),
    pair<ObjType, ObjType>(TypeBranch, TypeBranch),
    pair<ObjType, ObjType>(TypeBranch, TypeCalculate),
    pair<ObjType, ObjType>(TypeBranch, TypeRef),
    pair<ObjType, ObjType>(TypeCalculate, TypeRef),
    pair<ObjType, ObjType>(TypeCalculate, TypeInput),
    pair<ObjType, ObjType>(TypeCalculate, TypeOutput),
    pair<ObjType, ObjType>(TypeLocalVariable, TypeRef),
    pair<ObjType, ObjType>(TypeInput, TypeRef),
    pair<ObjType, ObjType>(TypeOutput, TypeRef),
    pair<ObjType, ObjType>(TypeSchedule, TypeBranch),
    pair<ObjType, ObjType>(TypeSchedule, TypeCalculate),
    pair<ObjType, ObjType>(TypeSchedule, TypeLocalVariable),
};

ILObject::ILObject()
{
    this->objType = ILObject::TypeUnknown;
}

ILObject::~ILObject()
{
}

vector<ILHeadFile*>* ILObject::getListOfILHeadFile() const
{
    if(this->objType == ILObject::TypeFile) {
        return &(static_cast<ILFile*>(const_cast<ILObject*>(this))->headFiles);
    }
    return nullptr;
}

vector<ILMacro*>* ILObject::getListOfILMacro() const
{
    if(this->objType == ILObject::TypeFile) {
        return &(static_cast<ILFile*>(const_cast<ILObject*>(this))->macros);
    }
    return nullptr;
}

vector<ILStructDefine*>* ILObject::getListOfILStructDefine() const
{
    if(this->objType == ILObject::TypeFile) {
        return &(static_cast<ILFile*>(const_cast<ILObject*>(this))->structDefines);
    }
    return nullptr;
}

vector<ILGlobalVariable*>* ILObject::getListOfILGlobalVariable() const
{
    if(this->objType == ILObject::TypeFile) {
        return &(static_cast<ILFile*>(const_cast<ILObject*>(this))->globalVariables);
    }
    return nullptr;
}

vector<ILUtilityFunction*>* ILObject::getListOfILUtilityFunction() const
{
    if(this->objType == ILObject::TypeFile) {
        return &(static_cast<ILFile*>(const_cast<ILObject*>(this))->utilityFunctions);
    }
    return nullptr;
}

vector<ILFunction*>* ILObject::getListOfILFunction() const
{
    if(this->objType == ILObject::TypeFile) {
        return &(static_cast<ILFile*>(const_cast<ILObject*>(this))->functions);
    }
    return nullptr;
}

vector<ILRef*>* ILObject::getListOfILRef() const
{
    if(this->objType == ILObject::TypeBranch) {
        return &(static_cast<ILBranch*>(const_cast<ILObject*>(this))->refs);
    }else if(this->objType == ILObject::TypeCalculate) {
        return &(static_cast<ILCalculate*>(const_cast<ILObject*>(this))->refs);
    }else if(this->objType == ILObject::TypeFunction) {
        return &(static_cast<ILFunction*>(const_cast<ILObject*>(this))->refs);
    }else if(this->objType == ILObject::TypeGlobalVariable) {
        return &(static_cast<ILGlobalVariable*>(const_cast<ILObject*>(this))->refs);
    }else if(this->objType == ILObject::TypeHeadFile) {
        return &(static_cast<ILHeadFile*>(const_cast<ILObject*>(this))->refs);
    }else if(this->objType == ILObject::TypeInput) {
        return &(static_cast<ILInput*>(const_cast<ILObject*>(this))->refs);
    }else if(this->objType == ILObject::TypeLocalVariable) {
        return &(static_cast<ILLocalVariable*>(const_cast<ILObject*>(this))->refs);
    }else if(this->objType == ILObject::TypeMacro) {
        return &(static_cast<ILMacro*>(const_cast<ILObject*>(this))->refs);
    }else if(this->objType == ILObject::TypeOutput) {
        return &(static_cast<ILOutput*>(const_cast<ILObject*>(this))->refs);
    }else if(this->objType == ILObject::TypeStructDefine) {
        return &(static_cast<ILStructDefine*>(const_cast<ILObject*>(this))->refs);
    }else if(this->objType == ILObject::TypeTypeDefine) {
        return &(static_cast<ILTypeDefine*>(const_cast<ILObject*>(this))->refs);
    }else if(this->objType == ILObject::TypeUtilityFunction) {
        return &(static_cast<ILUtilityFunction*>(const_cast<ILObject*>(this))->refs);
    }
    return nullptr;
}

vector<ILMember*>* ILObject::getListOfILMember() const
{
    if(this->objType == ILObject::TypeFile) {
        return &(static_cast<ILStructDefine*>(const_cast<ILObject*>(this))->members);
    }
    return nullptr;
}

vector<ILInput*>* ILObject::getListOfILInput() const
{
    if(this->objType == ILObject::TypeCalculate) {
        return &(static_cast<ILCalculate*>(const_cast<ILObject*>(this))->inputs);
    }else if(this->objType == ILObject::TypeFunction) {
        return &(static_cast<ILFunction*>(const_cast<ILObject*>(this))->inputs);
    }
    return nullptr;
}

vector<ILOutput*>* ILObject::getListOfILOutput() const
{
    if(this->objType == ILObject::TypeCalculate) {
        return &(static_cast<ILCalculate*>(const_cast<ILObject*>(this))->outputs);
    }else if(this->objType == ILObject::TypeFunction) {
        return &(static_cast<ILFunction*>(const_cast<ILObject*>(this))->outputs);
    }
    return nullptr;
}

vector<ILScheduleNode*>* ILObject::getListOfILScheduleNode() const
{
    if(this->objType == ILObject::TypeBranch) {
        return &(static_cast<ILBranch*>(const_cast<ILObject*>(this))->scheduleNodes);
    }else if(this->objType == ILObject::TypeSchedule) {
        return &(static_cast<ILSchedule*>(const_cast<ILObject*>(this))->scheduleNodes);
    }
    return nullptr;
}

vector<ILLocalVariable*>* ILObject::getListOfILLocalVariable() const
{
    if(this->objType == ILObject::TypeSchedule) {
        return &(static_cast<ILSchedule*>(const_cast<ILObject*>(this))->localVariables);
    }
    return nullptr;
}

vector<ILTypeDefine*>* ILObject::getListOfILTypeDefine() const
{
    if(this->objType == ILObject::TypeFile) {
        return &(static_cast<ILFile*>(const_cast<ILObject*>(this))->typeDefines);
    }
    return nullptr;
}

std::vector<ILEnumDefine*>* ILObject::getListOfILEnumDefine() const
{
    if(this->objType == ILObject::TypeFile) {
        return &(static_cast<ILFile*>(const_cast<ILObject*>(this))->enumDefines);
    }
    return nullptr;
}

std::vector<ILEnumValue*>* ILObject::getListOfILEnumValue() const
{
    if(this->objType == ILObject::TypeEnumDefine) {
        return &(static_cast<ILEnumDefine*>(const_cast<ILObject*>(this))->enumValues);
    }
    return nullptr;
}

ILHeadFile* ILObject::findNextILHeadFile(const std::vector<ILHeadFile*>* list, ILHeadFile* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILMacro* ILObject::findNextILMacro(const std::vector<ILMacro*>* list, ILMacro* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILStructDefine* ILObject::findNextILStructDefine(const std::vector<ILStructDefine*>* list, 
    ILStructDefine* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILGlobalVariable* ILObject::findNextILGlobalVariable(const std::vector<ILGlobalVariable*>* list, 
    ILGlobalVariable* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILUtilityFunction* ILObject::findNextILUtilityFunction(const std::vector<ILUtilityFunction*>* list, 
    ILUtilityFunction* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILFunction* ILObject::findNextILFunction(const std::vector<ILFunction*>* list, ILFunction* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILRef* ILObject::findNextILRef(const std::vector<ILRef*>* list, ILRef* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILMember* ILObject::findNextILMember(const std::vector<ILMember*>* list, ILMember* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILInput* ILObject::findNextILInput(const std::vector<ILInput*>* list, ILInput* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILOutput* ILObject::findNextILOutput(const std::vector<ILOutput*>* list, ILOutput* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILScheduleNode* ILObject::findNextILScheduleNode(const std::vector<ILScheduleNode*>* list, 
    ILScheduleNode* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILLocalVariable* ILObject::findNextILLocalVariable(const std::vector<ILLocalVariable*>* list, 
    ILLocalVariable* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILTypeDefine* ILObject::findNextILTypeDefine(const std::vector<ILTypeDefine*>* list, ILTypeDefine* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILEnumDefine* ILObject::findNextILEnum(const std::vector<ILEnumDefine*>* list, ILEnumDefine* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILEnumValue* ILObject::findNextILEnumValue(const std::vector<ILEnumValue*>* list, ILEnumValue* obj) const
{
    if(!list)
        return nullptr;
	auto iter = std::find(list->begin(),list->end(),obj);
	if(iter == list->end())
		return nullptr;
	iter++;
	if(iter == list->end())
		return nullptr;
	return *iter;
}

ILObject* ILObject::getNextSameILObjectA() const
{
    ILObject* t = const_cast<ILObject*>(this);
    if(this->objType == ILObject::TypeHeadFile) {
        vector<ILHeadFile*>* listILHeadFile = parent->getListOfILHeadFile();
        return findNextILHeadFile(listILHeadFile, static_cast<ILHeadFile*>(t));
    } else if(this->objType == ILObject::TypeMacro) {
        vector<ILMacro*>* listILMacro = parent->getListOfILMacro();
        return findNextILMacro(listILMacro, static_cast<ILMacro*>(t));
    } else if(this->objType == ILObject::TypeStructDefine) {
        vector<ILStructDefine*>* listILStructDefine = parent->getListOfILStructDefine();
        return findNextILStructDefine(listILStructDefine, static_cast<ILStructDefine*>(t));
    } else if(this->objType == ILObject::TypeGlobalVariable) {
        vector<ILGlobalVariable*>* listILGlobalVariable = parent->getListOfILGlobalVariable();
        return findNextILGlobalVariable(listILGlobalVariable, static_cast<ILGlobalVariable*>(t));
    } else if(this->objType == ILObject::TypeUtilityFunction) {
        vector<ILUtilityFunction*>* listILUtilityFunction = parent->getListOfILUtilityFunction();
        return findNextILUtilityFunction(listILUtilityFunction, static_cast<ILUtilityFunction*>(t));
    } else if(this->objType == ILObject::TypeFunction) {
        vector<ILFunction*>* listILFunction = parent->getListOfILFunction();
        return findNextILFunction(listILFunction, static_cast<ILFunction*>(t));
    } else if(this->objType == ILObject::TypeRef) {
        vector<ILRef*>* listILRef = parent->getListOfILRef();
        return findNextILRef(listILRef, static_cast<ILRef*>(t));
    } else if(this->objType == ILObject::TypeMember) {
        vector<ILMember*>* listILMember = parent->getListOfILMember();
        return findNextILMember(listILMember, static_cast<ILMember*>(t));
    } else if(this->objType == ILObject::TypeInput) {
        vector<ILInput*>* listILInput = parent->getListOfILInput();
        return findNextILInput(listILInput, static_cast<ILInput*>(t));
    } else if(this->objType == ILObject::TypeOutput) {
        vector<ILOutput*>* listILOutput = parent->getListOfILOutput();
        return findNextILOutput(listILOutput, static_cast<ILOutput*>(t));
    } else if(this->objType == ILObject::TypeBranch || this->objType == ILObject::TypeCalculate) {
        vector<ILScheduleNode*>* listILScheduleNode = parent->getListOfILScheduleNode();
        return findNextILScheduleNode(listILScheduleNode, static_cast<ILScheduleNode*>(t));
    } else if(this->objType == ILObject::TypeLocalVariable) {
        vector<ILLocalVariable*>* listILLocalVariable = parent->getListOfILLocalVariable();
        return findNextILLocalVariable(listILLocalVariable, static_cast<ILLocalVariable*>(t));
    } else if(this->objType == ILObject::TypeTypeDefine) {
        vector<ILTypeDefine*>* listILTypeDefine = parent->getListOfILTypeDefine();
        return findNextILTypeDefine(listILTypeDefine, static_cast<ILTypeDefine*>(t));
    } else if(this->objType == ILObject::TypeEnumDefine) {
        vector<ILEnumDefine*>* listILEnum = parent->getListOfILEnumDefine();
        return findNextILEnum(listILEnum, static_cast<ILEnumDefine*>(t));
    } else if(this->objType == ILObject::TypeEnumValue) {
        vector<ILEnumValue*>* listILEnumValue = parent->getListOfILEnumValue();
        return findNextILEnumValue(listILEnumValue, static_cast<ILEnumValue*>(t));
    }
    return nullptr;
}

ILObject* ILObject::getPreviousSameILObjectA() const
{
    ILObject* t = const_cast<ILObject*>(this);
    if(this->objType == ILObject::TypeHeadFile) {
        vector<ILHeadFile*>* listILHeadFile = parent->getListOfILHeadFile();
        return findPreviousILHeadFile(listILHeadFile, static_cast<ILHeadFile*>(t));
    } else if(this->objType == ILObject::TypeMacro) {
        vector<ILMacro*>* listILMacro = parent->getListOfILMacro();
        return findPreviousILMacro(listILMacro, static_cast<ILMacro*>(t));
    } else if(this->objType == ILObject::TypeStructDefine) {
        vector<ILStructDefine*>* listILStructDefine = parent->getListOfILStructDefine();
        return findPreviousILStructDefine(listILStructDefine, static_cast<ILStructDefine*>(t));
    } else if(this->objType == ILObject::TypeGlobalVariable) {
        vector<ILGlobalVariable*>* listILGlobalVariable = parent->getListOfILGlobalVariable();
        return findPreviousILGlobalVariable(listILGlobalVariable, static_cast<ILGlobalVariable*>(t));
    } else if(this->objType == ILObject::TypeUtilityFunction) {
        vector<ILUtilityFunction*>* listILUtilityFunction = parent->getListOfILUtilityFunction();
        return findPreviousILUtilityFunction(listILUtilityFunction, static_cast<ILUtilityFunction*>(t));
    } else if(this->objType == ILObject::TypeFunction) {
        vector<ILFunction*>* listILFunction = parent->getListOfILFunction();
        return findPreviousILFunction(listILFunction, static_cast<ILFunction*>(t));
    } else if(this->objType == ILObject::TypeRef) {
        vector<ILRef*>* listILRef = parent->getListOfILRef();
        return findPreviousILRef(listILRef, static_cast<ILRef*>(t));
    } else if(this->objType == ILObject::TypeMember) {
        vector<ILMember*>* listILMember = parent->getListOfILMember();
        return findPreviousILMember(listILMember, static_cast<ILMember*>(t));
    } else if(this->objType == ILObject::TypeInput) {
        vector<ILInput*>* listILInput = parent->getListOfILInput();
        return findPreviousILInput(listILInput, static_cast<ILInput*>(t));
    } else if(this->objType == ILObject::TypeOutput) {
        vector<ILOutput*>* listILOutput = parent->getListOfILOutput();
        return findPreviousILOutput(listILOutput, static_cast<ILOutput*>(t));
    } else if(this->objType == ILObject::TypeBranch || this->objType == ILObject::TypeCalculate) {
        vector<ILScheduleNode*>* listILScheduleNode = parent->getListOfILScheduleNode();
        return findPreviousILScheduleNode(listILScheduleNode, static_cast<ILScheduleNode*>(t));
    } else if(this->objType == ILObject::TypeLocalVariable) {
        vector<ILLocalVariable*>* listILLocalVariable = parent->getListOfILLocalVariable();
        return findPreviousILLocalVariable(listILLocalVariable, static_cast<ILLocalVariable*>(t));
    } else if(this->objType == ILObject::TypeTypeDefine) {
        vector<ILTypeDefine*>* listILTypeDefine = parent->getListOfILTypeDefine();
        return findPreviousILTypeDefine(listILTypeDefine, static_cast<ILTypeDefine*>(t));
    } else if(this->objType == ILObject::TypeEnumDefine) {
        vector<ILEnumDefine*>* listILEnum = parent->getListOfILEnumDefine();
        return findPreviousILEnumDefine(listILEnum, static_cast<ILEnumDefine*>(t));
    } else if(this->objType == ILObject::TypeEnumValue) {
        vector<ILEnumValue*>* listILEnumValue = parent->getListOfILEnumValue();
        return findPreviousILEnumValue(listILEnumValue, static_cast<ILEnumValue*>(t));
    }
    return nullptr;
}


ILHeadFile* ILObject::findPreviousILHeadFile(const std::vector<ILHeadFile*>* list, ILHeadFile* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILMacro* ILObject::findPreviousILMacro(const std::vector<ILMacro*>* list, ILMacro* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILStructDefine* ILObject::findPreviousILStructDefine(const std::vector<ILStructDefine*>* list, 
    ILStructDefine* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILGlobalVariable* ILObject::findPreviousILGlobalVariable(const std::vector<ILGlobalVariable*>* list, 
    ILGlobalVariable* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILUtilityFunction* ILObject::findPreviousILUtilityFunction(const std::vector<ILUtilityFunction*>* list, 
    ILUtilityFunction* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILFunction* ILObject::findPreviousILFunction(const std::vector<ILFunction*>* list, ILFunction* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILRef* ILObject::findPreviousILRef(const std::vector<ILRef*>* list, ILRef* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILMember* ILObject::findPreviousILMember(const std::vector<ILMember*>* list, ILMember* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILInput* ILObject::findPreviousILInput(const std::vector<ILInput*>* list, ILInput* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILOutput* ILObject::findPreviousILOutput(const std::vector<ILOutput*>* list, ILOutput* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILScheduleNode* ILObject::findPreviousILScheduleNode(const std::vector<ILScheduleNode*>* list, 
    ILScheduleNode* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILLocalVariable* ILObject::findPreviousILLocalVariable(const std::vector<ILLocalVariable*>* list, 
    ILLocalVariable* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILTypeDefine* ILObject::findPreviousILTypeDefine(const std::vector<ILTypeDefine*>* list, ILTypeDefine* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILEnumDefine* ILObject::findPreviousILEnumDefine(const std::vector<ILEnumDefine*>* list, ILEnumDefine* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILEnumValue* ILObject::findPreviousILEnumValue(const std::vector<ILEnumValue*>* list, ILEnumValue* obj) const
{
    if(!list)
        return nullptr;
    auto iter = std::find(list->begin(),list->end(), obj);
	if(iter == list->end() || iter == list->begin()){
		return nullptr;
    }
	return *(--iter);
}

ILObject* ILObject::getPreviousSameILObject() const
{
    for(int i = 0; this->parent && i < ILObject::objTypeParentToChild.size(); i++) {
        if(!(parent->objType == ILObject::objTypeParentToChild[i].first &&
             this->objType == ILObject::objTypeParentToChild[i].second)) {
            continue;
        }
        return getPreviousSameILObjectA();
    }
	return nullptr;
}




ILObject* ILObject::getNextSameILObject() const
{
    for(int i = 0; this->parent && i < ILObject::objTypeParentToChild.size(); i++) {
        if(!(parent->objType == ILObject::objTypeParentToChild[i].first &&
             this->objType == ILObject::objTypeParentToChild[i].second)) {
            continue;
        }
        return getNextSameILObjectA();
    }
	return nullptr;
}


std::vector<void*> ILObject::getDomain() const
{
    vector<void*> ret;
    if(this->objType ==ILObject::TypeGlobalVariable)
    {
        ILGlobalVariable* iLGlobalVariable = static_cast<ILGlobalVariable*>(const_cast<ILObject*>(this));
        if(!iLGlobalVariable->isStatic)
            return ret;
    }
    
    if(this->parent)
        ret = this->parent->getDomain();

    if(this->objType == ILObject::TypeFile || this->objType == ILObject::TypeFunction || this->objType == ILObject::TypeBranch)
    {
        ret.push_back(const_cast<ILObject*>(this));
    }

    return ret;
}
