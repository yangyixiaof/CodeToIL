#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "../Common/tinyxml2.h"


#include "StmtBatchCalculation.h";
#include "StmtAssemble.h";

class StmtExpression;
class StmtReturn;
class StmtBreak;
class StmtDefault;
class StmtCase;
class StmtSwitch;
class StmtContinue;
class StmtFor;
class StmtDoWhile;
class StmtWhile;
class StmtElseIf;
class StmtElse;
class StmtIf;
class Statement;
class ILLocalVariable;
class ILCalculate;
class ILBranch;
class ILScheduleNode;
class ILMember;
class ILEnumValue;
class ILSchedule;
class ILRef;
class ILOutput;
class ILInput;
class ILFunction;
class ILGlobalVariable;
class ILStructDefine;
class ILEnumDefine;
class ILUtilityFunction;
class ILTypeDefine;
class ILMacro;
class ILHeadFile;
class ILFile;
class StmtLocalVariable;
class ILConfig;
class ILParser;

class ILSaver
{
public:
	int save(ILParser* iLParser, std::string outputFilePath) const;

	enum {
		ErrorScheduleNodeHasInvalidChildNode = 30001,
		ErrorThereShouldBeNoStmtBraceStatementNode,
		ErrorInvalidStatementType,
		ErrorConditionOfStmtIfCanNotBeEmpty,
		ErrorConditionOfStmtElseIfCanNotBeEmpty,
		ErrorConditionOfStmtWhileCanNotBeEmpty,
		ErrorConditionOfStmtDoWhileCanNotBeEmpty,
		ErrorConditionOfStmtSwitchCanNotBeEmpty,
		ErrorValueOfStmtCaseCanNotBeEmpty,
		ErrorExpressionOfStmtExpressionCanNotBeEmpty,
		ErrorInvalidOperationOfStatementBatchCalculation,
		ErrorInvalidDataTypeOfStatementBatchCalculation,
		ErrorInvalidModifierOfStatementBatchCalculation,
		ErrorInvalidShapeOfStatementBatchCalculation,
	};

	int saveConfig(const ILConfig* config, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;

    void saveExternalAttribute(const ILObject* obj, tinyxml2::XMLElement* root) const;

	int saveFile(const ILFile* file, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveHeadFile(const ILHeadFile* headFile, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveMacro(const ILMacro* macro, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveTypeDefine(const ILTypeDefine* typeDefine, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveUtilityFunction(const ILUtilityFunction* utilityFunction, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStructDefine(const ILStructDefine* structDefine, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveEnumDefine(const ILEnumDefine* enumDefine, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveGlobalVariable(const ILGlobalVariable* globalVariable, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveFunction(const ILFunction* function, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	
	int saveInput(const ILInput* input, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveOutput(const ILOutput* output, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveRef(const ILRef* ref, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveSchedule(const ILSchedule* schedule, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;

	int saveMember(const ILMember* member, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveEnumValue(const ILEnumValue* enumValue, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	
	int saveScheduleNode(const ILScheduleNode* scheduleNode, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveBranch(const ILBranch* branch, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveCalculate(const ILCalculate* calculate, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveLocalVariable(const ILLocalVariable* localVariable, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	
	int saveStatement(const Statement* statement, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	
	int saveStmtIf(const StmtIf* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtElse(const StmtElse* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtElseIf(const StmtElseIf* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtWhile(const StmtWhile* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtDoWhile(const StmtDoWhile* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtFor(const StmtFor* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtContinue(const StmtContinue* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtSwitch(const StmtSwitch* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtCase(const StmtCase* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtDefault(const StmtDefault* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtBreak(const StmtBreak* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtReturn(const StmtReturn* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtBraceStatement(const Statement* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtLocalVariable(const StmtLocalVariable* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
	int saveStmtExpressionWithSemicolon(const StmtExpression* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;

    int saveStmtBatchCalculation(const StmtBatchCalculation* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
    int saveStmtBatchCalculationInput(const StmtBatchCalculation::StmtBatchCalculationInput* input, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
    int saveStmtBatchCalculationOutput(const StmtBatchCalculation::StmtBatchCalculationOutput* output, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
    int saveStmtAssemble(const StmtAssemble* stmt, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
    int saveStmtAssembleInput(const StmtAssemble::StmtAssembleInput* input, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
    int saveStmtAssembleOutput(const StmtAssemble::StmtAssembleOutput* output, tinyxml2::XMLElement* root, tinyxml2::XMLDocument* doc) const;
    
    
	bool isEmptyStatement(Statement* stmt) const;
	bool isEmptyStatements(std::vector<Statement*> stmts) const;
};

