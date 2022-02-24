#pragma once
#include <map>
#include <string>
#include <vector>
class ILEnumDefine;
class ILEnumValue;
class ILTypeDefine;
class ILLocalVariable;
class ILScheduleNode;
class ILOutput;
class ILInput;
class ILMember;
class ILRef;
class ILFunction;
class ILGlobalVariable;
class ILUtilityFunction;
class ILStructDefine;
class ILMacro;
class ILHeadFile;

class ILObject
{
public:

    ILObject();
    virtual ~ILObject();

    enum ObjType{
		TypeFile = 1,
		TypeHeadFile,
		TypeMacro,
		TypeTypeDefine,
		TypeUtilityFunction,
		TypeEnumDefine,
		TypeStructDefine,
		TypeGlobalVariable,
		TypeFunction,

		TypeBranch,
		TypeCalculate,
		TypeLocalVariable,

		TypeInput,
		TypeOutput,
		TypeRef,
		TypeSchedule,

        TypeEnumValue,
		TypeMember,
        
		TypeState,
        
		TypeConfig,

		TypeUnknown,
	};

	ObjType objType = TypeUnknown;

    ILObject* parent = nullptr;

	virtual void release(){};

	ILObject* getNextSameILObject() const;
	ILObject* getPreviousSameILObject() const;

    std::vector<ILHeadFile*>* getListOfILHeadFile() const;
    std::vector<ILMacro*>* getListOfILMacro() const;
    std::vector<ILStructDefine*>* getListOfILStructDefine() const;
    std::vector<ILGlobalVariable*>* getListOfILGlobalVariable() const;
    std::vector<ILUtilityFunction*>* getListOfILUtilityFunction() const;
    std::vector<ILFunction*>* getListOfILFunction() const;
    std::vector<ILRef*>* getListOfILRef() const;
    std::vector<ILMember*>* getListOfILMember() const;
    std::vector<ILInput*>* getListOfILInput() const;
    std::vector<ILOutput*>* getListOfILOutput() const;
    std::vector<ILScheduleNode*>* getListOfILScheduleNode() const;
    std::vector<ILLocalVariable*>* getListOfILLocalVariable() const;
    std::vector<ILTypeDefine*>* getListOfILTypeDefine() const;
    std::vector<ILEnumDefine*>* getListOfILEnumDefine() const;
    std::vector<ILEnumValue*>* getListOfILEnumValue() const;

    ILHeadFile* findPreviousILHeadFile(const std::vector<ILHeadFile*>* list, ILHeadFile* obj) const;
    ILMacro* findPreviousILMacro(const std::vector<ILMacro*>* list, ILMacro* obj) const;
    ILStructDefine* findPreviousILStructDefine(const std::vector<ILStructDefine*>* list, ILStructDefine* obj) const;
    ILGlobalVariable* findPreviousILGlobalVariable(const std::vector<ILGlobalVariable*>* list, ILGlobalVariable* obj) const;
    ILUtilityFunction* findPreviousILUtilityFunction(const std::vector<ILUtilityFunction*>* list, ILUtilityFunction* obj) const;
    ILFunction* findPreviousILFunction(const std::vector<ILFunction*>* list, ILFunction* obj) const;
    ILRef* findPreviousILRef(const std::vector<ILRef*>* list, ILRef* obj) const;
    ILMember* findPreviousILMember(const std::vector<ILMember*>* list, ILMember* obj) const;
    ILInput* findPreviousILInput(const std::vector<ILInput*>* list, ILInput* obj) const;
    ILOutput* findPreviousILOutput(const std::vector<ILOutput*>* list, ILOutput* obj) const;
    ILScheduleNode* findPreviousILScheduleNode(const std::vector<ILScheduleNode*>* list, ILScheduleNode* obj) const;
    ILLocalVariable* findPreviousILLocalVariable(const std::vector<ILLocalVariable*>* list, ILLocalVariable* obj) const;
    ILTypeDefine* findPreviousILTypeDefine(const std::vector<ILTypeDefine*>* list, ILTypeDefine* obj) const;
    ILEnumDefine* findPreviousILEnumDefine(const std::vector<ILEnumDefine*>* list, ILEnumDefine* obj) const;
    ILEnumValue* findPreviousILEnumValue(const std::vector<ILEnumValue*>* list, ILEnumValue* obj) const;

    ILHeadFile* findNextILHeadFile(const std::vector<ILHeadFile*>* list, ILHeadFile* obj) const;
    ILMacro* findNextILMacro(const std::vector<ILMacro*>* list, ILMacro* obj) const;
    ILStructDefine* findNextILStructDefine(const std::vector<ILStructDefine*>* list, ILStructDefine* obj) const;
    ILGlobalVariable* findNextILGlobalVariable(const std::vector<ILGlobalVariable*>* list, ILGlobalVariable* obj) const;
    ILUtilityFunction* findNextILUtilityFunction(const std::vector<ILUtilityFunction*>* list, ILUtilityFunction* obj) const;
    ILFunction* findNextILFunction(const std::vector<ILFunction*>* list, ILFunction* obj) const;
    ILRef* findNextILRef(const std::vector<ILRef*>* list, ILRef* obj) const;
    ILMember* findNextILMember(const std::vector<ILMember*>* list, ILMember* obj) const;
    ILInput* findNextILInput(const std::vector<ILInput*>* list, ILInput* obj) const;
    ILOutput* findNextILOutput(const std::vector<ILOutput*>* list, ILOutput* obj) const;
    ILScheduleNode* findNextILScheduleNode(const std::vector<ILScheduleNode*>* list, ILScheduleNode* obj) const;
    ILLocalVariable* findNextILLocalVariable(const std::vector<ILLocalVariable*>* list, ILLocalVariable* obj) const;
    ILTypeDefine* findNextILTypeDefine(const std::vector<ILTypeDefine*>* list, ILTypeDefine* obj) const;
    ILEnumDefine* findNextILEnum(const std::vector<ILEnumDefine*>* list, ILEnumDefine* obj) const;
    ILEnumValue* findNextILEnumValue(const std::vector<ILEnumValue*>* list, ILEnumValue* obj) const;


    std::vector<void*> getDomain() const;

    std::map<std::string, std::string> allAttributeMap;

private:
    static std::vector<std::pair<ObjType, ObjType>>objTypeParentToChild;
    ILObject* getNextSameILObjectA() const;
	ILObject* getPreviousSameILObjectA() const;

};

