#include "ILSaver.h"

#include "ILHeadFile.h"
#include "ILMacro.h"
#include "ILTypeDefine.h"
#include "ILUtilityFunction.h"
#include "ILStructDefine.h"
#include "ILGlobalVariable.h"
#include "ILFunction.h"
#include "ILInput.h"
#include "ILOutput.h"
#include "ILRef.h"
#include "ILBranch.h"
#include "ILCalculate.h"
#include "ILConfig.h"
#include "ILEnumDefine.h"
#include "ILEnumValue.h"
#include "ILMember.h"
#include "ILLocalVariable.h"
#include "ILSchedule.h"
#include "StmtBatchCalculation.h"
#include "StmtAssemble.h"
#include "StmtCase.h"
#include "StmtDoWhile.h"
#include "StmtElse.h"
#include "StmtElseIf.h"
#include "StmtExpression.h"
#include "StmtFor.h"
#include "StmtIf.h"
#include "StmtLocalVariable.h"
#include "StmtReturn.h"
#include "StmtSwitch.h"
#include "StmtWhile.h"
#include "StmtContinue.h"
#include "StmtDefault.h"
#include "StmtBreak.h"

#include "../ILBasic/ILParser.h"
#include "../ILBasic/ILFile.h"
#include "../ILBasic/Expression.h"

using namespace std;

int ILSaver::save(ILParser* iLParser, string outputFilePath) const
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* root = doc.NewElement("Root");
    root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    root->SetAttribute("xsi:noNamespaceSchemaLocation", "http://www.example.org/IRMetadata");

	int res = saveConfig(&(iLParser->config), root, &doc);
	if(res < 0)
		return res;
	for(int i = 0;i < iLParser->files.size();i++)
	{
		res = saveFile(iLParser->files[i], root, &doc);
		if(res < 0)
			return res;
	}
	doc.InsertEndChild(root);
	tinyxml2::XMLPrinter printer;
	doc.Print(&printer);

	//printf("%s\n", printer.CStr());

	doc.SaveFile(outputFilePath.c_str());

    return 1;
}

int ILSaver::saveConfig(const ILConfig* config, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	
    tinyxml2::XMLElement* xml = doc->NewElement("Config");
	root->InsertEndChild(xml);
	if(config->iterationCount != 0)
	{
		tinyxml2::XMLElement* xmlConfig = doc->NewElement("IterationCount");
		xmlConfig->SetAttribute("Value", config->iterationCount);
		xml->InsertEndChild(xmlConfig);
	}
	if(!config->mainInitFunction.empty())
	{
		tinyxml2::XMLElement* xmlConfig = doc->NewElement("MainInitFunction");
		xmlConfig->SetAttribute("Name", config->mainInitFunction.c_str());
		xml->InsertEndChild(xmlConfig);
	}
	if(!config->mainExecFunction.empty())
	{
		tinyxml2::XMLElement* xmlConfig = doc->NewElement("MainExecFunction");
		xmlConfig->SetAttribute("Name", config->mainExecFunction.c_str());
		xml->InsertEndChild(xmlConfig);
	}
	if(!config->mainCompositeState.empty())
	{
		tinyxml2::XMLElement* xmlConfig = doc->NewElement("MainCompositeState");
		xmlConfig->SetAttribute("Name", config->mainCompositeState.c_str());
		xml->InsertEndChild(xmlConfig);
	}

	return 1;
}

void ILSaver::saveExternalAttribute(const ILObject* obj, tinyxml2::XMLElement* root) const
{
    if(obj->objType == ILObject::TypeFunction) {
        if(obj->allAttributeMap.find("Type") != obj->allAttributeMap.end()) {
            root->SetAttribute("Type", obj->allAttributeMap.at("Type").c_str());
        }
        if(obj->allAttributeMap.find("MemStartMacro") != obj->allAttributeMap.end()) {
            root->SetAttribute("MemStartMacro", obj->allAttributeMap.at("MemStartMacro").c_str());
        }
        if(obj->allAttributeMap.find("MemStopMacro") != obj->allAttributeMap.end()) {
            root->SetAttribute("MemStopMacro", obj->allAttributeMap.at("MemStopMacro").c_str());
        }
        if(obj->allAttributeMap.find("MemMapInclusion") != obj->allAttributeMap.end()) {
            root->SetAttribute("MemMapInclusion", obj->allAttributeMap.at("MemMapInclusion").c_str());
        }
    }
}

