#include <iostream>
#include <assert.h>
#include "util.h"
#include "../Common/Utility.h"
#include "../Common/FileUtil.h"
#include "../ILBasic/ILParser.h"
#include "../ILBasic/ILExpressionParser.h"
#include "../ILBasic/ILSaver.h"
#include "../ILBasic/ILInput.h"
#include "../ILBasic/ILOutput.h"
#include "../ILBasic/ILFile.h"
#include "../ILBasic/ILStructDefine.h"
#include "../ILBasic/ILTypeDefine.h"
#include "../ILBasic/ILFunction.h"
#include "../ILBasic/ILSchedule.h"
#include "../ILBasic/ILCalculate.h"
#include "../ILBasic/ILGlobalVariable.h"

enum ParamType {
	FuncInput,
	FuncOutput,
};

void PreprocessParameter(std::string parameter, std::string pfx, std::vector<std::string>& container)
{
	int paramLen = parameter.length();
	int pfxLen = pfx.length();// "input:"
	std::string detail = parameter.substr(pfxLen, paramLen - pfxLen);
	container.push_back(detail);
}

void TransParameterToMemoryObject(std::vector<std::string> params, ParamType ptype, ILFunction* iLFunctionInit)
{
	// 给这个函数加一个input，output就是ILOutput
	// iLInputInit->arraySize.push_back(2).push_back(3);[2, 3]形状的数组
	for (std::string param : params) {
		std::vector<std::string> details = stringSplit(param, "=");
		std::string name = details.at(0);
		std::string type_with_size = details.at(1);

		std::vector<std::string> ts_details = stringSplit(type_with_size, ",");

		bool isAddress = false;

		std::string type = ts_details.at(0);
		if (stringEndsWith(type, "*")) {
			type = type.substr(0, type.length() - 1);
			isAddress = true;
		}

		int size = 0;
		if (ts_details.size() > 1) {
			size = atoi(ts_details.at(1).c_str());
			assert(size > 0);
		}
		if (ptype == FuncInput) {
			ILInput* iLInputInit = new ILInput();
			iLInputInit->name = name;
			iLInputInit->type = type;
			iLInputInit->isAddress = isAddress;
			if (isAddress) {
				iLInputInit->arraySize.push_back(size);
			}
			iLFunctionInit->inputs.push_back(iLInputInit);
		}
		else if (ptype == FuncOutput) {
			ILOutput* iLOutputInit = new ILOutput();
			iLOutputInit->name = name;
			iLOutputInit->type = type;
			iLOutputInit->isAddress = isAddress;
			if (isAddress) {
				iLOutputInit->arraySize.push_back(size);
			}
			iLFunctionInit->outputs.push_back(iLOutputInit);
		}
	}
}

