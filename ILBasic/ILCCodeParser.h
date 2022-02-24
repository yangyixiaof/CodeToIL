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

	//������չ���������ͣ����������ڴ����е�typedefine��#define���ض�������ͺ����нṹ�嶨��(�ṹ���Ա����ֻ֧��ָ����ָ��)
	//��ʽΪ
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

	//���������ΪToken
	int laxAnalyze(std::string& code);

	//ȥ��\r, ȥ��ע��, �滻�����Ŀո�Ϊ���ո�(������ʶ����ո����������)
	//����-1 ��ʾȱ������ע��β
	int laxPreprocess(std::string& code) const;


	//���ش�����, ������1��û������0
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

	//���Ի�ȡһ������
	int tryWord(const std::string& code, int& charIndex) const;

	//�����ͼ���TokenList�������ó�Word���͵�Token������DataType, Function, Variable ���� Member
	//�ٷ����м���TokenList�������ó�TypeConvert
	void laxProcess();

	void laxProcessDataType();		//���ض�word�����ʶ��Ϊ��������
	void laxProcessDataTypeA(const std::vector<std::vector<std::string>> &typeSplits, int i, int j, bool &loopBreak);
	//void laxProcessTypeConvert();	//ʶ����������ת����ʶ����̱�����DataType֮�����
	void laxProcessFunction();		//ʶ����
	void laxProcessElseIf();		//ʶ��else if���Ϊelseif
	void laxProcessMember();		//ʶ���Ա������ʶ����̱�����Function֮�����
	void laxProcessVariable();		//ʣ��δ�����word��ʶ��Ϊ����,ʶ����̱�����DataType��TypeConvert��Function��Member֮�����
	void laxProcessAdd();			//ʶ���ض�������ʹ�����Ϊ�ӷ�
	void laxProcessSub();			//ʶ���ض��ĸ���ʹ�����Ϊ����
	void laxProcessIndirection();	//ʶ���ض���*��Ϊȡֵ��
	void laxProcessAddressOf();		//ʶ���ض���&��Ϊȡ��ַ��

	//���������Ĵʷ�Token�������﷨��
	int syntaxParse();

	int syntaxStatement(int& tokenIndex, Statement* parent);
	//ʶ�������䣬����ڲ����ܰ�������䣬������Щ�������Եݹ����
	//����0��ʾδʶ�������䣬����1��ʾʶ�������䣬���ظ�����ʾʶ����̳���
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
	//һЩ�����Եĺ���
	std::string getMultiTokenValue(int index, int count);
	bool isExpressionToken(Token::Type type) const;
	bool isOperator(Token::Type type) const;
	bool isMonocularOperator(Token::Type type) const;
	bool isBinocularOperator(Token::Type type) const;
	bool isLeftCalculateOperator(Token::Type type) const;
	bool isRightCalculateOperator(Token::Type type) const;


	//�����ʽջ��������ǰ�ϲ�������Ϊ�������ȼ�������������ϲ�С�ڸ����ȼ��Ĳ�����
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


	//���ڴ洢���д������
	Statement* statementRoot = nullptr;
	Statement* statementRet = nullptr;

	bool isEmptyStatement(Statement* stmt) const;

	//���ڽ�statementRootת��ΪXML
	int translateStatement(Statement* stmt, Statement* parent, bool isNewStmtExpression = true) const;

	//����ת���������
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
