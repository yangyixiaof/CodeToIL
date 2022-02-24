#pragma once
#include <string>
#include <vector>

#include "Statement.h"
#include "Token.h"

class ILObject;
class ILStructDefine;
class ILMember;
class StmtLocalVariable;
class Expression;
class Statement;
class StructType;
class StructMember;

const int MAX_PRECEDENCE = 16;
class ILCCodeParser
{
public:
	int parseCCode(std::string& codeStr, Statement* ret);

	//增加扩展的数据类型，包含出现在代码中的typedefine，#define所重定义的类型和所有结构体定义(结构体成员变量只支持指针或非指针)
	//格式为
	//"DataType"
	//"struct TestDataType{DataType MemberName;DataType *MemberNames;DataType *MemberName[6];}"
	int setExtendDataType(std::vector<std::string> types) const;

public:
	enum {
		ErrorCommentTailNotFound = 10001,
		ErrorUnrecognizedToken,
		ErrorExponentHasNoDigits,
		ErrorInvalidDigitInOctalConstant,
		ErrorInvalidNumericalExpression,
		ErrorStringMissingTerminatingCharacter,
		ErrorCharMissingTerminatingCharacter,
		ErrorUnrecognizedStatement,
		ErrorUnknownStatementType,
		ErrorMissingStatement,
		ErrorStatementMissingSemicolon,
		ErrorStatementMissingColon,
		ErrorStatementMissingBracketLeft,
		ErrorStatementMissingBracketRight,
		ErrorStatementMissingBraceLeft,
		ErrorStatementMissingBraceRight,
		ErrorStatementMissingWhile,
		ErrorStatementLocalVariableMissingVariable,
		ErrorStatementLocalVariableMissingSubscriptRight,
		ErrorStatementLocalVariableMissingInitialization,
		ErrorStatementVariableInvalidAssignment,
		ErrorStatementForInitializationError,
		ErrorStatementReturnInvalidExpression,
		ErrorExpressionCanNotBeEmpty,
		ErrorExpressionBracketCanNotBeEmpty,
		ErrorExpressionSubscriptCanNotBeEmpty,
		ErrorExpressionRecognitionFailed,
		ErrorExpressionMissingRValue,
		ErrorExpressionMissingLValue,
		ErrorExpressionMissingValueOfSubscript,
		ErrorExpressionInternalError,
		ErrorExpressionUncertain,
		ErrorExpressionCompoundElementIndexCanNotBeEmpty,
		ErrorExpressionCompoundElementIndexInvalid,
		ErrorExpressionLeftBracketUnmatched,
		ErrorExpressionInvalidValueInBracket,
		ErrorExpressionLeftSubscriptUnmatched,
		ErrorExpressionInvalidValueInSubscript,
		ErrorExpressionLeftBraceUnmatched,
		ErrorExpressionInvalidValueInBrace,
		ErrorExpressionInvalidLValue,
		ErrorExpressionInvalidRValue,
		ErrorExpressionCanNotFindOperator,
		ErrorExpressionParseError,
		ErrorIntermediateLanguageDoesNotSupportReferenceType,
		ErrorIntermediateLanguageDoesNotSupportMultiPointer,
		ErrorInvalidArraySize,
		ErrorExtendDataTypeInvalid,
		ErrorExtendDataTypeRedefined,
		ErrorInner,
	};

	static int getOperatorPrecedence(Token::Type type);
	static Expression* createExpression(Token::Type type, std::string expStr, bool isExp);
	static Statement* createStatement(Statement::Type type, Statement* parent);
	static bool isAssignOperator(Token::Type type);