int main()
{
	std::string modelName = "euler321_I2B_12B";

	ILParser iLParser;

	iLParser.config.iterationCount = 10;
	iLParser.config.mainExecFunction = modelName + "_step";
	iLParser.config.mainInitFunction = modelName + "_init";
	iLParser.config.mainCompositeState = modelName + "_state";

	ILFile* iLFile = new ILFile();
	iLParser.files.push_back(iLFile);
	iLFile->name = modelName;

	ILStructDefine* mainStateStruct = new ILStructDefine();
	mainStateStruct->name = iLParser.config.mainCompositeState;
	iLFile->structDefines.push_back(mainStateStruct);

	// 添加TypeDefine
	std::string typedefStr = readFile("typeDefine.txt");
	std::vector<std::string> typedefStrLine = stringSplit(typedefStr, "\n");
	typedefStrLine = stringTrimList(typedefStrLine);
	for (int i = 0; i < typedefStrLine.size(); i++)
	{
		std::vector<std::string> typeDef = stringSplit(typedefStrLine[i], "=");
		ILTypeDefine* iLTypeDef = new ILTypeDefine();
		iLTypeDef->name = typeDef[0];
		iLTypeDef->value = typeDef[1];

		ILParser::typeDefineTypeList.push_back(iLTypeDef);
		ILParser::allDataTypeList.push_back(iLTypeDef->value);

	}
	iLFile->typeDefines.insert(iLFile->typeDefines.begin(), ILParser::typeDefineTypeList.begin(), ILParser::typeDefineTypeList.end());

	// 添加StructDefine
	std::string structStr = readFile("struct.txt");
	std::vector<std::string> structStrLine = stringSplit(processStruct(structStr), "\n");
	structStrLine = stringTrimList(structStrLine);
	ILCCodeParser codeParser;
	codeParser.setExtendDataType(structStrLine);

	iLFile->structDefines.insert(iLFile->structDefines.begin(), ILParser::structTypeList.begin(), ILParser::structTypeList.end());

	// 添加GlobalVariable
	std::string globalVariableStr = readFile("globalVariable.txt");
	std::vector<std::string> globalVariableStrLine = stringSplit(globalVariableStr, "\n");
	globalVariableStrLine = stringTrimList(globalVariableStrLine);
	for (int i = 0; i < globalVariableStrLine.size(); i++)
	{
		std::vector<std::string> globalVariable = stringSplit(globalVariableStrLine[i], "=");
		ILGlobalVariable* iLGlobalVariable = new ILGlobalVariable();
		std::string raw_type_with_size = globalVariable[0];
		std::vector<std::string> details = stringSplit(raw_type_with_size, ",");
		std::string raw_type = stringTrim(details.at(0));
		int size = 0;
		if (details.size() > 1) {
			size = atoi(stringTrim(details.at(1)).c_str());
			assert(size > 0);
		}
		bool isAddr = false;
		if (stringEndsWith(raw_type, "*")) {
			raw_type = raw_type.substr(0, raw_type.length() - 1);
			isAddr = true;
		}
		std::string type = stringTrim(raw_type);
		iLGlobalVariable->isAddress = isAddr;
		iLGlobalVariable->type = type;
		iLGlobalVariable->name = globalVariable[1];
		if (size > 0) {
			iLGlobalVariable->arraySize.push_back(size);
		}

		if (globalVariable.size() > 2) {
			std::string ini = globalVariable.at(2);
			if (ini != "") {
				ILExpressionParser expressionParser;
				Expression* expRet = nullptr;
				int res = expressionParser.parseExpression(ini, &expRet);
				if (res < 0)
					return res;
				iLGlobalVariable->defaultValue = expRet;
			}
		}
		iLFile->globalVariables.push_back(iLGlobalVariable);
	}

	{
		// 添加Step函数
		std::string stepFunctionStr = readFile("stepFunction.txt");
		std::vector<std::string> stepFunctionStrLine = stringSplit(stepFunctionStr, "\n");
		stepFunctionStrLine = stringTrimList(stepFunctionStrLine);
		std::string stepFunctionName = iLParser.config.mainExecFunction;
		stepFunctionStr = "";
		for (int i = 0; i < stepFunctionStrLine.size(); i++)
		{
			if (i != 0)
				stepFunctionStr += "\n";
			stepFunctionStr += stepFunctionStrLine[i];
		}

		Statement stepStmt;
		int res = codeParser.parseCCode(stepFunctionStr, &stepStmt);
		if (res < 0)
		{
			return res;
		}

		ILFunction* iLFunctionStep = new ILFunction();
		iLFile->functions.push_back(iLFunctionStep);
		ILInput* iLInputStep = new ILInput();
		iLInputStep->name = "self";
		iLInputStep->type = iLParser.config.mainCompositeState;
		iLInputStep->isAddress = true;
		iLFunctionStep->inputs.push_back(iLInputStep);

		iLFunctionStep->name = stepFunctionName;
		iLFunctionStep->type = ILFunction::FunctionExec;
		iLFunctionStep->returnType = "void";

		ILSchedule* iLScheduleStep = new ILSchedule();
		iLFunctionStep->schedule = iLScheduleStep;

		ILCalculate* iLCalculateStep = new ILCalculate();
		iLCalculateStep->statements = stepStmt;
		iLCalculateStep->name = modelName + "_step";

		iLScheduleStep->scheduleNodes.push_back(iLCalculateStep);
	}

	{
		// 添加Init函数
		std::string initFunctionStr = readFile("initFunction.txt");
		std::vector<std::string> initFunctionStrLine = stringSplit(initFunctionStr, "\n");

		initFunctionStrLine = stringTrimList(initFunctionStrLine);
		std::string initFunctionName = iLParser.config.mainInitFunction;
		initFunctionStr = "";

		for (int i = 0; i < initFunctionStrLine.size(); i++)
		{
			if (i != 0)
				initFunctionStr += "\n";
			initFunctionStr += initFunctionStrLine[i];
		}

		Statement initStmt;
		ILCCodeParser codeParserInit;
		int res = codeParserInit.parseCCode(initFunctionStr, &initStmt);
		if (res < 0)
		{
			return res;
		}

		ILFunction* iLFunctionInit = new ILFunction();
		iLFile->functions.push_back(iLFunctionInit);
		{
			ILInput* iLInputInit = new ILInput();
			iLInputInit->name = "self";
			iLInputInit->type = iLParser.config.mainCompositeState;
			iLInputInit->isAddress = true;
			iLFunctionInit->inputs.push_back(iLInputInit);
		}

		iLFunctionInit->name = initFunctionName;
		iLFunctionInit->type = ILFunction::FunctionInit;
		iLFunctionInit->returnType = "void";

		ILSchedule* iLScheduleInit = new ILSchedule();
		iLFunctionInit->schedule = iLScheduleInit;

		ILCalculate* iLCalculateInit = new ILCalculate();
		iLCalculateInit->statements = initStmt;
		iLCalculateInit->name = modelName + "_init";

		iLScheduleInit->scheduleNodes.push_back(iLCalculateInit);
	}

	std::vector<std::string> files;
	ListFiles("", files);
	if (files.size() > 0) {
		for (std::string file : files) {
			if (stringStartsWith(file, "otherFunction")) {
				// 添加Otherfunction函数
				std::string initFunctionStr = readFile(file);
				std::vector<std::string> initFunctionStrLine = stringSplit(initFunctionStr, "\n");
				initFunctionStrLine = stringTrimList(initFunctionStrLine);

				// 前n行解析，把input，output重要内容抽取出来
				// 函数体不变
				std::vector<std::string> inputs;
				std::vector<std::string> outputs;
				std::string initFunctionName = initFunctionStrLine.at(0);

				int rs = 1;
				for (rs = 1; rs < initFunctionStrLine.size(); rs++)
				{
					std::string parameter = initFunctionStrLine.at(rs);
					if (stringStartsWith(parameter, "input:")) {
						PreprocessParameter(parameter, "input:", inputs);
					}
					else if (stringStartsWith(parameter, "output:")) {
						PreprocessParameter(parameter, "output:", outputs);
					}
					else {
						break;
					}
				}

				initFunctionStr = "";

				// i需要从第n行开始取

				for (int i = rs; i < initFunctionStrLine.size(); i++)
				{
					if (i != rs)
						initFunctionStr += "\n";
					initFunctionStr += initFunctionStrLine[i];
				}

				Statement initStmt;
				ILCCodeParser codeParserInit;
				int res = codeParserInit.parseCCode(initFunctionStr, &initStmt);
				if (res < 0)
				{
					return res;
				}

				ILFunction* iLFunctionInit = new ILFunction();
				iLFile->functions.push_back(iLFunctionInit);

				TransParameterToMemoryObject(inputs, FuncInput, iLFunctionInit);
				TransParameterToMemoryObject(outputs, FuncOutput, iLFunctionInit);

				iLFunctionInit->name = initFunctionName;
				iLFunctionInit->type = ILFunction::FunctionInit;
				iLFunctionInit->returnType = "void";

				ILSchedule* iLScheduleInit = new ILSchedule();
				iLFunctionInit->schedule = iLScheduleInit;

				ILCalculate* iLCalculateInit = new ILCalculate();
				iLCalculateInit->statements = initStmt;
				iLCalculateInit->name = modelName + "_init";

				iLScheduleInit->scheduleNodes.push_back(iLCalculateInit);
			}
		}
	}
	
	// 复制Init函数

    ILSaver iLSaver;
    iLSaver.save(&iLParser, modelName + ".xml");

    iLParser.release();

    return 1;
}