int ILSaver::saveFile(const ILFile* file, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const
{
	int res = 0;
	tinyxml2::XMLElement* xml = doc->NewElement("File");
	xml->SetAttribute("Name", file->name.c_str());
    if(file->isHeadFile)
        xml->SetAttribute("IsHeadFile", file->isHeadFile);

    if(file->isIndependent)
        xml->SetAttribute("IsIndependent", file->isIndependent);

	for (int i = 0; res >= 0 && i < file->headFiles.size(); i++) {
		res = saveHeadFile(file->headFiles[i], xml, doc);
	}
    for (int i = 0; res >= 0 && i < file->enumDefines.size(); i++) {
		res = saveEnumDefine(file->enumDefines[i], xml, doc);
	}
	for (int i = 0; res >= 0 && i < file->structDefines.size(); i++) {
		res = saveStructDefine(file->structDefines[i], xml, doc);
	}
	for (int i = 0; res >= 0 && i < file->macros.size(); i++) {
		res = saveMacro(file->macros[i], xml, doc);
	}
	for (int i = 0; res >= 0 && i < file->typeDefines.size(); i++) {
		res = saveTypeDefine(file->typeDefines[i], xml, doc);
	}
	for (int i = 0; res >= 0 && i < file->globalVariables.size(); i++) {
		res = saveGlobalVariable(file->globalVariables[i], xml, doc);
	}
	for (int i = 0; res >= 0 && i < file->utilityFunctions.size(); i++) {
		res = saveUtilityFunction(file->utilityFunctions[i], xml, doc);
	}
	for (int i = 0; res >= 0 && i < file->functions.size(); i++) {
		res = saveFunction(file->functions[i], xml, doc);
	}
    if(res < 0)
        return res;
    
    saveExternalAttribute(file, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveHeadFile(const ILHeadFile* headFile, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("HeadFile");
	xml->SetAttribute("Name", headFile->name.c_str());
	for (int i = 0; i < headFile->refs.size(); i++)
	{
		int res = saveRef(headFile->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
    
    saveExternalAttribute(headFile, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveMacro(const ILMacro* macro, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Macro");
	xml->SetAttribute("Name", macro->name.c_str());
	xml->SetAttribute("Value", macro->value.c_str());
	for (int i = 0; i < macro->refs.size(); i++)
	{
		int res = saveRef(macro->refs[i], xml, doc);
		if(res < 0)
			return res;
	}

    saveExternalAttribute(macro, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveTypeDefine(const ILTypeDefine* typeDefine, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("TypeDefine");
	xml->SetAttribute("Name", typeDefine->name.c_str());
	xml->SetAttribute("Value", typeDefine->value.c_str());
	for (int i = 0; i < typeDefine->refs.size(); i++)
	{
		int res = saveRef(typeDefine->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
    
    saveExternalAttribute(typeDefine, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveUtilityFunction(const ILUtilityFunction* utilityFunction, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("UtilityFunction");
	xml->SetAttribute("Name", utilityFunction->name.c_str());
	for (int i = 0; i < utilityFunction->refs.size(); i++)
	{
		int res = saveRef(utilityFunction->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
    
    saveExternalAttribute(utilityFunction, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStructDefine(const ILStructDefine* structDefine, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	int res;
	tinyxml2::XMLElement* xml = doc->NewElement("Struct");
	xml->SetAttribute("Name", structDefine->name.c_str());
	for (int i = 0; i < structDefine->refs.size(); i++)
	{
		res = saveRef(structDefine->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
	for (int i = 0; i < structDefine->members.size(); i++)
	{
		res = saveMember(structDefine->members[i], xml, doc);
		if(res < 0)
			return res;
	}
    
    saveExternalAttribute(structDefine, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveEnumDefine(const ILEnumDefine* enumDefine, tinyxml2::XMLElement* root,
    tinyxml2::XMLDocument* doc) const
{
	int res;
	tinyxml2::XMLElement* xml = doc->NewElement("Enum");
	xml->SetAttribute("Name", enumDefine->name.c_str());
	for (int i = 0; i < enumDefine->refs.size(); i++)
	{
		res = saveRef(enumDefine->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
	for (int i = 0; i < enumDefine->enumValues.size(); i++)
	{
		res = saveEnumValue(enumDefine->enumValues[i], xml, doc);
		if(res < 0)
			return res;
	}

    saveExternalAttribute(enumDefine, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveGlobalVariable(const ILGlobalVariable* globalVariable, tinyxml2::XMLElement* root, 
                                tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("GlobalVariable");
	xml->SetAttribute("Name", globalVariable->name.c_str());
	xml->SetAttribute("Type", globalVariable->type.c_str());
	if(globalVariable->isAddress > 0)
		xml->SetAttribute("IsAddress", globalVariable->isAddress);
	if(globalVariable->isStatic)
		xml->SetAttribute("IsStatic", globalVariable->isStatic);
    if(globalVariable->align > 0)
		xml->SetAttribute("Align", globalVariable->align);
    if(globalVariable->isVolatile)
		xml->SetAttribute("IsVolatile", globalVariable->isVolatile);
    if(globalVariable->isConst)
		xml->SetAttribute("IsConst", globalVariable->isConst);

	if(globalVariable->arraySize.size() > 0)
	{
		string arraySizeStr;
		for(int i = 0;i < globalVariable->arraySize.size(); i++)
		{
			if(i != 0)
				arraySizeStr += ",";
			arraySizeStr += to_string(globalVariable->arraySize[i]);
		}
		xml->SetAttribute("ArraySize", arraySizeStr.c_str());
	}
	if(globalVariable->defaultValue)
		xml->SetAttribute("DefaultValue", globalVariable->defaultValue->expressionStr.c_str());

	for (int i = 0; i < globalVariable->refs.size(); i++)
	{
		int res = saveRef(globalVariable->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
    
    saveExternalAttribute(globalVariable, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveFunction(const ILFunction* function, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	int res;
	tinyxml2::XMLElement* xml = doc->NewElement("Function");
	xml->SetAttribute("Name", function->name.c_str());
	string typeStr = ILFunction::functionTypeToTypeStrMap[function->type];

    if(!typeStr.empty())
	    xml->SetAttribute("Type", typeStr.c_str());
    
	if(function->isStatic)
		xml->SetAttribute("IsStatic", function->isStatic);
	if(!function->returnType.empty())
		xml->SetAttribute("ReturnType", function->returnType.c_str());

	for (int i = 0; i < function->refs.size(); i++)
	{
		res = saveRef(function->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
	for (int i = 0; i < function->inputs.size(); i++)
	{
		res = saveInput(function->inputs[i], xml, doc);
		if(res < 0)
			return res;
	}
	for (int i = 0; i < function->outputs.size(); i++)
	{
		res = saveOutput(function->outputs[i], xml, doc);
		if(res < 0)
			return res;
	}
	
	if(function->schedule)
	{
		res = saveSchedule(function->schedule, xml, doc);
		if(res < 0)
			return res;
	}

    saveExternalAttribute(function, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveInput(const ILInput* input, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Input");
	xml->SetAttribute("Name", input->name.c_str());
	xml->SetAttribute("Type", input->type.c_str());
	if(input->isAddress > 0)
		xml->SetAttribute("IsAddress", input->isAddress);
	if(input->arraySize.size() > 0)
	{
		string arraySizeStr;
		for(int i = 0;i < input->arraySize.size(); i++)
		{
			if(i != 0)
				arraySizeStr += ",";
			arraySizeStr += to_string(input->arraySize[i]);
		}
		xml->SetAttribute("ArraySize", arraySizeStr.c_str());
	}
	if(input->defaultValue)
		xml->SetAttribute("DefaultValue", input->defaultValue->expressionStr.c_str());

	if(!input->sourceStr.empty())
		xml->SetAttribute("Source", input->sourceStr.c_str());
    
    if(input->isConst)
		xml->SetAttribute("IsConst", input->isConst);

	for (int i = 0; i < input->refs.size(); i++)
	{
		int res = saveRef(input->refs[i], xml, doc);
		if(res < 0)
			return res;
	}

    saveExternalAttribute(input, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveOutput(const ILOutput* output, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Output");
	xml->SetAttribute("Name", output->name.c_str());
	xml->SetAttribute("Type", output->type.c_str());
	if(output->isAddress > 0)
		xml->SetAttribute("IsAddress", output->isAddress);
	if(output->arraySize.size() > 0)
	{
		string arraySizeStr;
		for(int i = 0;i < output->arraySize.size(); i++)
		{
			if(i != 0)
				arraySizeStr += ",";
			arraySizeStr += to_string(output->arraySize[i]);
		}
		xml->SetAttribute("ArraySize", arraySizeStr.c_str());
	}
	if(output->defaultValue)
		xml->SetAttribute("DefaultValue", output->defaultValue->expressionStr.c_str());

	if(!output->sourceStr.empty())
		xml->SetAttribute("Source", output->sourceStr.c_str());
    
    if(output->isConst)
		xml->SetAttribute("IsConst", output->isConst);

	for (int i = 0; i < output->refs.size(); i++)
	{
		int res = saveRef(output->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
    
    saveExternalAttribute(output, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveRef(const ILRef* ref, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const
{
	
    tinyxml2::XMLElement* xml = doc->NewElement("Ref");
	if(!ref->actor.empty())
		xml->SetAttribute("Actor", ref->actor.c_str());
	if(!ref->path.empty())
		xml->SetAttribute("Path", ref->path.c_str());
	
    saveExternalAttribute(ref, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveSchedule(const ILSchedule* schedule, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	int res;
	tinyxml2::XMLElement* xml = doc->NewElement("Schedule");
	for (int i = 0; i < schedule->localVariables.size(); i++)
	{
		res = saveLocalVariable(schedule->localVariables[i], xml, doc);
		if(res < 0)
			return res;
	}
	for (int i = 0; i < schedule->scheduleNodes.size(); i++)
	{
		res = saveScheduleNode(schedule->scheduleNodes[i], xml, doc);
		if(res < 0)
			return res;
	}
    
    saveExternalAttribute(schedule, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveMember(const ILMember* member, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	
    tinyxml2::XMLElement* xml = doc->NewElement("Member");
	xml->SetAttribute("Name", member->name.c_str());
	xml->SetAttribute("Type", member->type.c_str());
	if(member->isAddress > 0)
		xml->SetAttribute("IsAddress", member->isAddress);
    if(member->align > 0)
		xml->SetAttribute("Align", member->align);
    if(member->isVolatile)
		xml->SetAttribute("IsVolatile", member->isVolatile);
    if(member->isConst)
		xml->SetAttribute("IsConst", member->isConst);
	if(member->arraySize.size() > 0)
	{
		string arraySizeStr;
		for(int i = 0;i < member->arraySize.size(); i++)
		{
			if(i != 0)
				arraySizeStr += ",";
			arraySizeStr += to_string(member->arraySize[i]);
		}
		xml->SetAttribute("ArraySize", arraySizeStr.c_str());
	}

    for (int i = 0; i < member->refs.size(); i++)
	{
		int res = saveRef(member->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
    saveExternalAttribute(member, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveEnumValue(const ILEnumValue* enumValue, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const
{
    tinyxml2::XMLElement* xml = doc->NewElement("EnumValue");
	xml->SetAttribute("Name", enumValue->name.c_str());

	if(!enumValue->value.empty())
		xml->SetAttribute("Value", enumValue->value.c_str());
    
    saveExternalAttribute(enumValue, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveScheduleNode(const ILScheduleNode* scheduleNode, tinyxml2::XMLElement* root, 
                              tinyxml2::XMLDocument* doc) const
{
	int res;
	if(scheduleNode->objType == ILObject::TypeBranch)
	{
		res = saveBranch(static_cast<ILBranch*>(const_cast<ILScheduleNode*>(scheduleNode)), root, doc);
		if(res < 0)
			return res;
	}
	else if(scheduleNode->objType == ILObject::TypeCalculate)
	{
		res = saveCalculate(static_cast<ILCalculate*>(const_cast<ILScheduleNode*>(scheduleNode)), root, doc);
		if(res < 0)
			return res;
	}
	else if(scheduleNode->objType == ILObject::TypeLocalVariable)
	{
		res = saveLocalVariable(static_cast<ILLocalVariable*>(const_cast<ILScheduleNode*>(scheduleNode)), root, doc);
		if(res < 0)
			return res;
	}
	else
	{
		return -ErrorScheduleNodeHasInvalidChildNode;
	}
	return 1;
}

int ILSaver::saveBranch(const ILBranch* branch, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	int res;
	tinyxml2::XMLElement* xml = doc->NewElement("Branch");
	string typeStr;
	if(branch->type == ILBranch::TypeIf)
		typeStr = "If";
	else if(branch->type == ILBranch::TypeElse)
		typeStr = "Else";
	else if(branch->type == ILBranch::TypeElseIf)
		typeStr = "ElseIf";
	else if(branch->type == ILBranch::TypeFor)
		typeStr = "For";
	else if(branch->type == ILBranch::TypeWhile)
		typeStr = "While";
	xml->SetAttribute("Type", typeStr.c_str());

	if(branch->condition && !branch->condition->expressionStr.empty())
		xml->SetAttribute("Condition", branch->condition->expressionStr.c_str());
	
	for (int i = 0; i < branch->refs.size(); i++)
	{
		res = saveRef(branch->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
	for (int i = 0; i < branch->scheduleNodes.size(); i++)
	{
		res = saveScheduleNode(branch->scheduleNodes[i], xml, doc);
		if(res < 0)
			return res;
	}
	
    saveExternalAttribute(branch, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveCalculate(const ILCalculate* calculate, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	int res;
	tinyxml2::XMLElement* xml = doc->NewElement("Calculate");
	xml->SetAttribute("Name", calculate->name.c_str());
	if(calculate->isOptimized)
		xml->SetAttribute("IsOptimized", calculate->isOptimized);

	for (int i = 0; i < calculate->refs.size(); i++)
	{
		res = saveRef(calculate->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
	for (int i = 0; i < calculate->inputs.size(); i++)
	{
		res = saveInput(calculate->inputs[i], xml, doc);
		if(res < 0)
			return res;
	}
	for (int i = 0; i < calculate->outputs.size(); i++)
	{
		res = saveOutput(calculate->outputs[i], xml, doc);
		if(res < 0)
			return res;
	}
	
	for (int i = 0; i < calculate->statements.childStatements.size(); i++)
	{
		res = saveStatement(calculate->statements.childStatements[i], xml, doc);
		if(res < 0)
			return res;
	}
    
    saveExternalAttribute(calculate, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveLocalVariable(const ILLocalVariable* localVariable, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("LocalVariable");
	xml->SetAttribute("Name", localVariable->name.c_str());
	xml->SetAttribute("Type", localVariable->type.c_str());
	if(localVariable->isAddress > 0)
		xml->SetAttribute("IsAddress", localVariable->isAddress);
    if(localVariable->align > 0)
		xml->SetAttribute("Align", localVariable->align);
    if(localVariable->isVolatile)
		xml->SetAttribute("IsVolatile", localVariable->isVolatile);
    if(localVariable->isConst)
		xml->SetAttribute("IsConst", localVariable->isConst);
	if(localVariable->arraySize.size() > 0)
	{
		string arraySizeStr;
		for(int i = 0;i < localVariable->arraySize.size(); i++)
		{
			if(i != 0)
				arraySizeStr += ",";
			arraySizeStr += to_string(localVariable->arraySize[i]);
		}
		xml->SetAttribute("ArraySize", arraySizeStr.c_str());
	}
	if(localVariable->defaultValue)
		xml->SetAttribute("DefaultValue", localVariable->defaultValue->expressionStr.c_str());

	for (int i = 0; i < localVariable->refs.size(); i++)
	{
		int res = saveRef(localVariable->refs[i], xml, doc);
		if(res < 0)
			return res;
	}
    
    saveExternalAttribute(localVariable, xml);

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStatement(const Statement* statement, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	int res;
	if (statement->type == Statement::If && (res = saveStmtIf(static_cast<StmtIf*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::Else && (res = saveStmtElse(static_cast<StmtElse*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::ElseIf && (res = saveStmtElseIf(static_cast<StmtElseIf*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::While && (res = saveStmtWhile(static_cast<StmtWhile*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::DoWhile && (res = saveStmtDoWhile(static_cast<StmtDoWhile*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::For && (res = saveStmtFor(static_cast<StmtFor*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::Continue && (res = saveStmtContinue(static_cast<StmtContinue*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::Switch && (res = saveStmtSwitch(static_cast<StmtSwitch*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::Case && (res = saveStmtCase(static_cast<StmtCase*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::Default && (res = saveStmtDefault(static_cast<StmtDefault*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::Break && (res = saveStmtBreak(static_cast<StmtBreak*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::Return && (res = saveStmtReturn(static_cast<StmtReturn*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::BraceStatement && (res = saveStmtBraceStatement(statement, root, doc)) != 0) {
	}
	else if (statement->type == Statement::LocalVariable && (res = saveStmtLocalVariable(static_cast<StmtLocalVariable*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::ExpressionWithSemicolon && (res = saveStmtExpressionWithSemicolon(static_cast<StmtExpression*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::BatchCalculation && (res = saveStmtBatchCalculation(static_cast<StmtBatchCalculation*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else if (statement->type == Statement::Assemble && (res = saveStmtAssemble(static_cast<StmtAssemble*>(const_cast<Statement*>(statement)), root, doc)) != 0) {
	}
	else {
		return -ErrorInvalidStatementType;
	}
	if (res < 0) return res;
	return 1;
}

int ILSaver::saveStmtIf(const StmtIf* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("If");
	if(stmt->condition && !stmt->condition->expressionStr.empty())
		xml->SetAttribute("Condition", stmt->condition->expressionStr.c_str());
	else
		return -ErrorConditionOfStmtIfCanNotBeEmpty;
	for (int i = 0; i < stmt->statements.size(); i++)
	{
		saveStatement(stmt->statements[i], xml, doc);
	}
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtElse(const StmtElse* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Else");
	for (int i = 0; i < stmt->statements.size(); i++)
	{
		saveStatement(stmt->statements[i], xml, doc);
	}
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtElseIf(const StmtElseIf* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("ElseIf");
	if(stmt->condition && !stmt->condition->expressionStr.empty())
		xml->SetAttribute("Condition", stmt->condition->expressionStr.c_str());
	else
		return -ErrorConditionOfStmtElseIfCanNotBeEmpty;
	for (int i = 0; i < stmt->statements.size(); i++)
	{
		saveStatement(stmt->statements[i], xml, doc);
	}
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtWhile(const StmtWhile* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("While");
	if(stmt->condition && !stmt->condition->expressionStr.empty())
		xml->SetAttribute("Condition", stmt->condition->expressionStr.c_str());
	else
		return -ErrorConditionOfStmtWhileCanNotBeEmpty;
	for (int i = 0; i < stmt->statements.size(); i++)
	{
		saveStatement(stmt->statements[i], xml, doc);
	}
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtDoWhile(const StmtDoWhile* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("DoWhile");
	if(stmt->condition && !stmt->condition->expressionStr.empty())
		xml->SetAttribute("Condition", stmt->condition->expressionStr.c_str());
	else
		return -ErrorConditionOfStmtDoWhileCanNotBeEmpty;
	for (int i = 0; i < stmt->statements.size(); i++)
	{
		saveStatement(stmt->statements[i], xml, doc);
	}
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtFor(const StmtFor* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	int res;
	tinyxml2::XMLElement* xml = doc->NewElement("For");
	if (!isEmptyStatements(stmt->initial))
	{
		tinyxml2::XMLElement* xmlInit = doc->NewElement("Initial");
		for(int i = 0;i < stmt->initial.size();i++)
		{
			res = saveStatement(stmt->initial[i], xmlInit, doc);
			if(res < 0)
				return res;
		}
		xml->InsertEndChild(xmlInit);
	}
	if(stmt->condition && !stmt->condition->expressionStr.empty())
		xml->SetAttribute("Condition", stmt->condition->expressionStr.c_str());
	if (!isEmptyStatements(stmt->iterator))
	{
		tinyxml2::XMLElement* xmlIterator = doc->NewElement("Iterator");
		for(int i = 0;i < stmt->iterator.size();i++)
		{
			res = saveStatement(stmt->iterator[i], xmlIterator, doc);
			if(res < 0)
				return res;
		}
		xml->InsertEndChild(xmlIterator);
	}
	
	for (int i = 0; i < stmt->statements.size(); i++)
	{
		saveStatement(stmt->statements[i], xml, doc);
	}
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtContinue(const StmtContinue* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Continue");
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtSwitch(const StmtSwitch* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Switch");
	if(stmt->condition && !stmt->condition->expressionStr.empty())
		xml->SetAttribute("Condition", stmt->condition->expressionStr.c_str());
	else
		return -ErrorConditionOfStmtSwitchCanNotBeEmpty;
	for (int i = 0; i < stmt->statements.size(); i++)
	{
		saveStatement(stmt->statements[i], xml, doc);
	}
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtCase(const StmtCase* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Case");
	if(stmt->value && !stmt->value->expressionStr.empty())
		xml->SetAttribute("Value", stmt->value->expressionStr.c_str());
	else
		return -ErrorValueOfStmtCaseCanNotBeEmpty;
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtDefault(const StmtDefault* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Default");
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtBreak(const StmtBreak* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Break");
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtReturn(const StmtReturn* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Return");
	if(stmt->expression && !stmt->expression->expressionStr.empty())
		xml->SetAttribute("Expression", stmt->expression->expressionStr.c_str());
	
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtBraceStatement(const Statement* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	return -ErrorThereShouldBeNoStmtBraceStatementNode;
}

int ILSaver::saveStmtLocalVariable(const StmtLocalVariable* stmt, tinyxml2::XMLElement* root, 
    tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("LocalVariable");

	xml->SetAttribute("Name", stmt->name.c_str());
	xml->SetAttribute("Type", stmt->dataType.c_str());
	if(stmt->isAddress > 0)
		xml->SetAttribute("IsAddress", stmt->isAddress);
    if(stmt->align > 0)
		xml->SetAttribute("Align", stmt->align);
    if(stmt->isVolatile)
		xml->SetAttribute("IsVolatile", stmt->isVolatile);
    if(stmt->isConst)
		xml->SetAttribute("IsConst", stmt->isConst);
	if(stmt->arraySize.size() > 0)
	{
		string arraySizeStr;
		for(int i = 0;i < stmt->arraySize.size(); i++)
		{
			if(i != 0)
				arraySizeStr += ",";
			arraySizeStr += to_string(stmt->arraySize[i]);
		}
		xml->SetAttribute("ArraySize", arraySizeStr.c_str());
	}
	if(stmt->defaultValue && !stmt->defaultValue->expressionStr.empty())
		xml->SetAttribute("DefaultValue", stmt->defaultValue->expressionStr.c_str());

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtExpressionWithSemicolon(const StmtExpression* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Expression");
	if(stmt->expression && !stmt->expression->expressionStr.empty())
		xml->SetAttribute("Expression", stmt->expression->expressionStr.c_str());
	else
		return -ErrorExpressionOfStmtExpressionCanNotBeEmpty;
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtBatchCalculation(const StmtBatchCalculation* stmt, tinyxml2::XMLElement* root,
    tinyxml2::XMLDocument* doc) const
{
    tinyxml2::XMLElement* xml = doc->NewElement("BatchCalculation");

    if(StmtBatchCalculation::operationTypeToTypeStrMap.find(stmt->operationType) != StmtBatchCalculation::operationTypeToTypeStrMap.end()) {
        string operationStr = StmtBatchCalculation::operationTypeToTypeStrMap.at(stmt->operationType);
        xml->SetAttribute("Operation", operationStr.c_str());
    } else {
        return -ErrorInvalidOperationOfStatementBatchCalculation;
    }

    if(!stmt->dataType.empty()) {
        xml->SetAttribute("DataType", stmt->dataType.c_str());
    } else {
        return -ErrorInvalidDataTypeOfStatementBatchCalculation;
    }

    if(StmtBatchCalculation::modifierTypeToTypeStrMap.find(stmt->modifierType) != StmtBatchCalculation::modifierTypeToTypeStrMap.end()) {
        if(stmt->modifierType != StmtBatchCalculation::TypeNone){
            string modifierStr = StmtBatchCalculation::modifierTypeToTypeStrMap.at(stmt->modifierType);
            xml->SetAttribute("Modifier", modifierStr.c_str());
        }
    } else {
        return -ErrorInvalidModifierOfStatementBatchCalculation;
    }

    if(StmtBatchCalculation::shapeTypeToTypeStrMap.find(stmt->shapeType) != StmtBatchCalculation::shapeTypeToTypeStrMap.end()) {
        if(stmt->modifierType != StmtBatchCalculation::TypeNormal)
            xml->SetAttribute("ShapeType", StmtBatchCalculation::shapeTypeToTypeStrMap.at(stmt->shapeType).c_str());
    } else {
        return -ErrorInvalidShapeOfStatementBatchCalculation;
    }

    int res;
    for(int i = 0; i < stmt->inputs.size(); i++) {
        res = saveStmtBatchCalculationInput(&(stmt->inputs[i]), xml, doc);
        if (res < 0)
            return res;
    }

    for(int i = 0; i < stmt->outputs.size(); i++) {
        res = saveStmtBatchCalculationOutput(&(stmt->outputs[i]), xml, doc);
        if (res < 0)
            return res;
    }

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtBatchCalculationInput(const StmtBatchCalculation::StmtBatchCalculationInput* input,
    tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const
{
	tinyxml2::XMLElement* xml = doc->NewElement("Input");
	xml->SetAttribute("Name", input->name.c_str());
	xml->SetAttribute("Type", input->type.c_str());
	if(input->isAddress > 0)
		xml->SetAttribute("IsAddress", input->isAddress);
	if(input->arraySize.size() > 0)
	{
		string arraySizeStr;
		for(int i = 0;i < input->arraySize.size(); i++)
		{
			if(i != 0)
				arraySizeStr += ",";
			arraySizeStr += to_string(input->arraySize[i]);
		}
		xml->SetAttribute("ArraySize", arraySizeStr.c_str());
	}
	if(input->defaultValue)
		xml->SetAttribute("DefaultValue", input->defaultValue->expressionStr.c_str());
    
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtBatchCalculationOutput(const StmtBatchCalculation::StmtBatchCalculationOutput* output,
    tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const
{
    tinyxml2::XMLElement* xml = doc->NewElement("Output");
	xml->SetAttribute("Name", output->name.c_str());
	xml->SetAttribute("Type", output->type.c_str());
	if(output->isAddress > 0)
		xml->SetAttribute("IsAddress", output->isAddress);
	if(output->arraySize.size() > 0)
	{
		string arraySizeStr;
		for(int i = 0;i < output->arraySize.size(); i++)
		{
			if(i != 0)
				arraySizeStr += ",";
			arraySizeStr += to_string(output->arraySize[i]);
		}
		xml->SetAttribute("ArraySize", arraySizeStr.c_str());
	}
	if(output->defaultValue)
		xml->SetAttribute("DefaultValue", output->defaultValue->expressionStr.c_str());
    
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtAssemble(const StmtAssemble* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const
{
    tinyxml2::XMLElement* xml = doc->NewElement("Assemble");


    int res;
    for(int i = 0; i < stmt->inputs.size(); i++) {
        res = saveStmtAssembleInput(&(stmt->inputs[i]), xml, doc);
        if (res < 0)
            return res;
    }

    for(int i = 0; i < stmt->outputs.size(); i++) {
        res = saveStmtAssembleOutput(&(stmt->outputs[i]), xml, doc);
        if (res < 0)
            return res;
    }
    
    for (int i = 0; i < stmt->expressions.size(); i++)
    {
        tinyxml2::XMLElement* xmlChild = doc->NewElement("ASM");
        xmlChild->SetAttribute("Expression", stmt->expressions[i].c_str());
	    xml->InsertEndChild(xmlChild);
    }

	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtAssembleInput(const StmtAssemble::StmtAssembleInput* input, tinyxml2::XMLElement* root,
    tinyxml2::XMLDocument* doc) const
{
    tinyxml2::XMLElement* xml = doc->NewElement("Input");
	xml->SetAttribute("Name", input->name.c_str());
	xml->SetAttribute("Type", input->type.c_str());
	if(input->isAddress > 0)
		xml->SetAttribute("IsAddress", input->isAddress);
	if(input->arraySize.size() > 0)
	{
		string arraySizeStr;
		for(int i = 0;i < input->arraySize.size(); i++)
		{
			if(i != 0)
				arraySizeStr += ",";
			arraySizeStr += to_string(input->arraySize[i]);
		}
		xml->SetAttribute("ArraySize", arraySizeStr.c_str());
	}
	if(input->defaultValue)
		xml->SetAttribute("DefaultValue", input->defaultValue->expressionStr.c_str());
    
	root->InsertEndChild(xml);
	return 1;
}

int ILSaver::saveStmtAssembleOutput(const StmtAssemble::StmtAssembleOutput* output,
    tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const
{
    tinyxml2::XMLElement* xml = doc->NewElement("Output");
	xml->SetAttribute("Name", output->name.c_str());
	xml->SetAttribute("Type", output->type.c_str());
	if(output->isAddress > 0)
		xml->SetAttribute("IsAddress", output->isAddress);
	if(output->arraySize.size() > 0)
	{
		string arraySizeStr;
		for(int i = 0;i < output->arraySize.size(); i++)
		{
			if(i != 0)
				arraySizeStr += ",";
			arraySizeStr += to_string(output->arraySize[i]);
		}
		xml->SetAttribute("ArraySize", arraySizeStr.c_str());
	}
	if(output->defaultValue)
		xml->SetAttribute("DefaultValue", output->defaultValue->expressionStr.c_str());
    
	root->InsertEndChild(xml);
	return 1;
}

bool ILSaver::isEmptyStatement(Statement* stmt) const
{
	if (stmt->type == Statement::BraceStatement)
	{
		bool isEmpty = true;
		for (int i = 0; i < stmt->childStatements.size(); i++)
		{
			if (!isEmptyStatement(stmt->childStatements[i]))
			{
				isEmpty = false;
				break;
			}
		}
		return isEmpty;
	}
	else if (stmt->type == Statement::ExpressionWithSemicolon)
	{
		if (stmt->innerExpressions.empty())
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

bool ILSaver::isEmptyStatements(vector<Statement*> stmts) const
{
	for(int i = 0;i < stmts.size();i++)
	{
		if(!isEmptyStatement(stmts[i]))
			return false;
	}
	return true;
}