	std::vector<std::string> errorStrList = {
        "ErrorCommentTailNotFound,                                 ",
		"ErrorUnrecognizedToken,                                   ",
		"ErrorExponentHasNoDigits,                                 ",
		"ErrorInvalidDigitInOctalConstant,                         ",
		"ErrorInvalidNumericalExpression,                          ",
		"ErrorStringMissingTerminatingCharacter,                   ",
		"ErrorCharMissingTerminatingCharacter,                     ",
		"ErrorUnrecognizedStatement,                               ",
		"ErrorUnknownStatementType,                                ",
		"ErrorMissingStatement,                                    ",
		"ErrorStatementMissingSemicolon,                           ",
		"ErrorStatementMissingColon,                               ",
		"ErrorStatementMissingBracketLeft,                         ",
		"ErrorStatementMissingBracketRight,                        ",
		"ErrorStatementMissingBraceLeft,                           ",
		"ErrorStatementMissingBraceRight,                          ",
		"ErrorStatementMissingWhile,                               ",
		"ErrorStatementLocalVariableMissingVariable,               ",
		"ErrorStatementLocalVariableMissingSubscriptRight,         ",
		"ErrorStatementLocalVariableMissingInitialization,         ",
		"ErrorStatementVariableInvalidAssignment,                  ",
		"ErrorStatementForInitializationError,                     ",
		"ErrorStatementReturnInvalidExpression,                    ",
		"ErrorExpressionCanNotBeEmpty,                             ",
		"ErrorExpressionBracketCanNotBeEmpty,                      ",
		"ErrorExpressionSubscriptCanNotBeEmpty,                    ",
		"ErrorExpressionRecognitionFailed,                         ",
		"ErrorExpressionMissingRValue,                             ",
		"ErrorExpressionMissingLValue,                             ",
		"ErrorExpressionMissingValueOfSubscript,                   ",
		"ErrorExpressionInternalError,                             ",
		"ErrorExpressionUncertain,                                 ",
		"ErrorExpressionCompoundElementIndexCanNotBeEmpty,         ",
		"ErrorExpressionCompoundElementIndexInvalid,               ",
		"ErrorExpressionLeftBracketUnmatched,                      ",
		"ErrorExpressionInvalidValueInBracket,                     ",
		"ErrorExpressionLeftSubscriptUnmatched,                    ",
		"ErrorExpressionInvalidValueInSubscript,                   ",
		"ErrorExpressionLeftBraceUnmatched,                        ",
		"ErrorExpressionInvalidValueInBrace,                       ",
		"ErrorExpressionInvalidLValue,                             ",
		"ErrorExpressionInvalidRValue,                             ",
		"ErrorExpressionCanNotFindOperator,                        ",
		"ErrorExpressionParseError,                                ",
		"ErrorIntermediateLanguageDoesNotSupportReferenceType,     ",
		"ErrorIntermediateLanguageDoesNotSupportMultiPointer,      ",
		"ErrorInvalidArraySize,                                    ",
		"ErrorExtendDataTypeInvalid,                               ",
		"ErrorExtendDataTypeRedefined,                             ",
		"ErrorInner,                                               ",
	};

    
	std::string getErrorStr(int errorCode) const;
	std::string getErrorStr() const;
private:
	int errorCode = 0;
	void setCurrentError(int errorCode);

    void setExtendDataTypeA(const std::string &typeStr, std::vector<std::string> &tokens, std::vector<bool> &tokenIsWord) const;
    int setExtendDataTypeB(const std::vector<std::string> &tokens, const std::vector<bool> &tokenIsWord) const;
    int setExtendDataTypeC(const std::vector<std::string> &tokens, const std::vector<bool> &tokenIsWord) const;
    int setExtendDataTypeCA(const std::vector<std::string> &tokens, const std::vector<bool> &tokenIsWord, int &tokenIndex, ILMember* newStructMember) const;


    static bool isStructType(std::string name);
    static bool isEnumType(std::string name);

	//将代码解析为Token
	int laxAnalyze(std::string& code);

	//去除\r, 去除注释, 替换连续的空格为单空格(有助于识别带空格的数据类型)
	//返回-1 表示缺少批量注释尾
	int laxPreprocess(std::string& code) const;


	//返回错误码, 处理返回1，没处理返回0
    static std::vector<std::string> numberSuffix;
	int laxNumber(const std::string& code, int& charIndex);
	int laxNumberA(const std::string& code, int& i, int charIndexOri, int codeLength, int& mumberType) const;
	int laxNumberB(const std::string& code, int& i, int charIndexOri, int codeLength, int& mumberType) const;
	int laxNumberC(const std::string& code, int& i, int charIndexOri, int codeLength, int& mumberType) const;
	int laxNumberD(const std::string& code, int& i, int charIndexOri, int codeLength, int& mumberType) const;
	int laxNumberE(const std::string& code, int& i, int charIndexOri, int codeLength, int& mumberType) const;
    int laxNumberSkipNum(const std::string& code, int i) const;
	int laxWord(const std::string& code, int& charIndex);
	int laxString(const std::string& code, int& charIndex);
	int laxChar(const std::string& code, int& charIndex);
	int laxBranch(const std::string& code, int& charIndex);
	int laxMonocular(const std::string& code, int& charIndex);
	int laxBinocular(const std::string& code, int& charIndex);
	int laxBinocularA(const std::string& code, int& charIndex);
	int laxBinocularB(const std::string& code, int& charIndex);
	int laxBinocularC(const std::string& code, int& charIndex);
	int laxBinocularD(const std::string& code, int& charIndex);
	int laxTernary(const std::string& code, int& charIndex);
	int laxBracket(const std::string& code, int& charIndex);
	int laxOther(const std::string& code, int& charIndex);

	//尝试获取一个单词
	int tryWord(const std::string& code, int& charIndex) const;

	//分析低级的TokenList，分析得出Word类型的Token具体是DataType, Function, Variable 还是 Member
	//再分析中级的TokenList，分析得出TypeConvert
	void laxProcess();

	void laxProcessDataType();		//将特定word的组合识别为数据类型
	void laxProcessDataTypeA(const std::vector<std::vector<std::string>> &typeSplits, int i, int j, bool &loopBreak);
	//void laxProcessTypeConvert();	//识别数据类型转换，识别过程必须在DataType之后进行
	void laxProcessFunction();		//识别函数
	void laxProcessElseIf();		//识别else if组合为elseif
	void laxProcessMember();		//识别成员变量，识别过程必须在Function之后进行
	void laxProcessVariable();		//剩余未处理的word都识别为变量,识别过程必须在DataType，TypeConvert，Function，Member之后进行
	void laxProcessAdd();			//识别特定的正号使用情况为加法
	void laxProcessSub();			//识别特定的负号使用情况为减法
	void laxProcessIndirection();	//识别特定的*号为取值符
	void laxProcessAddressOf();		//识别特定的&号为取地址符

	//将分析到的词法Token解析成语法树
	int syntaxParse();

	int syntaxStatement(int& tokenIndex, Statement* parent);
	//识别各种语句，语句内部可能包含子语句，下面这些函数可以递归调用
	//返回0表示未识别出该语句，返回1表示识别出该语句，返回负数表示识别过程出错
	int syntaxStmtIf(int& tokenIndex, Statement* parent);
	int syntaxStmtElse(int& tokenIndex, Statement* parent);
	int syntaxStmtElseIf(int& tokenIndex, Statement* parent);
	int syntaxStmtWhile(int& tokenIndex, Statement* parent);
	int syntaxStmtDoWhile(int& tokenIndex, Statement* parent);
	int syntaxStmtFor(int& tokenIndex, Statement* parent);
	int syntaxStmtForA(int& tokenIndex, Statement* stmt, int tokenListSize);
	int syntaxStmtForB(int& tokenIndex, Statement* stmt, int tokenListSize);
	int syntaxStmtContinue(int& tokenIndex, Statement* parent);
	int syntaxStmtSwitch(int& tokenIndex, Statement* parent);
	int syntaxStmtCase(int& tokenIndex, Statement* parent);
	int syntaxStmtDefault(int& tokenIndex, Statement* parent);
	int syntaxStmtBreak(int& tokenIndex, Statement* parent);
	int syntaxStmtReturn(int& tokenIndex, Statement* parent);
	int syntaxStmtBraceStatement(int& tokenIndex, Statement* parent);
	int syntaxStmtLocalVariable(int& tokenIndex, Statement* parent);
	int syntaxStmtLocalVariableA(int& tokenIndex, StmtLocalVariable* stmt, bool& loopContinue, bool& loopBreak);
	int syntaxStmtLocalVariableB(int& tokenIndex, StmtLocalVariable* stmt, bool& loopContinue, bool& loopBreak);
	int syntaxStmtLocalVariableBA(int& tokenIndex, StmtLocalVariable* stmt, bool& loopContinue, bool& loopBreak);
	int syntaxStmtLocalVariableBB(int& tokenIndex, StmtLocalVariable* stmt, bool& loopContinue, bool& loopBreak);
	int syntaxStmtExpressionWithSemicolon(int& tokenIndex, Statement* parent);

	int syntaxExpression(int& tokenIndex, Statement* parent);
    int syntaxExpressionA(Token::Type &curTokenType, int i, std::vector<Expression*> &expStack, bool& loopBreak) const;
    int syntaxExpressionB(int tokenListSize, Token::Type &curTokenType, int &i, std::vector<Expression*> &expStack, bool& loopBreak);
    int syntaxExpressionC(Token::Type curTokenType, int i, std::vector<Expression*> &expStack, bool& loopBreak);

	std::vector<Token> tokenList;
	//一些辅助性的函数
	std::string getMultiTokenValue(int index, int count);
	bool isExpressionToken(Token::Type type) const;
	bool isOperator(Token::Type type) const;
	bool isMonocularOperator(Token::Type type) const;
	bool isBinocularOperator(Token::Type type) const;
	bool isLeftCalculateOperator(Token::Type type) const;
	bool isRightCalculateOperator(Token::Type type) const;


	//将表达式栈尽可能先前合并，参数为最大的优先级，将从右向左合并小于该优先级的操作符
	int mergeStackOperator(std::vector<Expression*>& expStack, int maxPrecedence) const;
    int mergeStackOperatorA(int i, std::vector<Expression*>& expStack, bool isMonocularOperatorT, bool isLeftCalculateOperatorT, bool isRightCalculateOperatorT, bool isBinocularOperatorT, bool &merged, bool& loopBreak) const;
	int mergeStackOperatorLeftOne(std::vector<Expression*>& expStack, int expIndex) const;
    int mergeStackOperatorLeftOneA(std::vector<Expression*>& expStack, int i) const;
    int mergeStackOperatorLeftOneB(std::vector<Expression*>& expStack, int i) const;
    int mergeStackOperatorLeftOneC(std::vector<Expression*>& expStack, int i) const;
    int mergeStackOperatorLeftOneD(std::vector<Expression*>& expStack, int i) const;
	int mergeStackOperatorRightOne(std::vector<Expression*>& expStack, int expIndex) const;
    int mergeStackOperatorRightOneA(std::vector<Expression*>& expStack, int i) const;
    int mergeStackOperatorRightOneB(std::vector<Expression*>& expStack, int i) const;
    int mergeStackOperatorRightOneC(std::vector<Expression*>& expStack, int i) const;
    int mergeStackOperatorRightOneD(std::vector<Expression*>& expStack, int i) const;
    int mergeStackOperatorRightOneE(std::vector<Expression*>& expStack, int i) const;
	int mergeStackOperatorBothSides(std::vector<Expression*>& expStack, int expIndex) const;

	bool expressionStackIsInBrace(const std::vector<Expression*>& expStack, int expIndex) const;


	//用于存储所有代码语句
	Statement* statementRoot = nullptr;
	Statement* statementRet = nullptr;

	bool isEmptyStatement(Statement* stmt) const;

	//用于将statementRoot转化为XML
	int translateStatement(Statement* stmt, Statement* parent, bool isNewStmtExpression = true) const;

	//用于转化各种语句
	int translateStmtIf(Statement* stmt, Statement* parent) const;
	int translateStmtElse(Statement* stmt, Statement* parent) const;
	int translateStmtElseIf(Statement* stmt, Statement* parent) const;
	int translateStmtWhile(Statement* stmt, Statement* parent) const;
	int translateStmtDoWhile(Statement* stmt, Statement* parent) const;
	int translateStmtFor(Statement* stmt, Statement* parent) const;
	int translateStmtContinue(Statement* parent) const;
	int translateStmtSwitch(Statement* stmt, Statement* parent) const;
	int translateStmtCase(Statement* stmt, Statement* parent) const;
	int translateStmtDefault(Statement* parent) const;
	int translateStmtBreak(Statement* parent) const;
	int translateStmtReturn(Statement* stmt, Statement* parent) const;
	int translateStmtBraceStatement(Statement* stmt, Statement* parent) const;
	int translateStmtLocalVariable(Statement* stmt, Statement* parent) const;
	int translateStmtLocalVariableA(const Statement* stmt, int &expIndex, Expression* &valueExp, std::string &valueStr, int &isPointer, std::string &variableName, std::vector<int> &arraySize, bool &handleSingleVarDec) const;
	int translateStmtLocalVariableB(Expression* &valueExp, const std::string &dataTypeStr, const std::string &valueStr, int isPointer, const std::string &variableName, const std::vector<int> &arraySize, Statement* parent, std::string arraySizeStr) const;
	int translateStmtVariableInitValue(const Expression* varNameExp, std::string dataType, std::vector<int> arraySize, Expression* valueExp, int childExpIndex, Statement* parent) const;
	int translateStmtVariableInitValueA(const Expression* varNameExp, std::string dataType, std::vector<int> arraySize, Expression* valueExp, int childExpIndex, Statement* parent) const;
	int translateStmtVariableInitValueB(const Expression* varNameExp, Expression* valueExp, int childExpIndex, Statement* parent, const ILStructDefine* structType) const;
	int translateStmtVariableInitValueC(const Expression* varNameExp, Expression* valueExp, int childExpIndex, Statement* parent) const;
	int translateStmtExpressionWithSemicolon(Statement* stmt, Statement* parent, bool isNewStmtExpression = true) const;
	
	int translateExpression(const Expression* exp, Statement* parent, bool isNewStmtExpression = true) const;

	Expression* getExtendSubscriptExp(const Expression* exp, int subscriptId) const;
	Expression* getExtendMemberExp(const Expression* exp, std::string memberName) const;
    
    void releaseStatement(Statement* stmt) const;
    void releaseExpression(Expression* exp) const;
    int releaseExpressionAndReturn(Expression* exp, int retCode) const;
    int releaseILObjectAndReturn(ILObject* obj, int retCode) const;
    int releaseStatementAndReturn(Statement* stmt, int retCode) const;
};
