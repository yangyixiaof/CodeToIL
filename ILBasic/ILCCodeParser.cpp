#include "ILCCodeParser.h"

#include <algorithm>

#include "ILParser.h"
#include "../Common/Utility.h"

#include "StmtBreak.h"
#include "StmtCase.h"
#include "StmtContinue.h"
#include "StmtDefault.h"
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
#include "../ILBasic/Expression.h"

#include "ILStructDefine.h"
#include "ILMember.h"
#include "ILEnumDefine.h"
#include "ILEnumValue.h"
#include "ILTypeDefine.h"

using namespace std;
namespace
{
    const int CONST_NUM_2  = 2;
    const int CONST_NUM_3  = 3;
    const int CONST_NUM_5  = 5;
    const int CONST_NUM_6  = 6;
    const int CONST_NUM_7  = 7;
}
int ILCCodeParser::parseCCode(string& codeStr, Statement* ret)
{
	statementRet = ret;
	statementRoot = new Statement();
	statementRoot->type = static_cast<Statement::Type>(-1);
	statementRoot->depth = 0;
	statementRoot->parentStatement = nullptr;

	int laxAnalyzeRet = laxAnalyze(codeStr);
	if (laxAnalyzeRet < 0)
	{
	    releaseStatement(statementRoot);
        statementRoot = nullptr;
		return laxAnalyzeRet;
	}
	int syntaxParseRet = syntaxParse();
	if (syntaxParseRet < 0)
	{
	    releaseStatement(statementRoot);
        statementRoot = nullptr;
		return syntaxParseRet;
	}
	
	ret->type = static_cast<Statement::Type>(-1);
	ret->depth = 0;
	ret->parentStatement = nullptr;
    
	int translateStatementRet = translateStatement(statementRoot, ret);
	releaseStatement(statementRoot);
    statementRoot = nullptr;
	if (translateStatementRet < 0)
	{
		return translateStatementRet;
	}

	return 1;
}


bool ILCCodeParser::isStructType(string name)
{
	if (ILParser::findStructType(name))
		return true;
	return false;
}

bool ILCCodeParser::isEnumType(std::string name)
{
    if (ILParser::findEnumType(name))
		return true;
	return false;
}

int ILCCodeParser::laxAnalyze(string& code)
{
	int preprocessRet = laxPreprocess(code);
	if (preprocessRet < 0)
	{
		return preprocessRet;
	}
	
	int charIndex = 0;
	int codeLength = code.size();
	while (charIndex < codeLength)
	{
		int charIndexOri = charIndex;
		int ret = 0;
		if (code[charIndex] == ' ' || code[charIndex] == '\t' || code[charIndex] == '\n') {
			charIndex++;
		}
		else if ((ret = laxNumber(code, charIndex)) != 0){
			//cout << "Number   : " << code.substr(charIndexOri, charIndex - charIndexOri) << "\n";
		}else if ((ret = laxString(code, charIndex)) != 0) {
			//cout << "String   : " << code.substr(charIndexOri, charIndex - charIndexOri) << "\n";
		}else if ((ret = laxChar(code, charIndex)) != 0) {
			//cout << "Char     : " << code.substr(charIndexOri, charIndex - charIndexOri) << "\n";
		}else if ((ret = laxBranch(code, charIndex)) != 0) {
			//cout << "Branch   : " << code.substr(charIndexOri, charIndex - charIndexOri) << "\n";
		}else if ((ret = laxBinocular(code, charIndex)) != 0) {
			//cout << "Binocular: " << code.substr(charIndexOri, charIndex - charIndexOri) << "\n";
		}else if ((ret = laxMonocular(code, charIndex)) != 0) {
			//cout << "Monocular: " << code.substr(charIndexOri, charIndex - charIndexOri) << "\n";
		}else if ((ret = laxTernary(code, charIndex)) != 0) {
			//cout << "Ternary  : " << code.substr(charIndexOri, charIndex - charIndexOri) << "\n";
		}else if ((ret = laxBracket(code, charIndex)) != 0) {
			//cout << "Bracket  : " << code.substr(charIndexOri, charIndex - charIndexOri) << "\n";
		}else if ((ret = laxOther(code, charIndex)) != 0) {
			//cout << "Other    : " << code.substr(charIndexOri, charIndex - charIndexOri) << "\n";
		}else if ((ret = laxWord(code, charIndex)) != 0) {
			//cout << "Word     : " << code.substr(charIndexOri, charIndex - charIndexOri) << "\n";
		}else {
			return -ErrorUnrecognizedToken;
		}
        if (ret < 0) 
            return ret;
	}
	
	laxProcess();
    //comment = true;
	//for (int i = 0; i < tokenList.size(); i++)
	//{
	//	cout << stringFillBlank(Token::TokenNameList[tokenList[i].type],10) << '\t' << tokenList[i].value << '\n';
	//}
	return 1;
}
namespace
{
    const int LAXNUMBER_DEC_NUM = 1;
    const int LAXNUMBER_HEX_NUM = 2;
    const int LAXNUMBER_OCT_NUM = 3;
    const int LAXNUMBER_FLOAT_NUM  = 4;
}
int ILCCodeParser::laxNumberA(const string& code, int& i, int charIndexOri, int codeLength, int& mumberType) const
{
    int isFloat = 0;
	while (code[i] >= '0' && code[i] <= '9') {
		//token_val = token_val * 10 + (code[i] - '0');
		i++;
		if (i >= codeLength)
			break;
		if (isFloat == 0 && code[i] == '.')
		{
			isFloat = 1;
			i++;
			if (i >= codeLength)
				break;
		}
	}
	if (isFloat || i < codeLength && (code[i] == 'e' || code[i] == 'E'))//识别到是小数
	{
		if (i < codeLength && (code[i] == 'e' || code[i] == 'E'))//识别到是指数形式的小数
		{
			i++;
			if (i < codeLength && (code[i] == '+' || code[i] == '-'))//识别到指数带正负号
			{
				i++;
				i = laxNumberSkipNum(code, i);
				mumberType = LAXNUMBER_FLOAT_NUM; //结果为以1-9开头的带正负号的指数小数
			}
			else if (i < codeLength && code[i] >= '0' && code[i] <= '9')//指数后面直接接数字
			{
				i++;
				i = laxNumberSkipNum(code, i);
				mumberType = LAXNUMBER_FLOAT_NUM; //结果为以1-9开头的不带正负号的指数小数
			}
			else
			{
				return -ErrorExponentHasNoDigits; //指数后面没有数字
			}
		}
		else
		{
			mumberType = LAXNUMBER_FLOAT_NUM; //结果为普通的以1-9开头的小数
		}
	}
	else
	{
		mumberType = LAXNUMBER_DEC_NUM; //结果为以1-9开头的整数
	}
    return 1;
}

int ILCCodeParser::laxNumberB(const string& code, int& i, int charIndexOri, int codeLength, int& mumberType) const
{   i+=CONST_NUM_2;
	//int hexValue = code[i];
	while (i < codeLength &&
        ((code[i] >= '0' && code[i] <= '9') ||
        (code[i] >= 'a' && code[i] <= 'f') ||
        (code[i] >= 'A' && code[i] <= 'F'))) {
		//token_val = token_val * 16 + (hexValue & 15) + (hexValue >= 'A' ? 9 : 0);
		i++;
		//hexValue = code[i];
	}
	mumberType = LAXNUMBER_HEX_NUM; //结果为0x或0X开头的十六进制数
    return 1;
}

int ILCCodeParser::laxNumberC(const string& code, int& i, int charIndexOri, int codeLength, int& mumberType) const
{
    i+=CONST_NUM_2;
	if (i < codeLength && (code[i] == '+' || code[i] == '-'))//识别到指数带正负号
	{
		i++;
		i = laxNumberSkipNum(code, i);
		mumberType = LAXNUMBER_FLOAT_NUM;//结果为以0E或0e开头的带正负号的指数小数
	}
	else if (i < codeLength && code[i] >= '0' && code[i] <= '9') //指数后面直接接数字
	{
		i++;
		i = laxNumberSkipNum(code, i);
		mumberType = LAXNUMBER_FLOAT_NUM;//结果为以0E或0e开头的不带正负号的指数小数
	}
	else
	{
		return -ErrorExponentHasNoDigits;//指数后面没有数字
	}	
    return 1;
}

int ILCCodeParser::laxNumberD(const string& code, int& i, int charIndexOri, int codeLength, int& mumberType) const
{
    int isFloat = 0;
	while (code[i] >= '0' && code[i] <= '9') { //识别可能是八进制或0开头的小数
		//token_val = token_val * 8 + (code[i] - '0');
		i++;
		if (i >= codeLength)
			break;
		if (isFloat == 0 && code[i] == '.')
		{
			isFloat = 1;
			i++;
			if (i >= codeLength)
				break;
		}
	}

	if (isFloat || i < codeLength && (code[i] == 'e' || code[i] == 'E')) //识别到是小数
	{
		if (i < codeLength && (code[i] == 'e' || code[i] == 'E'))//识别到是指数形式的小数
		{
			i++;
			if (i < codeLength && (code[i] == '+' || code[i] == '-'))//识别到指数带正负号
			{
				i++;
			    i = laxNumberSkipNum(code, i);
				mumberType = LAXNUMBER_FLOAT_NUM;//结果为以0开头的带正负号的指数小数
			}
			else if (i < codeLength && code[i] >= '0' && code[i] <= '9')//指数后面直接接数字
			{
				i++;
			    i = laxNumberSkipNum(code, i);
				mumberType = LAXNUMBER_FLOAT_NUM;//结果为以0开头的不带正负号的指数小数
			}
			else
			{
				return -ErrorExponentHasNoDigits;//指数后面没有数字
			}
		}
		else
		{
			mumberType = LAXNUMBER_FLOAT_NUM;//结果为普通的以0开头的小数
		}
	}
	else
	{
		for (int i2 = charIndexOri; i2 < i; i2++)
		{
			if (code[i2] > '7')
			{
				return -ErrorInvalidDigitInOctalConstant;
			}
		}
		mumberType = LAXNUMBER_OCT_NUM; //结果为以0开头的八进制数
	}
    return 1;
}

int ILCCodeParser::laxNumberE(const string& code, int& i, int charIndexOri, int codeLength, int& mumberType) const
{
    i += CONST_NUM_2;
	i = laxNumberSkipNum(code, i);
	if (i < codeLength && (code[i] == 'e' || code[i] == 'E'))//识别到是指数形式的小数
	{
		i++;
		if (i < codeLength && (code[i] == '+' || code[i] == '-'))//识别到指数带正负号
		{
            i++;
			i = laxNumberSkipNum(code, i);
			mumberType = LAXNUMBER_FLOAT_NUM;//结果为以.开头的带正负号的指数小数
		}
		else if (i < codeLength && code[i] >= '0' && code[i] <= '9')//指数后面直接接数字
		{
            i++;
			i = laxNumberSkipNum(code, i);
			mumberType = LAXNUMBER_FLOAT_NUM;//结果为以.开头的不带正负号的指数小数
		}
		else
		{
			return -ErrorExponentHasNoDigits;//指数后面没有数字
		}
	}
	else
	{
		mumberType = LAXNUMBER_FLOAT_NUM;//结果为普通的以.开头的小数
	}
    return 1;
}

int ILCCodeParser::laxNumberSkipNum(const std::string& code, int i) const
{
    int codeLength = code.size();
    while (i < codeLength && code[i] >= '0' && code[i] <= '9') {
		i++;
	}
    return i;
}

std::vector<std::string> ILCCodeParser::numberSuffix = {
    "d", "f", "ub", "us", "u", "b", "s", "l"};
int ILCCodeParser::laxNumber(const string& code, int& charIndex)
{
    int res = 0;
	int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
	int mumberType = 0; //1为普通整数 2为十六进制数 3为八进制数 4为浮点数
	if (i < codeLength && code[i] >= '0' && code[i] <= '9') //判断以数字开头的可能的数值
	{
		int tokenVal = code[i] - '0';
		if (tokenVal > 0) { //以1-9开头的整数或小数
			// dec, starts with [1-9]
			res = laxNumberA(code, i, charIndexOri, codeLength, mumberType);
		}
		else {
			// starts with number 0
			if (i+1 < codeLength && (code[i+1] == 'x' || code[i+1] == 'X')) { //识别十六进制数
				//hex
				res = laxNumberB(code, i, charIndexOri, codeLength, mumberType);
			}
			else if (i+1 < codeLength && (code[i+1] == 'e' || code[i+1] == 'E')) //识别以0E或0e开头的小数
			{
				res = laxNumberC(code, i, charIndexOri, codeLength, mumberType);
			}
			else {
				// oct
                res = laxNumberD(code, i, charIndexOri, codeLength, mumberType);
			}
		}
	}
	else if (i + 1 < codeLength && code[i] == '.' && code[i+1] >= '0' && code[i+1] <= '9')//识别到是以.开头的小数
	{
		res = laxNumberE(code, i, charIndexOri, codeLength, mumberType);
	}
	else
	{
		return 0; //未处理
	}
    
    if(res < 0)
    {
        return res;
    }

    for (int j = 0; j < numberSuffix.size(); j++)
    {
        int suffixLen = numberSuffix[j].length();
        if (i + suffixLen < codeLength &&
            stringToLower(code.substr(i, suffixLen)) == numberSuffix[j])
        {
            i += suffixLen;
            break;
        }
    }

	if (i < codeLength && (code[i] == 'e' || code[i] == 'E' || code[i] == '.'))
	{
		return -ErrorInvalidNumericalExpression; //后面是否跟了其它非法字符
	}

	charIndex = i;
	tokenList.push_back(Token(Token::Number, code.substr(charIndexOri, charIndex - charIndexOri)));
	return 1;
}

int ILCCodeParser::laxWord(const string& code, int& charIndex)
{
	int charIndexOri = charIndex;
	int charIndex2 = charIndex;
	int ret = tryWord(code, charIndex2);
	if (ret <= 0)
	{
		return ret;
	}
	else
	{
		charIndex = charIndex2;
		tokenList.push_back(Token(Token::Word, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
}

int ILCCodeParser::laxString(const string& code, int& charIndex)
{
	int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
	char c = code[i];
	if (c == '\"')
	{
		i++;
		if(i >= codeLength)
			return -ErrorStringMissingTerminatingCharacter;

		c = code[i];
		while (c != '\"' || (c=='\"' && code[i-1] == '\\')) {
			if (c == '\n')
				return -ErrorStringMissingTerminatingCharacter;
			i++;
			if (i >= codeLength)
				return -ErrorStringMissingTerminatingCharacter;
			c = code[i];
		}
		i++;
	}
	if (i == charIndexOri)
	{
		return 0;
	}
	charIndex = i;
	tokenList.push_back(Token(Token::String, code.substr(charIndexOri, i - charIndexOri)));
	return 1;
}

int ILCCodeParser::laxChar(const string& code, int& charIndex)
{
	int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
	char c = code[i];
	if (c == '\'')
	{
		i++;
		if (i >= codeLength)
			return -ErrorCharMissingTerminatingCharacter;

		c = code[i];
		while (c != '\'' || (c == '\'' && code[i - 1] == '\\')) {
			if (c == '\n')
				return -ErrorCharMissingTerminatingCharacter;
			i++;
			if (i >= codeLength)
				return -ErrorCharMissingTerminatingCharacter;
			c = code[i];
		}
		i++;
	}
	if (i == charIndexOri)
	{
		return 0;
	}
	charIndex = i;
	tokenList.push_back(Token(Token::Char, code.substr(charIndexOri, i - charIndexOri)));
	return 1;
}

int ILCCodeParser::laxBranch(const string& code, int& charIndex)
{
	int charIndexOri = charIndex;
	int codeLength = code.length();
	int charIndex2 = charIndex;
	int ret1 = tryWord(code, charIndex2);
	if (ret1 <= 0)
	{
		return ret1;
	}
	else
	{
		string word1 = code.substr(charIndexOri, charIndex2 - charIndexOri);
		if (word1 == "if"){
			tokenList.push_back(Token(Token::If, word1));
		}else if (word1 == "else") {
			tokenList.push_back(Token(Token::Else, word1));
		}else if (word1 == "while") {
			tokenList.push_back(Token(Token::While, word1));
		}else if (word1 == "do") {
			tokenList.push_back(Token(Token::Do, word1));
		}else if (word1 == "for") {
			tokenList.push_back(Token(Token::For, word1));
		}else if (word1 == "continue") {
			tokenList.push_back(Token(Token::Continue, word1));
		}else if (word1 == "switch") {
			tokenList.push_back(Token(Token::Switch, word1));
		}else if (word1 == "case") {
			tokenList.push_back(Token(Token::Case, word1));
		}else if (word1 == "default") {
			tokenList.push_back(Token(Token::Default, word1));
		}else if (word1 == "break") {
			tokenList.push_back(Token(Token::Break, word1));
		}else if (word1 == "return") {
			tokenList.push_back(Token(Token::Return, word1));
		}else{
			return 0;
		}
		charIndex = charIndex2;
		return 1;
	}
}

int ILCCodeParser::laxMonocular(const string& code, int& charIndex)
{
	int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
	if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "++")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::SelfAdd, code.substr(charIndexOri, charIndex -charIndexOri)));
		return 1;
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "--")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::SelfSub, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == '-')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Negative, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == '+')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Positive, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == '!')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Not, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == '~')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::BitNot, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::laxBinocular(const string& code, int& charIndex)
{
    int res = laxBinocularA(code, charIndex);
	if(res != 0)
        return res;
	res = laxBinocularB(code, charIndex);
	if(res != 0)
        return res;
	res = laxBinocularC(code, charIndex);
	if(res != 0)
        return res;
    res = laxBinocularD(code, charIndex);
	if(res != 0)
        return res;
    return 0;
}

int ILCCodeParser::laxBinocularA(const std::string& code, int& charIndex)
{
    int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
	if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "+=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::AddAssign, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "-=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::SubAssign, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "*=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::MulAssign, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "/=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::DivAssign, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "%=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::ModAssign, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_3) == "<<=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::LeftShiftAssign, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_3) == ">>=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::RightShiftAssign, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
    else
    {
        return 0;
    }
	return 1;
}

int ILCCodeParser::laxBinocularB(const std::string& code, int& charIndex)
{
    int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
    if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "&=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::AndAssign, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "^=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::XorAssign, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "|=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::OrAssign, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "!=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::NotEqual, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "==")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::Equal, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == ">=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::GreaterEqual, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "<=")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::LessEqual, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "<<")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::LeftShift, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
    else
    {
        return 0;
    }
    return 1;
}

int ILCCodeParser::laxBinocularC(const std::string& code, int& charIndex)
{
    int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
    if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == ">>")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::RightShift, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "->")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::Arrow, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "&&")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::And, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "||")
	{
		charIndex += CONST_NUM_2;
		tokenList.push_back(Token(Token::Or, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i < codeLength && code[i] == '*')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Mul, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i < codeLength && code[i] == '/')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Div, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i < codeLength && code[i] == '%')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Mod, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i < codeLength && code[i] == '>')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Greater, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
    else
    {
        return 0;
    }
    return 1;
}

int ILCCodeParser::laxBinocularD(const std::string& code, int& charIndex)
{
    int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
    if (i < codeLength && code[i] == '<')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Less, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i < codeLength && code[i] == '&')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::BitAnd, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i < codeLength && code[i] == '|')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::BitOr, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i < codeLength && code[i] == '^')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::BitXor, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i < codeLength && code[i] == '.')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Point, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else if (i < codeLength && code[i] == '=')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Assign, code.substr(charIndexOri, charIndex - charIndexOri)));
	}
	else
	{
		return 0;
	}
    return 1;
}

int ILCCodeParser::laxTernary(const string& code, int& charIndex)
{
	int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
	if (i < codeLength && code[i] == '?')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Condition, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == ':')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Select, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else{
		return 0;
	}
}

int ILCCodeParser::laxBracket(const string& code, int& charIndex)
{
	int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
	if (i < codeLength && code[i] == '[')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::SubscriptLeft, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == ']')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::SubscriptRight, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == '{')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::BraceLeft, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == '}')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::BraceRight, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == '(')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::BracketLeft, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == ')')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::BracketRight, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else{
		return 0;
	}
}

int ILCCodeParser::laxOther(const string& code, int& charIndex)
{
	int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
	if (i + CONST_NUM_5 < codeLength && code.substr(i,CONST_NUM_6) == "sizeof")
	{
		charIndex += CONST_NUM_6;
		tokenList.push_back(Token(Token::SizeOf, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == ';')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Semicolon, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else if (i < codeLength && code[i] == ',')
	{
		charIndex += 1;
		tokenList.push_back(Token(Token::Comma, code.substr(charIndexOri, charIndex - charIndexOri)));
		return 1;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::tryWord(const string& code, int& charIndex) const
{
	int charIndexOri = charIndex;
	int codeLength = code.length();
	int i = charIndex;
	char c = code[i];
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'))
	{
		while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_')) {
			i++;
			if (i >= codeLength)
				break;
			c = code[i];
		}
	}
	if (i == charIndexOri)
	{
		return 0;
	}
	charIndex = i;
	return 1;
}

int ILCCodeParser::laxPreprocess(string& code) const
{
	string retCode;
	
	//去除代码中的\r
	size_t pos = code.find('\r');
	while (pos != string::npos)
	{
		code.erase(pos, 1);
		pos = code.find('\r', pos);
	}

	int codeLength = code.length();
	retCode.reserve(codeLength);

	//去除注释
    retCode = codeRemoveComment(code);
	
	if (retCode.empty())
	{
        code = "";
        return 1;
	}
    //去除重复的空格(有助于识别带空格的数据类型)
	bool isInString = (retCode[0] == '\"');
	for (int i = 1; i < retCode.length();)
	{
		if (retCode[i] == '\"' && retCode[i-1] != '\\' )
		{
			if(!(isInString && retCode[i - 1] == '\\'))
				isInString = !isInString;
		}
		else if (isInString && retCode[i] == '\n')
		{
			isInString = !isInString;
		}
		if (!isInString && retCode[i - 1] == ' ' && retCode[i] == ' ')
		{
			retCode.erase(i, 1);
		}
		else
		{
			i++;
		}
	}
	code = retCode;
	return 1;
}

void ILCCodeParser::laxProcess()
{
	laxProcessDataType();
	//laxProcessTypeConvert();
	laxProcessFunction();
	laxProcessElseIf();
	laxProcessMember();
	laxProcessVariable();
	laxProcessAdd();
	laxProcessSub();
	//laxProcessIndirection();
	//laxProcessAddressOf();
}

void ILCCodeParser::laxProcessDataType()
{
	//识别数据类型
	vector<vector<string>> typeSplits;
	for (int j = 0; j < ILParser::allDataTypeList.size(); j++)
	{
        string typeName = ILParser::allDataTypeList[j];
		vector<string> typeSplit = stringSplit(typeName, " ");
		typeSplits.push_back(typeSplit);
	}
	for (int i = 0; i < tokenList.size(); i++)
	{
		for (int j = 0; j < ILParser::allDataTypeList.size(); j++)
		{
            bool loopBreak = false;
            laxProcessDataTypeA(typeSplits, i, j, loopBreak);
            if(loopBreak)
			    break;
		}
	}
}

void ILCCodeParser::laxProcessDataTypeA(const vector<vector<string>> &typeSplits, int i, int j, bool &loopBreak)
{
    int count = typeSplits[j].size();
	string multiTokenValue = getMultiTokenValue(i, count);
    string typeName = ILParser::allDataTypeList[j];
	if (multiTokenValue == typeName)
	{
		bool allWord = true;
		for (int k = i; k < i+count; k++)
		{
			if (tokenList[k].type != Token::Word)
			{
				allWord = false;
				loopBreak = true;
                return;
			}
		}
		if (!allWord){
			loopBreak = true;
            return;
        }
		tokenList[i].type = Token::DataType;
		tokenList[i].value = multiTokenValue;
		if (count > 1)
		{
			tokenList.erase(tokenList.begin() + i + 1, tokenList.begin() + i + count);
			loopBreak = true;
		}
	}
}

void ILCCodeParser::laxProcessFunction()
{
	for (int i = 0; i < tokenList.size(); i++)
	{
		if (i + 1 < tokenList.size() && tokenList[i].type == Token::Word && tokenList[i+1].type == Token::BracketLeft)
		{
			tokenList[i].type = Token::Function;
		}
	}
}

void ILCCodeParser::laxProcessMember()
{
	for (int i = 0; i < tokenList.size(); i++)
	{
		if (i + 1 < tokenList.size() && (tokenList[i].type == Token::Point || tokenList[i].type == Token::Arrow) && tokenList[i + 1].type == Token::Word)
		{
			tokenList[i + 1].type = Token::Member;
		}
	}
}

//void ILCCodeParser::laxProcessTypeConvert()
//{
//	for (int i = 0; i < tokenList.size(); i++)
//	{
//		if (i + CONST_NUM_2 < tokenList.size() && tokenList[i].type == Token::BracketLeft && tokenList[i+1].dataType == Token::DataType && tokenList[i + CONST_NUM_2].dataType == Token::BracketRight)
//		{
//			tokenList[i].type = Token::TypeConvert;
//			tokenList[i].value = tokenList[i].value + tokenList[i + 1].value + tokenList[i + CONST_NUM_2].value;
//			tokenList.erase(tokenList.begin() + i + 1, tokenList.begin() + i + 3);
//		}
//	}
//}

void ILCCodeParser::laxProcessVariable()
{
	for (int i = 0; i < tokenList.size(); i++)
	{
		if (tokenList[i].type != Token::Word)
		{
			continue;
		}
        string tokenStr = tokenList[i].value;
        string lowerTokenStr =  stringToLower(tokenStr);
		if(lowerTokenStr == "true" || lowerTokenStr == "false"  || lowerTokenStr == "null" )
			continue;
        else if(ILParser::findEnumValue(tokenStr)) {
            tokenList[i].type = Token::Number;
        }else if(ILParser::findTypeDefineByValue(tokenStr)) {
            tokenList[i].type = Token::DataType;
        } else {
            tokenList[i].type = Token::Variable;
        }
	}
}

void ILCCodeParser::laxProcessAdd()
{
	for (int i = 0; i < tokenList.size(); i++)
	{
		if (i + 1 < tokenList.size() && tokenList[i + 1].type == Token::Positive)
		{
			if (tokenList[i].type == Token::TypeConvert || tokenList[i].type == Token::Assign ||
				tokenList[i].type == Token::Return || tokenList[i].type == Token::Case ||
				tokenList[i].type == Token::AddAssign || tokenList[i].type == Token::SubAssign ||
				tokenList[i].type == Token::MulAssign || tokenList[i].type == Token::DivAssign ||
				tokenList[i].type == Token::ModAssign || tokenList[i].type == Token::NotEqual ||
				tokenList[i].type == Token::Equal || tokenList[i].type == Token::GreaterEqual ||
				tokenList[i].type == Token::LessEqual || tokenList[i].type == Token::LeftShift ||
				tokenList[i].type == Token::RightShift || tokenList[i].type == Token::And ||
				tokenList[i].type == Token::Or || tokenList[i].type == Token::Mul ||
				tokenList[i].type == Token::Div || tokenList[i].type == Token::Mod ||
				tokenList[i].type == Token::Greater || tokenList[i].type == Token::Less ||
				tokenList[i].type == Token::BitAnd || tokenList[i].type == Token::BitOr ||
				tokenList[i].type == Token::BitXor || tokenList[i].type == Token::Condition ||
				tokenList[i].type == Token::Select || tokenList[i].type == Token::SubscriptLeft ||
				tokenList[i].type == Token::BraceLeft || tokenList[i].type == Token::BracketLeft ||
				tokenList[i].type == Token::Semicolon || tokenList[i].type == Token::Comma ||
				tokenList[i].type == Token::Add || tokenList[i].type == Token::Sub ||
				tokenList[i].type == Token::Negative || tokenList[i].type == Token::Positive ||
				tokenList[i].type == Token::Not || tokenList[i].type == Token::BitNot)
			{
				tokenList[i + 1].type = Token::Positive;
			}
			else if (tokenList[i].type == Token::Number || tokenList[i].type == Token::String ||
				tokenList[i].type == Token::Char || tokenList[i].type == Token::SelfAdd ||
				tokenList[i].type == Token::SelfSub || tokenList[i].type == Token::MulAssign ||
				tokenList[i].type == Token::SubscriptRight ||
                //暂注释掉该项，放到后面判断，因为可能是类型转换，也可能是加号 tokenList[i].type == Token::BracketRight ||
				tokenList[i].type == Token::BraceRight || tokenList[i].type == Token::Variable ||
				tokenList[i].type == Token::Member)
			{
				tokenList[i + 1].type = Token::Add;
			}
			else
			{
                //comment = true;
				//tokenList[i + 1].type = Token::Unknow;
			}
			
		}
		else if (i == 0 && tokenList[i].type == Token::Positive)
		{
			tokenList[i].type = Token::Positive;
		}
	}
}

void ILCCodeParser::laxProcessSub()
{
	for (int i = 0; i < tokenList.size(); i++)
	{
		if (i + 1 < tokenList.size() && tokenList[i + 1].type == Token::Negative)
		{
			if (tokenList[i].type == Token::TypeConvert || tokenList[i].type == Token::Assign ||
				tokenList[i].type == Token::Return || tokenList[i].type == Token::Case ||
				tokenList[i].type == Token::AddAssign || tokenList[i].type == Token::SubAssign ||
				tokenList[i].type == Token::MulAssign || tokenList[i].type == Token::DivAssign ||
				tokenList[i].type == Token::ModAssign || tokenList[i].type == Token::NotEqual ||
				tokenList[i].type == Token::Equal || tokenList[i].type == Token::GreaterEqual ||
				tokenList[i].type == Token::LessEqual || tokenList[i].type == Token::LeftShift ||
				tokenList[i].type == Token::RightShift || tokenList[i].type == Token::And ||
				tokenList[i].type == Token::Or || tokenList[i].type == Token::Mul ||
				tokenList[i].type == Token::Div || tokenList[i].type == Token::Mod ||
				tokenList[i].type == Token::Greater || tokenList[i].type == Token::Less ||
				tokenList[i].type == Token::BitAnd || tokenList[i].type == Token::BitOr ||
				tokenList[i].type == Token::BitXor || tokenList[i].type == Token::Condition ||
				tokenList[i].type == Token::Select || tokenList[i].type == Token::SubscriptLeft ||
				tokenList[i].type == Token::BraceLeft || tokenList[i].type == Token::BracketLeft ||
				tokenList[i].type == Token::Semicolon || tokenList[i].type == Token::Comma ||
				tokenList[i].type == Token::Add || tokenList[i].type == Token::Sub ||
				tokenList[i].type == Token::Negative || tokenList[i].type == Token::Positive ||
				tokenList[i].type == Token::Not || tokenList[i].type == Token::BitNot)
			{
				tokenList[i + 1].type = Token::Negative;
			}
			else if (tokenList[i].type == Token::Number || tokenList[i].type == Token::String ||
				tokenList[i].type == Token::Char || tokenList[i].type == Token::SelfAdd ||
				tokenList[i].type == Token::SelfSub || tokenList[i].type == Token::MulAssign ||
				tokenList[i].type == Token::SubscriptRight || tokenList[i].type == Token::BraceRight ||
                //暂注释掉该项，放到后面判断，因为可能是类型转换，也可能是减号 tokenList[i].type == Token::BracketRight ||
				tokenList[i].type == Token::Variable || tokenList[i].type == Token::Member)
			{
				tokenList[i + 1].type = Token::Sub;
			}
			else
			{
                //comment = true;
				//tokenList[i + 1].type = Token::Unknow;
			}

		}
		else if(i == 0 && tokenList[i].type == Token::Negative)
		{
			tokenList[i].type = Token::Negative;
		}
	}
}

void ILCCodeParser::laxProcessIndirection()
{
	for (int i = 0; i < tokenList.size(); i++)
	{
		if (i + 1 < tokenList.size() && tokenList[i + 1].type == Token::Mul)
		{
			if (tokenList[i].type == Token::Number || tokenList[i].type == Token::Char ||
				tokenList[i].type == Token::SelfAdd || tokenList[i].type == Token::SelfSub ||
				tokenList[i].type == Token::SubscriptRight || 
                //暂注释掉该项，放到后面判断，因为可能是类型转换，也可能是乘号 tokenList[i].type == Token::BracketRight ||
				tokenList[i].type == Token::Variable || tokenList[i].type == Token::Member)
			{
				tokenList[i + 1].type = Token::Mul;
			}
			else if (tokenList[i].type == Token::Negative || tokenList[i].type == Token::Positive ||
				tokenList[i].type == Token::Not || tokenList[i].type == Token::BitNot ||
				tokenList[i].type == Token::AddAssign || tokenList[i].type == Token::SubAssign ||
				tokenList[i].type == Token::MulAssign || tokenList[i].type == Token::DivAssign ||
				tokenList[i].type == Token::ModAssign || tokenList[i].type == Token::NotEqual ||
				tokenList[i].type == Token::Equal || tokenList[i].type == Token::GreaterEqual ||
				tokenList[i].type == Token::LessEqual || tokenList[i].type == Token::LeftShift ||
				tokenList[i].type == Token::RightShift || tokenList[i].type == Token::And ||
				tokenList[i].type == Token::Or || tokenList[i].type == Token::Mul ||
				tokenList[i].type == Token::Div || tokenList[i].type == Token::Mod ||
				tokenList[i].type == Token::Greater || tokenList[i].type == Token::Less ||
				tokenList[i].type == Token::BitAnd || tokenList[i].type == Token::BitOr ||
				tokenList[i].type == Token::BitXor || tokenList[i].type == Token::Assign ||
				tokenList[i].type == Token::Condition || tokenList[i].type == Token::Select ||
				tokenList[i].type == Token::SubscriptLeft || tokenList[i].type == Token::BraceLeft ||
				tokenList[i].type == Token::BracketLeft || tokenList[i].type == Token::Semicolon ||
				tokenList[i].type == Token::Comma || tokenList[i].type == Token::Add ||
				tokenList[i].type == Token::Sub || tokenList[i].type == Token::Indirection ||
				tokenList[i].type == Token::AddressOf || tokenList[i].type == Token::TypeConvert)
			{
				tokenList[i + 1].type = Token::Indirection;
			}
			else
			{
                //放到后面判断，因为可能是类型转换，也可能是乘号
                //comment = true;
				tokenList[i + 1].type = Token::Indirection;
			}

		}
		else if (i == 0 && tokenList[i].type == Token::Mul)
		{
			tokenList[i].type = Token::Indirection;
		}
	}
}

void ILCCodeParser::laxProcessAddressOf()
{
	for (int i = 0; i < tokenList.size(); i++)
	{
		if (i + 1 < tokenList.size() && tokenList[i + 1].type == Token::BitAnd)
		{
			if (tokenList[i].type == Token::Number || tokenList[i].type == Token::Char ||
				tokenList[i].type == Token::SelfAdd || tokenList[i].type == Token::SelfSub ||
				tokenList[i].type == Token::SubscriptRight ||
                //暂注释掉该项，放到后面判断，因为可能是类型转换，也可能是乘号 tokenList[i].type == Token::BracketRight ||
				tokenList[i].type == Token::Variable || tokenList[i].type == Token::Member)
			{
				tokenList[i + 1].type = Token::BitAnd;
			}
			else if (tokenList[i].type == Token::Negative || tokenList[i].type == Token::Positive ||
				tokenList[i].type == Token::Not || tokenList[i].type == Token::BitNot ||
				tokenList[i].type == Token::AddAssign || tokenList[i].type == Token::SubAssign ||
				tokenList[i].type == Token::MulAssign || tokenList[i].type == Token::DivAssign ||
				tokenList[i].type == Token::ModAssign || tokenList[i].type == Token::NotEqual ||
				tokenList[i].type == Token::Equal || tokenList[i].type == Token::GreaterEqual ||
				tokenList[i].type == Token::LessEqual || tokenList[i].type == Token::LeftShift ||
				tokenList[i].type == Token::RightShift || tokenList[i].type == Token::And ||
				tokenList[i].type == Token::Or || tokenList[i].type == Token::Mul ||
				tokenList[i].type == Token::Div || tokenList[i].type == Token::Mod ||
				tokenList[i].type == Token::Greater || tokenList[i].type == Token::Less ||
				tokenList[i].type == Token::BitAnd || tokenList[i].type == Token::BitOr ||
				tokenList[i].type == Token::BitXor || tokenList[i].type == Token::Assign ||
				tokenList[i].type == Token::Condition || tokenList[i].type == Token::Select ||
				tokenList[i].type == Token::SubscriptLeft || tokenList[i].type == Token::BraceLeft ||
				tokenList[i].type == Token::BracketLeft || tokenList[i].type == Token::Semicolon ||
				tokenList[i].type == Token::Comma || tokenList[i].type == Token::Add ||
				tokenList[i].type == Token::Sub || tokenList[i].type == Token::Indirection ||
				tokenList[i].type == Token::AddressOf || tokenList[i].type == Token::TypeConvert)
			{
				tokenList[i + 1].type = Token::AddressOf;
			}
			else
			{
                //comment = true;
                //放到后面判断，因为可能是类型转换，也可能是与号
				tokenList[i + 1].type = Token::AddressOf;
			}

		}
		else if (i == 0 && tokenList[i].type == Token::BitAnd)
		{
			tokenList[i].type = Token::AddressOf;
		}
	}
}

void ILCCodeParser::laxProcessElseIf()
{
	for (int i = 0; i < tokenList.size(); i++)
	{
		if (i + 1 < tokenList.size() && tokenList[i].type == Token::Else && tokenList[i + 1].type == Token::If)
		{
			tokenList[i].type = Token::ElseIf;
			tokenList[i].value = tokenList[i].value + " " + tokenList[i + 1].value;
			tokenList.erase(tokenList.begin() + i + 1, tokenList.begin() + i + CONST_NUM_2);
		}
	}
}


int ILCCodeParser::syntaxParse()
{
	statementRoot->depth = 0;
	statementRoot->type = static_cast<Statement::Type>(-1);
	statementRoot->parentStatement = nullptr;
	int tokenIndex = 0;
	int tokenListSize = tokenList.size();
	while (tokenIndex < tokenListSize)
	{
		int tokenIndexOri = tokenIndex;
		int ret = syntaxStatement(tokenIndex, statementRoot);
		if (ret < 0) {
			//statementRoot->print();
			return ret;
		}
	}

	//statementRoot->print();

	return 1;
}


int ILCCodeParser::syntaxStatement(int& tokenIndex, Statement* parent)
{
	int ret;
	if (tokenIndex >= tokenList.size())
	{
		//cout << "error(" << -ErrorMissingStatement << ")\n";
		return -ErrorMissingStatement;
	}
	else if ((ret = syntaxStmtIf(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtElse(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtElseIf(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtWhile(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtDoWhile(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtFor(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtContinue(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtSwitch(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtCase(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtDefault(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtBreak(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtReturn(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtBraceStatement(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtLocalVariable(tokenIndex, parent)) != 0) {
	}
	else if ((ret = syntaxStmtExpressionWithSemicolon(tokenIndex, parent)) != 0) {
	}
	else {
		cout << "error(" << -ErrorUnrecognizedStatement << "): " << tokenList[tokenIndex].value << "\n";
		return -ErrorUnrecognizedStatement;
	}
	if (ret < 0) return ret;
	return 1;
}



int ILCCodeParser::syntaxStmtIf(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::If)
	{
		tokenIndex++;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketLeft)
			tokenIndex++;
		else
			return -ErrorStatementMissingBracketLeft;
		StmtIf* stmt = static_cast<StmtIf*>(createStatement(Statement::If, parent));

		//if条件，吸收一个表达式，不算上括号的表达式
		int res = syntaxExpression(tokenIndex, stmt);
		if (res < 0) return res;
		if (stmt->innerExpressions.empty()) 
			return -ErrorExpressionCanNotBeEmpty;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketRight)
			tokenIndex++;
		else
			return -ErrorStatementMissingBracketRight;
		//if语句块，吸收一个语句，算上由大括号合并的语句块
		res = syntaxStatement(tokenIndex, stmt);
		if (res < 0) return res;


		//如果吸收的语句是If或ElseIf,继续吸收之后可能的Else或ElseIf
		while ((stmt->childStatements[stmt->childStatements.size() - 1]->type == Statement::If ||
			stmt->childStatements[stmt->childStatements.size() - 1]->type == Statement::ElseIf) &&
			(tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::Else ||
				tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::ElseIf))
		{
			res = syntaxStatement(tokenIndex, stmt);
			if (res < 0) return res;
		}

		
		stmt->condition = stmt->innerExpressions[0];
		stmt->statements = stmt->childStatements;

		return 1;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtElse(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::Else)
	{
		tokenIndex++;
		
		StmtElse* stmt = static_cast<StmtElse*>(createStatement(Statement::Else, parent));
		
		//else语句块，吸收一个语句，算上由大括号合并的语句块
		int res = syntaxStatement(tokenIndex, stmt);
		if (res < 0) return res;

		stmt->statements = stmt->childStatements;

		return 1;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtElseIf(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::ElseIf)
	{
		tokenIndex++;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketLeft)
			tokenIndex++;
		else
			return -ErrorStatementMissingBracketLeft;
		StmtElseIf* stmt = static_cast<StmtElseIf*>(createStatement(Statement::ElseIf, parent));

		//else if条件，吸收一个表达式，不算上括号的表达式
		int res = syntaxExpression(tokenIndex, stmt);
		if (res < 0) return res;
		if (stmt->innerExpressions.empty())
			return -ErrorExpressionCanNotBeEmpty;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketRight)
			tokenIndex++;
		else
			return -ErrorStatementMissingBracketRight;
		//else if语句块，吸收一个语句，算上由大括号合并的语句块
		res = syntaxStatement(tokenIndex, stmt);
		if (res < 0) return res;


		//如果吸收的语句是If或ElseIf,继续吸收之后可能的Else或ElseIf
		while ((stmt->childStatements[stmt->childStatements.size() - 1]->type == Statement::If ||
			stmt->childStatements[stmt->childStatements.size() - 1]->type == Statement::ElseIf) &&
			(tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::Else ||
				tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::ElseIf))
		{
			res = syntaxStatement(tokenIndex, stmt);
			if (res < 0) return res;
		}

		
		stmt->condition = stmt->innerExpressions[0];
		stmt->statements = stmt->childStatements;

		return 1;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtWhile(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::While)
	{
		tokenIndex++;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketLeft)
			tokenIndex++;
		else
			return -ErrorStatementMissingBracketLeft;
		StmtWhile* stmt = static_cast<StmtWhile*>(createStatement(Statement::While, parent));
		
		//While条件，吸收一个表达式，不算上括号的表达式
		int res = syntaxExpression(tokenIndex, stmt);
		if (res < 0) return res;
		if (stmt->innerExpressions.empty())
			return -ErrorExpressionCanNotBeEmpty;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketRight)
			tokenIndex++;
		else
			return -ErrorStatementMissingBracketRight;
		//While语句块，吸收一个语句，算上由大括号合并的语句块
		res = syntaxStatement(tokenIndex, stmt);
		if (res < 0) return res;

		//如果吸收的语句是If或ElseIf,继续吸收之后可能的Else或ElseIf
		while ((stmt->childStatements[stmt->childStatements.size() - 1]->type == Statement::If ||
			stmt->childStatements[stmt->childStatements.size() - 1]->type == Statement::ElseIf) &&
			(tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::Else ||
				tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::ElseIf))
		{
			res = syntaxStatement(tokenIndex, stmt);
			if (res < 0) return res;
		}

		
		stmt->condition = stmt->innerExpressions[0];
		stmt->statements = stmt->childStatements;


		return 1;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtDoWhile(int& tokenIndex, Statement* parent)
{
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::Do)
	{
		tokenIndex++;
		StmtDoWhile* stmt = static_cast<StmtDoWhile*>(createStatement(Statement::DoWhile, parent));

		//DoWhile语句块，吸收一个语句，算上由大括号合并的语句块
		int res = syntaxStatement(tokenIndex, stmt);
		if (res < 0) return res;

		
		//如果吸收的语句是If或ElseIf,继续吸收之后可能的Else或ElseIf
		while ((stmt->childStatements[stmt->childStatements.size() - 1]->type == Statement::If ||
			stmt->childStatements[stmt->childStatements.size() - 1]->type == Statement::ElseIf) &&
			(tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::Else ||
				tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::ElseIf))
		{
			res = syntaxStatement(tokenIndex, stmt);
			if (res < 0) return res;
		}

		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::While)
			tokenIndex++;
		else
			return -ErrorStatementMissingWhile;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketLeft)
			tokenIndex++;
		else
			return -ErrorStatementMissingBracketLeft;
		
		//DoWhile条件，吸收一个表达式，不算上括号的表达式
		res = syntaxExpression(tokenIndex, stmt);
		if (res < 0) return res;
		if (stmt->innerExpressions.empty())
			return -ErrorExpressionCanNotBeEmpty;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketRight)
			tokenIndex++;
		else
			return -ErrorStatementMissingBracketRight;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::Semicolon)
			tokenIndex++;
		else
			return -ErrorStatementMissingSemicolon;

		stmt->condition = stmt->innerExpressions[0];
		stmt->statements = stmt->childStatements;

		return 1;
	}
	return 0;
}

int ILCCodeParser::syntaxStmtFor(int& tokenIndex, Statement* parent)
{
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::For)
	{
		tokenIndex++;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketLeft)
			tokenIndex++;
		else
			return -ErrorStatementMissingBracketLeft;
		StmtFor* stmt = static_cast<StmtFor*>(createStatement(Statement::For, parent));
		int res = syntaxStmtForA(tokenIndex, stmt, tokenListSize);
        if (res < 0) return res;
        
		res = syntaxStmtForB(tokenIndex, stmt, tokenListSize);
        if (res < 0) return res;

		stmt->initial = stmt->childStatements[0]->childStatements;
		if(stmt->childStatements[1]->innerExpressions.size() > 0 && stmt->childStatements[1]->innerExpressions[0])
			stmt->condition = stmt->childStatements[1]->innerExpressions[0];
		for(int i = 0; i < stmt->innerExpressions.size(); i++)
		{
			StmtExpression* statementIteratorTemp = new StmtExpression();
			statementIteratorTemp->type = Statement::ExpressionWithSemicolon;
			statementIteratorTemp->depth = parent->depth + CONST_NUM_2;
			statementIteratorTemp->parentStatement = stmt;
			statementIteratorTemp->innerExpressions.push_back(stmt->innerExpressions[i]);
			stmt->iterator.push_back(statementIteratorTemp);
		}
		for(int i = CONST_NUM_2; i < stmt->childStatements.size();i++)
		{
			stmt->statements.push_back(stmt->childStatements[i]);
		}

		return 1;
	}
	return 0;
}

int ILCCodeParser::syntaxStmtForA(int& tokenIndex, Statement* stmt, int tokenListSize)
{
    //For语句块，吸收一个语句，for的初始化语句
	int res = syntaxStatement(tokenIndex, stmt);
	if (res < 0) return res;
	if (stmt->childStatements[0]->type != Statement::LocalVariable && stmt->childStatements[0]->type != Statement::ExpressionWithSemicolon)
		return -ErrorStatementForInitializationError;
	//For语句块，再吸收一个语句，for的判断条件
	res = syntaxStatement(tokenIndex, stmt);
	if (res < 0) return res;
	if (stmt->childStatements[1]->type != Statement::ExpressionWithSemicolon)
		return -ErrorStatementForInitializationError;
	//For语句块，再吸收多个表达式，for的循环执行语句
	while (true)
	{
		int res = syntaxExpression(tokenIndex, stmt);
		if (res < 0) return res;

		if (tokenIndex < tokenListSize)
		{
			if (tokenList[tokenIndex].type == Token::BracketRight)
			{
				break;
			}
			else if (tokenList[tokenIndex].type == Token::Comma)
			{
				tokenIndex += 1;
			}
			else
			{
				return -ErrorStatementMissingSemicolon;
			}
		}
		else
		{
			return -ErrorStatementMissingSemicolon;
		}
	}
	if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketRight)
		tokenIndex++;
	else
		return -ErrorStatementMissingBracketRight;
    return 0;
}

int ILCCodeParser::syntaxStmtForB(int& tokenIndex, Statement* stmt, int tokenListSize)
{
    //For语句块，吸收一个语句，算上由大括号合并的语句块
	int res = syntaxStatement(tokenIndex, stmt);
	if (res < 0) return res;
    //如果吸收的语句是If或ElseIf,继续吸收之后可能的Else或ElseIf
	while ((stmt->childStatements[stmt->childStatements.size() - 1]->type == Statement::If ||
		stmt->childStatements[stmt->childStatements.size() - 1]->type == Statement::ElseIf) &&
		(tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::Else ||
			tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::ElseIf))
	{
		res = syntaxStatement(tokenIndex, stmt);
		if (res < 0) return res;
	}
    return 0;
}

int ILCCodeParser::syntaxStmtContinue(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::Continue)
	{
		tokenIndex++;

		StmtContinue* stmt = static_cast<StmtContinue*>(createStatement(Statement::Continue, parent)); 
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::Semicolon)
		{
			tokenIndex++;

			return 1;
		}
		else
			return -ErrorStatementMissingSemicolon;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtSwitch(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::Switch)
	{
		tokenIndex++;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketLeft)
			tokenIndex++;
		else
			return -ErrorStatementMissingBracketLeft;
		StmtSwitch* stmt = static_cast<StmtSwitch*>(createStatement(Statement::Switch, parent));

		//Switch条件，吸收一个表达式，不算上括号的表达式
		int res = syntaxExpression(tokenIndex, stmt);
		if (res < 0) return res;
		if (stmt->innerExpressions.empty())
			return -ErrorExpressionCanNotBeEmpty;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BracketRight)
			tokenIndex++;
		else
			return -ErrorStatementMissingBracketRight;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BraceLeft)
			tokenIndex++;
		else
			return -ErrorStatementMissingBraceLeft;
		while (tokenIndex < tokenListSize)
		{
			if (tokenList[tokenIndex].type == Token::BraceRight)
				break;
			int res = syntaxStatement(tokenIndex, stmt);
			if (res < 0) return res;
		}
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BraceRight)
			tokenIndex++;
		else
			return -ErrorStatementMissingBraceRight;

		
		stmt->condition = stmt->innerExpressions[0];
		stmt->statements = stmt->childStatements;


		return 1;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtCase(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::Case)
	{
		tokenIndex++;

		StmtCase* stmt = static_cast<StmtCase*>(createStatement(Statement::Case, parent));

		//Case语句，吸收一个Expression
		int res = syntaxExpression(tokenIndex, stmt);
		if (res < 0) return res;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::Select)
		{
			tokenIndex++;

			stmt->value = stmt->innerExpressions[0];
			return 1;
		}
		else
			return -ErrorStatementMissingColon;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtDefault(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::Default)
	{
		tokenIndex++;

		StmtDefault* stmt = static_cast<StmtDefault*>(createStatement(Statement::Default, parent));
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::Select)
		{
			tokenIndex++;
			return 1;
		}
		else
			return -ErrorStatementMissingColon;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtBreak(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::Break)
	{
		tokenIndex++;

		StmtBreak* stmt = static_cast<StmtBreak*>(createStatement(Statement::Break, parent));

		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::Semicolon)
		{
			tokenIndex++;
			return 1;
		}
		else
			return -ErrorStatementMissingSemicolon;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtReturn(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::Return)
	{
		tokenIndex++;

		StmtReturn* stmt = static_cast<StmtReturn*>(createStatement(Statement::Return, parent));

		//Return语句，吸收一个Expression
		int res = syntaxExpression(tokenIndex, stmt);
		if (res < 0) return res;
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::Semicolon)
		{
			tokenIndex++;

			if(stmt->innerExpressions.empty())
				stmt->expression = nullptr;
			else
				stmt->expression = stmt->innerExpressions[0];
			return 1;
		}
		else
			return -ErrorStatementMissingSemicolon;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtBraceStatement(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::BraceLeft)
	{
		Statement* stmt = createStatement(Statement::BraceStatement, parent);
		tokenIndex++;
		while (tokenIndex < tokenListSize)
		{
			if (tokenList[tokenIndex].type == Token::BraceRight)
				break;
			int res = syntaxStatement(tokenIndex, stmt);
			if (res < 0) return res;
		}
		if (tokenIndex < tokenListSize && tokenList[tokenIndex].type == Token::BraceRight)
			tokenIndex++;
		else
			return -ErrorStatementMissingBraceRight;
		return 1;
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtLocalVariable(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	if (i < tokenListSize && tokenList[i].type == Token::DataType)
	{
		StmtLocalVariable* stmt = static_cast<StmtLocalVariable*>(createStatement(Statement::LocalVariable, parent));
		int res;
		
		//变量类型在表达式内存储
		Expression* dataTypeExp = createExpression(Token::DataType, tokenList[tokenIndex].value, false);
		stmt->innerExpressions.push_back(dataTypeExp);
		tokenIndex++;
		while (tokenIndex < tokenList.size())
		{
            bool loopContinue = false;
            bool loopBreak = false;
			if (tokenList[tokenIndex].type == Token::BitAnd)
			{
                res = syntaxStmtLocalVariableA(tokenIndex, stmt, loopContinue, loopBreak);
			}
			else
			{
				res = syntaxStmtLocalVariableB(tokenIndex, stmt, loopContinue, loopBreak);
			}
            if(res < 0)
                return res;
            if(loopContinue)
                continue;
            if(loopBreak)
                break;
		}
		if (tokenList[tokenIndex].type == Token::Semicolon)
		{
			tokenIndex += 1;
			return 1;
		}
		else
		{
			return -ErrorStatementMissingSemicolon;
		}
	}
	else
	{
		return 0;
	}
}

int ILCCodeParser::syntaxStmtLocalVariableA(int& tokenIndex, StmtLocalVariable* stmt, 
    bool& loopContinue, bool& loopBreak)
{
    Expression* dataVariableAddressOfExp = createExpression(Token::References, tokenList[tokenIndex].value,
        false);
    stmt->innerExpressions.push_back(dataVariableAddressOfExp);
    tokenIndex++;

    if (tokenList[tokenIndex].type == Token::Variable)
    {
	    Expression* dataVariableExp = createExpression(Token::Variable, tokenList[tokenIndex].value, false);
	    stmt->innerExpressions.push_back(dataVariableExp);
	    tokenIndex++;

	    if (tokenList[tokenIndex].type == Token::Assign)
	    {
		    Expression* assignExp = createExpression(Token::Assign, tokenList[tokenIndex].value, false);
		    stmt->innerExpressions.push_back(assignExp);
		    tokenIndex++;

		    int innerExpressionsCount = stmt->innerExpressions.size();
		    int res = syntaxExpression(tokenIndex, stmt);
		    if (res < 0) return res;
		    if (innerExpressionsCount == stmt->innerExpressions.size())
			    return -ErrorExpressionCanNotBeEmpty;
						
		    if (tokenList[tokenIndex].type == Token::Semicolon)
		    {
			    loopBreak = true;
                return 0;
		    }
		    else if (tokenList[tokenIndex].type == Token::Comma)
		    {
			    Expression* commaExp = createExpression(Token::Comma, tokenList[tokenIndex].value, false);
			    stmt->innerExpressions.push_back(commaExp);
			    tokenIndex++;
			    loopContinue = true;
                return 0;
		    }
		    else
		    {
			    return -ErrorStatementMissingSemicolon;
		    }
	    }
	    else
	    {
		    return -ErrorStatementLocalVariableMissingInitialization;
	    }
    }
    else
    {
	    return -ErrorStatementLocalVariableMissingVariable;
    }
    return 0;
}

int ILCCodeParser::syntaxStmtLocalVariableB(int& tokenIndex, StmtLocalVariable* stmt, 
    bool& loopContinue, bool& loopBreak)
{
    int pointerCount = 0;
	string pointerStr = "";
	//看变量是否是指针类型
	while (tokenList[tokenIndex].type == Token::Mul)
	{
		pointerCount++;
		pointerStr += "*";
		tokenIndex++;
	}
	if (pointerCount > 0)
	{
		Expression* dataVariableAddressExp = createExpression(Token::Pointer, pointerStr, false);
		stmt->innerExpressions.push_back(dataVariableAddressExp);
	}

	if (tokenList[tokenIndex].type == Token::Variable) //判断接下来是否是变量名
	{
        int res = syntaxStmtLocalVariableBA(tokenIndex, stmt, loopContinue, loopBreak);
        if(res < 0)
            return res;
        if(loopContinue || loopBreak)
            return 0;
        
        res = syntaxStmtLocalVariableBB(tokenIndex, stmt, loopContinue, loopBreak);
        if(res < 0)
            return res;
        if(loopContinue || loopBreak)
            return 0;
	}
	else
	{
		return -ErrorStatementLocalVariableMissingVariable;
	}
    return 0;
}

int ILCCodeParser::syntaxStmtLocalVariableBA(int& tokenIndex, StmtLocalVariable* stmt, 
    bool& loopContinue, bool& loopBreak)
{
    loopBreak = false;
    loopContinue = false;
    Expression* dataVariableExp = createExpression(Token::Variable, tokenList[tokenIndex].value, false);
	stmt->innerExpressions.push_back(dataVariableExp);
	tokenIndex++;

	while (tokenList[tokenIndex].type == Token::SubscriptLeft)//吸收可能的数组中括号
	{
		//Expression* dataVariableExp = createExpression(Token::SubscriptLeft, tokenList[tokenIndex].value, false); //吸收左中括号
		//stmt->innerExpressions.push_back(dataVariableExp);
		tokenIndex++;
		int lastExpCount = stmt->innerExpressions.size();
		int res = syntaxExpression(tokenIndex, stmt);
		if (res < 0) return res;
		if (tokenList[tokenIndex].type == Token::SubscriptRight) //吸收右中括号
		{
			if (stmt->innerExpressions.size() != lastExpCount)
			{
				Expression* dimensionExp = stmt->innerExpressions[stmt->innerExpressions.size() - 1];
				dimensionExp->type = Token::Dimension;
				dimensionExp->expressionStr = "[" + dimensionExp->expressionStr + "]";
			}
			else
			{
				Expression* dataVariableDimensionExp = createExpression(Token::Dimension, "[]", false);
				stmt->innerExpressions.push_back(dataVariableDimensionExp);
			}
			//Expression* dataVariableExp = createExpression(Token::SubscriptRight, tokenList[tokenIndex].value, false);
			//stmt->innerExpressions.push_back(dataVariableExp);
			tokenIndex++;
		}
		else
		{
			return -ErrorStatementLocalVariableMissingSubscriptRight;
		}
	}
    return 0;
}

int ILCCodeParser::syntaxStmtLocalVariableBB(int& tokenIndex, StmtLocalVariable* stmt, 
    bool& loopContinue, bool& loopBreak)
{
    
	if (tokenList[tokenIndex].type == Token::Semicolon) //是分号则结束
	{
		loopBreak = true;
        return 0;
	}
	else if (tokenList[tokenIndex].type == Token::Comma) //是逗号则继续
	{
		Expression* commaExp = createExpression(Token::Comma, tokenList[tokenIndex].value, false);
		stmt->innerExpressions.push_back(commaExp);
		tokenIndex++;
		loopContinue = true;
        return 0;
	}
	else if (tokenList[tokenIndex].type == Token::Assign) //是等号则吸收等号右边的表达式
	{
		Expression* assignExp = createExpression(Token::Assign, tokenList[tokenIndex].value, false);
		stmt->innerExpressions.push_back(assignExp);
		tokenIndex++;

		int innerExpressionsCount = stmt->innerExpressions.size();
		int res = syntaxExpression(tokenIndex, stmt);
		if (res < 0) return res;
		if (innerExpressionsCount == stmt->innerExpressions.size())
			return -ErrorExpressionCanNotBeEmpty;

		if (tokenIndex < tokenList.size() && tokenList[tokenIndex].type == Token::Semicolon)//是分号则结束
		{
		    loopBreak = true;
            return 0;
		}
		else if (tokenIndex < tokenList.size() && tokenList[tokenIndex].type == Token::Comma)//是逗号则继续
		{
			Expression* commaExp = createExpression(Token::Comma, tokenList[tokenIndex].value, false);
			stmt->innerExpressions.push_back(commaExp);
			tokenIndex++;
		    loopContinue = true;
            return 0;
		}
		else
		{
			return -ErrorStatementMissingSemicolon;
		}
	}
    return 0;
}

int ILCCodeParser::syntaxStmtExpressionWithSemicolon(int& tokenIndex, Statement* parent)
{
	int tokenIndexOri = tokenIndex;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	
	StmtExpression* stmt = static_cast<StmtExpression*>(createStatement(Statement::ExpressionWithSemicolon, 
        parent));
	while (true)
	{
		//ExpressionWithSemicolon吸收一个表达式，不算上括号的表达式
		int res = syntaxExpression(tokenIndex, stmt);
		if (res < 0) return res;

		if (tokenIndex < tokenListSize)
		{
			if (tokenList[tokenIndex].type == Token::Semicolon)
			{
				tokenIndex += 1;
				return 1;
			}
			else if(tokenList[tokenIndex].type == Token::Comma)
			{
				tokenIndex++;
			}
			else
			{
				return -ErrorStatementMissingSemicolon;
			}
		}
		else
		{
			return -ErrorStatementMissingSemicolon;
		}
	}
	return 1;
	
}


int ILCCodeParser::syntaxExpression(int& tokenIndex, Statement* parent)
{
	//通过栈存储现有表达式、符号和括号实现符号的匹配和解析
	vector<Expression*> expStack;
	int tokenListSize = tokenList.size();
	int i = tokenIndex;
	int res;
	Token::Type curTokenType;
	while (i < tokenListSize)
	{
        bool loopBreak = false;
        res = syntaxExpressionA(curTokenType, i, expStack, loopBreak);
        if(res != 0)
            return res;
        else if(loopBreak)
            break;

        loopBreak = false;
        res = syntaxExpressionB(tokenListSize, curTokenType, i, expStack, loopBreak);
        if(res != 0)
            return res;
        else if(loopBreak)
            break;

        loopBreak = false;
        res = syntaxExpressionC(curTokenType, i, expStack, loopBreak);
        if(res != 0)
            return res;
        else if(loopBreak)
            break;

		i++;
	}
	res = mergeStackOperator(expStack, MAX_PRECEDENCE);//向前合并所有栈中的运算表达
	if (res < 0)
		return res;
	if (expStack.size() > 1)//表达式栈元素大于1表示未能处理好该表达式
		return -ErrorExpressionRecognitionFailed;
	else if (expStack.size() == 1 && expStack[0]->type == Token::Bracket && expStack[0]->childExpressions.size() == 0)
		return -ErrorExpressionBracketCanNotBeEmpty;
	else if (expStack.size() == 1 && expStack[0]->type == Token::Subscript && expStack[0]->childExpressions.size() == 0)
		return -ErrorExpressionSubscriptCanNotBeEmpty;
	else if(expStack.size() == 1)
		parent->innerExpressions.push_back(expStack[0]);
	tokenIndex = i;
	return 1;
}


int ILCCodeParser::syntaxExpressionA(Token::Type &curTokenType, int i, vector<Expression*> &expStack, bool& loopBreak) const
{
    curTokenType = tokenList[i].type;
	if (expStack.size() == 1 &&
         //如果已经形成完整表达式，并且后面是左大括号
		((expStack[0]->isExpression && curTokenType == Token::BraceLeft) ||
		//comment = true;
        //(!expStack[0]->isExpression && expStack[0]->type != Token::TypeConvert &&
        //curTokenType == Token::BraceLeft) ||
        //如果前面不是表达式，也不是类型转换，并且后面是左大括号，表示语句结束
		(!expStack[0]->isExpression && curTokenType == Token::SubscriptLeft && 
        expStack[0]->type != Token::BraceLeft && expStack[0]->type != Token::Comma))) //如果遇到左中括号//，并且前面不是左大括号，也不是逗号，则表示语句结束
	{
		loopBreak = true;
        return 0;
	}
	if (curTokenType == Token::Comma) //如果遇到逗号，先尝试合并，如果前面已经形成完整表达式，表示语句结束
	{
		int ret = mergeStackOperator(expStack, MAX_PRECEDENCE);//向前合并所有栈中的运算表达
		if (ret < 0)
			return ret;
		if (expStack.size() == 1)
		{
		    loopBreak = true;
            return 0;
		}
	}
	if (!isExpressionToken(curTokenType))
	{
		loopBreak = true;
	}
    return 0;
}

int ILCCodeParser::syntaxExpressionB(int tokenListSize, Token::Type &curTokenType, int &i, vector<Expression*> &expStack, bool& loopBreak)
{
    loopBreak = false;
	//处理大括号组合表达中指定结构体元素的情况
	if (i + CONST_NUM_2 < tokenListSize &&
		curTokenType == Token::Point &&
		tokenList[i + 1].type == Token::Member &&
		tokenList[i + CONST_NUM_2].type == Token::Assign &&
		expressionStackIsInBrace(expStack, expStack.size() - 1)) //如果大括号中出现了 .member= 则表示一个结构体组合元素出现了
	{
		Expression* expCompoundElementIndex = createExpression(Token::CompoundElementIndex,
            "." + tokenList[i + 1].value + "=", false);
		expStack.push_back(expCompoundElementIndex);
		i += CONST_NUM_3;
		curTokenType = tokenList[i].type;
	}
	else if (curTokenType == Token::SubscriptLeft && 
		expressionStackIsInBrace(expStack, expStack.size() - 1))//如果大括号中出现了 [ 则表示一个数组组合元素出现了
	{
		//吸收 [] 里面的表达式
		Statement tempStmt;
		int tempTokenIndex = i + 1;
		int res = syntaxExpression(tempTokenIndex, &tempStmt);
		if (res < 0) 
			return res;
		if (tempStmt.innerExpressions.size() != 1)
			return -ErrorExpressionCompoundElementIndexCanNotBeEmpty;
		if (!(tempTokenIndex + 1 < tokenList.size() && 
			tokenList[tempTokenIndex].type == Token::SubscriptRight && 
			tokenList[tempTokenIndex+1].type == Token::Assign))
			return -ErrorExpressionCompoundElementIndexInvalid;

		Expression* expCompoundElementIndex = createExpression(Token::CompoundElementIndex,
            "[" + tempStmt.innerExpressions[0]->expressionStr + "]=", false);
		expStack.push_back(expCompoundElementIndex);
		i = tempTokenIndex + CONST_NUM_2;
		curTokenType = tokenList[i].type;
	}
    return 0;
}

int ILCCodeParser::syntaxExpressionC(Token::Type curTokenType, int i, vector<Expression*> &expStack, bool& loopBreak)
{
    int res = 0;
	if (curTokenType >= Token::Number && curTokenType <= Token::Member || 
        curTokenType == Token::DataType ||curTokenType == Token::BraceLeft || 
        curTokenType == Token::BracketLeft) { //curTokenType == Token::SubscriptLeft || 为避免a.b[1]先算b[1]注释掉了SubscriptLeft
		Expression* exp = createExpression(curTokenType, tokenList[i].value, 
                                           curTokenType >= Token::Number && curTokenType <= Token::Member);
		expStack.push_back(exp);
        return 0;
	} // else {
	
	int precedence = getOperatorPrecedence(curTokenType);
    //排除自增自减的优先级冲突(   ++a++;  )，应先计算右面的自增
	if (!(expStack.size() >= CONST_NUM_2 &&  
		(curTokenType == Token::SelfAdd && expStack[expStack.size() - CONST_NUM_2]->type == Token::SelfAdd ||
		curTokenType == Token::SelfSub && expStack[expStack.size() - CONST_NUM_2]->type == Token::SelfSub))){
		if((res = mergeStackOperator(expStack, 
                                     (curTokenType == Token::BracketRight ||  curTokenType == Token::SubscriptRight || 
                                      curTokenType == Token::BraceRight) ? MAX_PRECEDENCE : precedence)) < 0) {
            return res;
        }
	}
	if (expStack.size() == 1 && 
         //如果前面是左括号，后面是右括号，则继续，因为存在函数参数为空的情况
        (expStack[0]->type != Token::BracketLeft && curTokenType == Token::BracketRight ||	   
        //如果前面是左中括号，后面是右中括号，则继续，因为存在数组类型定义的情况
		expStack[0]->type != Token::SubscriptLeft && curTokenType == Token::SubscriptRight ||
        //如果已经形成完整表达式，并且后面是冒号，表示语句结束
		curTokenType == Token::Select) ||     
		(expStack.size() == 0 && (curTokenType == Token::BracketRight ||
        //如果已经形成完整表达式，并且后面是右括号，表示语句结束
        //如果已经形成完整表达式，并且后面是右中括号，表示语句结束
		curTokenType == Token::SubscriptRight))){
		loopBreak = true;
        return 0;
	}
    Expression* exp = createExpression(curTokenType, tokenList[i].value, false);
	expStack.push_back(exp);
	if (precedence == 1 && isMonocularOperator(curTokenType)){//对于优先级最高的运算符，直接合并
		if((res = mergeStackOperator(expStack, 1)) < 0){
			return res;
		}
	}
	if (expStack.size() >= CONST_NUM_2 && expStack[expStack.size() - CONST_NUM_2]->type == Token::DataType) {
		Token::Type type1 = expStack[expStack.size() - 1]->type; // 处理数组、指针、引用类型
		if(type1 == Token::Subscript || type1 == Token::Mul || type1 == Token::BitAnd){
			mergeStackOperatorRightOne(expStack, expStack.size() - CONST_NUM_2);
        }
	} else if (expStack.size() >= CONST_NUM_2 && expStack[expStack.size() - CONST_NUM_2]->type == Token::Function &&
             expStack[expStack.size() - 1]->type == Token::Bracket) { // 处理函数调用
		mergeStackOperatorRightOne(expStack, expStack.size() - CONST_NUM_2);
	}
    return 0;
}

int ILCCodeParser::mergeStackOperator(vector<Expression*>& expStack, int maxPrecedence) const
{
	if (expStack.empty())
		return 1;
	while (expStack.size() > 1)
	{
		bool merged = false;
		for (int i = expStack.size() - 1; i >= 0; i--)
		{
			if (i < expStack.size() - CONST_NUM_2)
				return 0;
			Token::Type type = expStack[i]->type;
			if (!isOperator(type) || expStack[i]->isExpression)
				continue;
			else if (getOperatorPrecedence(expStack[i]->type) > maxPrecedence) {
				break;
            }else if(type == Token::SizeOf && i < expStack.size() - 1 && expStack[i + 1]->type != Token::Bracket && expStack[i + 1]->type != Token::TypeConvert) {
                break;//如果是sizeof，并且sizeof右侧不是完整的()的话则跳过，比如 sizeof，( 时
            }

            //由于前面初步分析负号的时候，不能完全分析出来是负号还是减号，这里通过符号前面是否为表达式判断
            if(i > 0 && expStack[i - 1]->isExpression){
                if(type == Token::Negative) {
                    expStack[i]->type = Token::Sub;
                } else if(type == Token::Positive) {
                    expStack[i]->type = Token::Add;
                } else if(type == Token::Indirection) {
                    expStack[i]->type = Token::And;
                } else if(type == Token::AddressOf) {
                    expStack[i]->type = Token::Mul;
                }
                type = expStack[i]->type;
            }
			bool isMonocularOperatorT = isMonocularOperator(type);
			bool isLeftCalculateOperatorT = isLeftCalculateOperator(type);
			bool isRightCalculateOperatorT = isRightCalculateOperator(type);
			bool isBinocularOperatorT = isBinocularOperator(type);
            
			bool loopBreak = false;
            int res = mergeStackOperatorA(i, expStack,isMonocularOperatorT,
                                          isLeftCalculateOperatorT,isRightCalculateOperatorT,
                                          isBinocularOperatorT, merged, loopBreak);

		    if(res != 0)
                return res;
            else if(loopBreak)
                break;
			//else
			//{
			//	return -ErrorExpressionCanNotFindOperator;
			//}
		}
		if (!merged)
			break;
	}
	return 1;
}

int ILCCodeParser::mergeStackOperatorA(int i, vector<Expression*>& expStack, bool isMonocularOperatorT, bool isLeftCalculateOperatorT, bool isRightCalculateOperatorT, bool isBinocularOperatorT, bool &merged, bool& loopBreak) const
{
    int res;
    if (i == expStack.size() - 1){//操作符在最右面
		if (!isMonocularOperatorT){//如果不是单目运算符
			return CONST_NUM_2;
		}
        //如果是单目运算符
        if (isLeftCalculateOperatorT){//如果是先左计算的
			res = mergeStackOperatorLeftOne(expStack, i);
            if (res < 0){
		        return res;
            } else if (res == 1) {
                merged = true;
            }
	        loopBreak = true;
            return 0;
		} else if (isRightCalculateOperatorT && expStack.size() >= CONST_NUM_2 &&
            expStack[i]->type == Token::TypeConvert &&
            expStack[i - 1]->type == Token::SizeOf) { // 如果是先右计算的
			return 0; // 考虑到可能是 sizeof + TypeConvert 的情况，暂时不处理
		} else if (isRightCalculateOperatorT) { // 如果是先右计算的
			return CONST_NUM_2;
		} else {//其它不确定情况
			return -ErrorExpressionUncertain;
		}
	} else if(i != expStack.size() - CONST_NUM_2) {
        return 0;
    }
    //操作符在倒数第二位
	if (isMonocularOperatorT && !isBinocularOperatorT &&
        (expStack[i + 1]->isExpression || expStack[i + 1]->type == Token::TypeConvert) || //如果是单目运算符,同时不是双目运算符
		isMonocularOperatorT && isBinocularOperatorT && 
        !(i-1>=0 && expStack[i - 1]->isExpression && expStack[i + 1]->isExpression)){
		//如果是单目运算符,同时也是双目运算符 + - * & //且符号两边不完全是表达式
		if (isLeftCalculateOperatorT && i - 1 >= 0 && expStack[i - 1]->isExpression) {
		    //如果是先左计算的,并且左边是表达式, 才能进行左合并(避免++ --运算符匹配错误)
			res = mergeStackOperatorLeftOne(expStack, i);
		} else if (isRightCalculateOperatorT) {
			res = mergeStackOperatorRightOne(expStack, i);
		} else {
			return -ErrorExpressionUncertain;
		}
	} else if (isBinocularOperatorT) {
		res = mergeStackOperatorBothSides(expStack, i);
	} else {
		return 0; //-ErrorExpressionUncertain;
	}
    if (res < 0){
		return res;
    } else if (res == 1) {
        merged = true;
    }
	loopBreak = true;
    return 0;
}

int ILCCodeParser::mergeStackOperatorLeftOne(vector<Expression*>& expStack, int expIndex) const
{
	if (expIndex - 1 < 0)
		return -ErrorExpressionInternalError;
	int i = expIndex;
	Token::Type type = expStack[i]->type;
	
	if (type == Token::BracketRight)//处理右括号的情况
	{
        return mergeStackOperatorLeftOneA(expStack, i);
	}
	else if (type == Token::SubscriptRight)
	{
        return mergeStackOperatorLeftOneB(expStack, i);
	}
	else if (type == Token::Subscript)
	{
        return mergeStackOperatorLeftOneC(expStack, i);
	}
	else if (type == Token::BraceRight)
	{
        return mergeStackOperatorLeftOneD(expStack, i);
	}
	else// if (i - 1 >= 0)//符号的左边还有内容
	{
		if (!expStack[i - 1]->isExpression)//符号左边必须是表达式
		{
			if(type == Token::SelfAdd || type == Token::SelfSub ||
                type == Token::Mul || type == Token::BitAnd ||
                type == Token::Add || type == Token::Sub ||
                type == Token::Positive || type == Token::Negative ||
                type == Token::AddressOf || type == Token::Indirection)
				return 0;
			else
				return -ErrorExpressionInvalidLValue;
		}

		expStack[i]->childExpressions.push_back(expStack[i - 1]);
		expStack[i - 1]->parentExpression = expStack[i];
		expStack[i]->expressionStr = expStack[i-1]->expressionStr + expStack[i]->expressionStr;
		expStack[i]->isExpression = true;
		expStack.erase(expStack.begin() + i - 1);
		return 1;
	}
	//else
	//{
	//	return -ErrorExpressionMissingLValue;
	//}
}

int ILCCodeParser::mergeStackOperatorLeftOneA(vector<Expression*>& expStack, int i) const
{
	if(i - CONST_NUM_2 >=0 && expStack[i - CONST_NUM_2]->type == Token::BracketLeft) {
		Token::Type type1 = expStack[i - 1]->type;
		if (type1 == Token::Subscript){ //括号里不能仅仅是中括号
			return -ErrorExpressionInvalidValueInBracket;
        } else if (type1 == Token::DataType || (type1 == Token::TypeConvert && !expStack[i - 1]->isExpression)) {
			expStack[i - CONST_NUM_2]->type = Token::TypeConvert;
			expStack[i - CONST_NUM_2]->childExpressions.push_back(expStack[i - 1]);
			expStack[i - 1]->parentExpression = expStack[i - CONST_NUM_2];
			expStack[i - CONST_NUM_2]->isExpression = false;
			expStack[i - CONST_NUM_2]->expressionStr = "(" + expStack[i - 1]->expressionStr + ")";
			expStack.erase(expStack.begin() + i - 1, expStack.begin() + i + 1);
			return 1;
		}
	}
	int bracketStart = -1;
	for (int j = i - 1; j >= 0; j--) {
		if (expStack[j]->type == Token::BracketLeft) {
			bracketStart = j;
			break;
		}
	}
	if (bracketStart == -1){//保证有左边的括号
		return -ErrorExpressionLeftBraceUnmatched;
    } else if (i - bracketStart > 1 && (i - bracketStart) % CONST_NUM_2 != 0){
		return -ErrorExpressionInvalidValueInBracket;
    }
	for (int j = bracketStart + CONST_NUM_2; j < i; j += CONST_NUM_2) { //保证括号内部是用逗号分隔的
        if (expStack[j]->type != Token::Comma) {
		    return -ErrorExpressionInvalidValueInBracket;
        }
	}
	for (int j = bracketStart + 1; j < i; j += CONST_NUM_2){//保证大括号内部逗号分隔的都是表达式
		if (!expStack[j]->isExpression){
			return -ErrorExpressionInvalidValueInBracket;
        }
	}
	//合并括号表达式
	expStack[bracketStart]->type = Token::Bracket;
	expStack[bracketStart]->isExpression = true;
	expStack[bracketStart]->expressionStr = "(";
	for (int j = bracketStart + 1; j < i; j += CONST_NUM_2) {
		expStack[bracketStart]->expressionStr += expStack[j]->expressionStr;
		expStack[bracketStart]->expressionStr += ((i - j > CONST_NUM_2) ? "," : "");
		expStack[bracketStart]->childExpressions.push_back(expStack[j]);
		expStack[j]->parentExpression = expStack[bracketStart];
	}
	expStack[bracketStart]->expressionStr += ")";
	expStack.erase(expStack.begin() + bracketStart + 1, expStack.begin() + i + 1);
	return 1;
}

int ILCCodeParser::mergeStackOperatorLeftOneB(vector<Expression*>& expStack, int i) const
{
	if (expStack[i - 1]->type == Token::SubscriptLeft)
	{
		expStack[i - 1]->type = Token::Subscript;
		expStack[i - 1]->isExpression = false;
		expStack[i - 1]->expressionStr = "[]";
		expStack.erase(expStack.begin() + i);
		return 1;
	}
	else if (i - CONST_NUM_2 >= 0 && expStack[i - CONST_NUM_2]->type == Token::SubscriptLeft)
	{
		Token::Type type1 = expStack[i - 1]->type;
		if (!expStack[i - 1]->isExpression) //中括号里必须是表达式
			return -ErrorExpressionInvalidValueInSubscript;
        
		expStack[i - CONST_NUM_2]->type = Token::Subscript;
		expStack[i - CONST_NUM_2]->childExpressions.push_back(expStack[i - 1]);
		expStack[i - 1]->parentExpression = expStack[i - CONST_NUM_2];
		expStack[i - CONST_NUM_2]->isExpression = false;
		expStack[i - CONST_NUM_2]->expressionStr = "[" + expStack[i - 1]->expressionStr + "]";
		expStack.erase(expStack.begin() + i - 1, expStack.begin() + i + 1);
		return 1;
	}
	else
	{
		return -ErrorExpressionLeftSubscriptUnmatched;
	}
}

int ILCCodeParser::mergeStackOperatorLeftOneC(vector<Expression*>& expStack, int i) const
{
	if (i - 1 >= 0 && expStack[i - 1]->isExpression)
	{
		Expression* arrayExp = expStack[i - 1];
		Expression* indexExp = expStack[i];

		Expression* arrayIndexExp = createExpression(Token::ArrayIndex, 
            arrayExp->expressionStr + indexExp->expressionStr,true);
		arrayIndexExp->childExpressions.push_back(arrayExp);
		arrayExp->parentExpression = arrayIndexExp;
		arrayIndexExp->childExpressions.push_back(indexExp);
		indexExp->parentExpression = arrayIndexExp;

		expStack[i - 1] = arrayIndexExp;
		expStack.erase(expStack.begin() + i, expStack.begin() + i + 1);
		return 1;
	}
	else if(i - 1 >= 0 && expStack[i - 1]->type == Token::DataType)
	{
		return 0;
	}
	else
	{
		return -ErrorExpressionMissingValueOfSubscript;
	}
}

int ILCCodeParser::mergeStackOperatorLeftOneD(vector<Expression*>& expStack, int i) const
{
	int braceStart = -1;
	for (int j = i - 1; j >= 0; j--)
	{
		if (expStack[j]->type == Token::BraceLeft)
		{
			braceStart = j;
			break;
		}
	}
	if (braceStart == -1)//保证有左边的大括号
		return -ErrorExpressionLeftBraceUnmatched;
	if (i - braceStart == 1)//保证大括号内部有数据
		return -ErrorExpressionInvalidValueInBrace;
	for (int j = braceStart + CONST_NUM_2; j < i; j+=CONST_NUM_2)//保证大括号内部是用逗号分隔的
	{
		if (expStack[j]->type != Token::Comma)
			return -ErrorExpressionInvalidValueInBrace;
	}
	for (int j = braceStart + 1; j < i; j += CONST_NUM_2)//保证大括号内部逗号分隔的都是表达式
	{
		if (!expStack[j]->isExpression)
			return -ErrorExpressionInvalidValueInBrace;
	}
	//合并大括号表达式
	expStack[braceStart]->type = Token::Compound;
	expStack[braceStart]->isExpression = true;
	expStack[braceStart]->expressionStr = "{";
	for (int j = braceStart + 1; j < i; j += CONST_NUM_2)
	{
		expStack[braceStart]->expressionStr += expStack[j]->expressionStr;
		if(i - j > CONST_NUM_2)
			expStack[braceStart]->expressionStr += ",";
		expStack[braceStart]->childExpressions.push_back(expStack[j]);
		expStack[j]->parentExpression = expStack[braceStart];
	}
	expStack[braceStart]->expressionStr += "}";
	expStack.erase(expStack.begin() + braceStart + 1, expStack.begin() + i + 1);
	return 1;
}


int ILCCodeParser::mergeStackOperatorRightOne(vector<Expression*>& expStack, int expIndex) const
{
	if (expIndex >= expStack.size() - 1)
		return -ErrorExpressionInternalError;

	int i = expIndex;
	Token::Type type = expStack[i]->type;
	if (type == Token::TypeConvert)
	{
		return mergeStackOperatorRightOneA(expStack, i);
	}
	else if (type == Token::DataType)
	{
		return mergeStackOperatorRightOneB(expStack, i);
	}
	else if (type == Token::Function)
	{
		return mergeStackOperatorRightOneC(expStack, i);
	}
	else if (type == Token::SizeOf)
	{
		return mergeStackOperatorRightOneD(expStack, i);
	}
	else if (type == Token::CompoundElementIndex)
	{
        return mergeStackOperatorRightOneE(expStack, i);
	}
	else
	{
		if(!expStack[i + 1]->isExpression)
			return -ErrorExpressionInvalidRValue;
		if (expStack[i]->type == Token::SelfAdd) expStack[i]->type = Token::SelfAddLeft;
		if (expStack[i]->type == Token::SelfSub) expStack[i]->type = Token::SelfSubLeft;
		if (expStack[i]->type == Token::Mul) expStack[i]->type = Token::Indirection;
		if (expStack[i]->type == Token::BitAnd) expStack[i]->type = Token::AddressOf;

		expStack[i]->isExpression = true;
		expStack[i]->expressionStr += expStack[i + 1]->expressionStr;
		expStack[i]->childExpressions.push_back(expStack[i + 1]);
		expStack[i + 1]->parentExpression = expStack[i];
		expStack.erase(expStack.begin() + i + 1);
		return 1;
	}
}

int ILCCodeParser::mergeStackOperatorRightOneA(vector<Expression*>& expStack, int i) const
{
    Token::Type type1 = expStack[i + 1]->type;
	if (type1 == Token::Compound)
	{
		expStack[i]->isExpression = true;
		expStack[i]->expressionStr += expStack[i + 1]->expressionStr;
		expStack[i]->childExpressions.push_back(expStack[i + 1]);
		expStack[i + 1]->parentExpression = expStack[i];
		expStack.erase(expStack.begin() + i + 1);
		return 1;
	}
	/*else if (type1 == Token::Subscript)
	{
		expStack[i+1]->type = Token::DataType;
		expStack[i+1]->isExpression = false;
		expStack[i+1]->expressionStr = expStack[i]->expressionStr + expStack[i + 1]->expressionStr;
		expStack[i+1]->childExpressions.push_back(expStack[i]);
		expStack[i]->parentExpression = expStack[i+1];
		expStack.erase(expStack.begin() + i);
		return 1;
	}*/
	else if (expStack[i + 1]->isExpression)
	{
		expStack[i]->isExpression = true;
		expStack[i]->expressionStr += expStack[i + 1]->expressionStr;
		expStack[i]->childExpressions.push_back(expStack[i + 1]);
		expStack[i + 1]->parentExpression = expStack[i];
		expStack.erase(expStack.begin() + i + 1);
		return 1;
	}
	else
	{
		return -ErrorExpressionInvalidRValue;
	}
}

int ILCCodeParser::mergeStackOperatorRightOneB(vector<Expression*>& expStack, int i) const
{
    Token::Type type1 = expStack[i + 1]->type;
	if (type1 == Token::Subscript || type1 == Token::Mul || type1 == Token::BitAnd)
	{
		expStack[i+1]->type = Token::DataType;
		expStack[i+1]->isExpression = false;
		expStack[i+1]->expressionStr = expStack[i]->expressionStr + expStack[i + 1]->expressionStr;
		expStack[i+1]->childExpressions.push_back(expStack[i]);
		expStack[i]->parentExpression = expStack[i+1];
		expStack.erase(expStack.begin() + i);
		return 1;
	}
	else
	{
		return -ErrorExpressionInvalidRValue;
	}
}

int ILCCodeParser::mergeStackOperatorRightOneC(vector<Expression*>& expStack, int i) const
{
    Token::Type type1 = expStack[i + 1]->type;
	if (type1 == Token::Bracket)
	{
		expStack[i]->isExpression = true;
		expStack[i]->expressionStr += expStack[i + 1]->expressionStr;
		expStack[i]->childExpressions.push_back(expStack[i + 1]);
		expStack[i + 1]->parentExpression = expStack[i];
		expStack.erase(expStack.begin() + i + 1);
		return 1;
	}
	else
	{
		return -ErrorExpressionInvalidRValue;
	}
}

int ILCCodeParser::mergeStackOperatorRightOneD(vector<Expression*>& expStack, int i) const
{
    Token::Type type1 = expStack[i + 1]->type;
	if (type1 == Token::TypeConvert)
	{
		expStack[i]->isExpression = true;
		expStack[i]->expressionStr += expStack[i + 1]->expressionStr;
		expStack[i]->childExpressions.push_back(expStack[i + 1]);
		expStack[i + 1]->parentExpression = expStack[i];
		expStack.erase(expStack.begin() + i + 1);
		return 1;
	}
	else if (type1 == Token::Bracket)
	{
		expStack[i]->isExpression = true;
		expStack[i]->expressionStr += expStack[i + 1]->expressionStr;
		expStack[i]->childExpressions.push_back(expStack[i + 1]);
		expStack[i + 1]->parentExpression = expStack[i];
		expStack.erase(expStack.begin() + i + 1);
		return 1;
	}
	else
	{
		return -ErrorExpressionInvalidRValue;
	}
}

int ILCCodeParser::mergeStackOperatorRightOneE(vector<Expression*>& expStack, int i) const
{
	Token::Type type1 = expStack[i + 1]->type;
	if (expStack[i + 1]->isExpression)
	{
		Expression* compoundElement = createExpression(Token::CompoundElement,
            expStack[i]->expressionStr + expStack[i + 1]->expressionStr, true);
		compoundElement->childExpressions.push_back(expStack[i]);
		expStack[i]->parentExpression = compoundElement;
		compoundElement->childExpressions.push_back(expStack[i + 1]);
		expStack[i + 1]->parentExpression = compoundElement;
		expStack.erase(expStack.begin() + i, expStack.begin() + i + CONST_NUM_2);
		expStack.insert(expStack.begin() + i, compoundElement);
		return 1;
	}
	else
	{
		return -ErrorExpressionInvalidRValue;
	}
}

int ILCCodeParser::mergeStackOperatorBothSides(vector<Expression*>& expStack, int expIndex) const
{
	if (expIndex >= expStack.size() - 1 || expIndex - 1 < 0)
		return -ErrorExpressionInternalError;

	int i = expIndex;
	Token::Type type = expStack[i]->type;
	if (type == Token::Condition)
	{
		Token::Type type1 = expStack[i + 1]->type;
		if (type1 == Token::Select)
		{
			expStack[i]->type = Token::TernaryConditional;
			expStack[i]->isExpression = true;
			expStack[i]->expressionStr = expStack[i - 1]->expressionStr + "?" + expStack[i + 1]->expressionStr;
			expStack[i]->childExpressions.push_back(expStack[i - 1]);
			expStack[i - 1]->parentExpression = expStack[i];
			expStack[i]->childExpressions.push_back(expStack[i + 1]);
			expStack[i + 1]->parentExpression = expStack[i];
			expStack.erase(expStack.begin() + i + 1);
			expStack.erase(expStack.begin() + i - 1);
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (!expStack[i + 1]->isExpression)
			return 0;
		if (!expStack[i - 1]->isExpression)
			return -ErrorExpressionInvalidLValue;

		expStack[i]->isExpression = true;
		expStack[i]->expressionStr = expStack[i - 1]->expressionStr + expStack[i]->expressionStr + expStack[i + 1]->expressionStr;
		expStack[i]->childExpressions.push_back(expStack[i - 1]);
		expStack[i - 1]->parentExpression = expStack[i];
		expStack[i]->childExpressions.push_back(expStack[i + 1]);
		expStack[i + 1]->parentExpression = expStack[i];
		expStack.erase(expStack.begin() + i + 1);
		expStack.erase(expStack.begin() + i - 1);
		return 1;
	}
}

bool ILCCodeParser::expressionStackIsInBrace(const vector<Expression*>& expStack, int expIndex) const
{
	for (int i = expIndex; i >= 0; i--)
	{
		if (expStack[i]->type == Token::BracketLeft)
			return false;
		if (expStack[i]->type == Token::BraceLeft)
			return true;
	}
	return false;
}


string ILCCodeParser::getMultiTokenValue(int index, int count)
{
	if (index < 0 || count <= 0 || index + count > tokenList.size())
		return "";
	string ret;
	for (int i = index; i < index + count; i++)
	{
		if (i != index + count - 1)
			ret += tokenList[i].value + " ";
		else
			ret += tokenList[i].value;
	}
	return ret;
}
namespace
{
    const int GETOPERATORPRECEDENCE_P1 = 1;
    const int GETOPERATORPRECEDENCE_P2 = 2;
    const int GETOPERATORPRECEDENCE_P3 = 3;
    const int GETOPERATORPRECEDENCE_P4 = 4;
    const int GETOPERATORPRECEDENCE_P5 = 5;
    const int GETOPERATORPRECEDENCE_P6 = 6;
    const int GETOPERATORPRECEDENCE_P7 = 7;
    const int GETOPERATORPRECEDENCE_P8 = 8;
    const int GETOPERATORPRECEDENCE_P9 = 9;
    const int GETOPERATORPRECEDENCE_P10 = 10;
    const int GETOPERATORPRECEDENCE_P11 = 11;
    const int GETOPERATORPRECEDENCE_P12 = 12;
    const int GETOPERATORPRECEDENCE_P13 = 13;
    const int GETOPERATORPRECEDENCE_P14 = 14;
    const int GETOPERATORPRECEDENCE_P15 = 15;
}
int ILCCodeParser::getOperatorPrecedence(Token::Type type)
{
	if (type >= Token::SelfAdd && type <= Token::Compound || type == Token::BraceRight || type == Token::Subscript || type == Token::Function || type == Token::Bracket) return GETOPERATORPRECEDENCE_P1;
	if (type >= Token::SelfAddLeft && type <= Token::SizeOf) return GETOPERATORPRECEDENCE_P2;
	if (type >= Token::Mul && type <= Token::Mod) return GETOPERATORPRECEDENCE_P3;
	if (type == Token::Add || type == Token::Sub) return GETOPERATORPRECEDENCE_P4;
	if (type == Token::LeftShift || type == Token::RightShift) return GETOPERATORPRECEDENCE_P5;
	if (type >= Token::Less && type <= Token::GreaterEqual) return GETOPERATORPRECEDENCE_P6;
	if (type == Token::Equal || type == Token::NotEqual) return GETOPERATORPRECEDENCE_P7;
	if (type == Token::BitAnd) return GETOPERATORPRECEDENCE_P8;
	if (type == Token::BitXor) return GETOPERATORPRECEDENCE_P9;
	if (type == Token::BitOr) return GETOPERATORPRECEDENCE_P10;
	if (type == Token::And) return GETOPERATORPRECEDENCE_P11;
	if (type == Token::Or) return GETOPERATORPRECEDENCE_P12;
	if (type == Token::TernaryConditional || type == Token::Condition || type == Token::Select) return GETOPERATORPRECEDENCE_P13;
	if (type >= Token::Assign && type <= Token::OrAssign || type == Token::CompoundElementIndex) return GETOPERATORPRECEDENCE_P14;
	if (type == Token::Comma) return GETOPERATORPRECEDENCE_P15;
	return -1;
}

bool ILCCodeParser::isExpressionToken(Token::Type type) const
{
	if (type <= Token::Member || type >= Token::Condition && type <= Token::BraceRight || type == Token::DataType) return true;
	return false;
}

bool ILCCodeParser::isMonocularOperator(Token::Type type) const
{
	if (type == Token::SelfAdd || 
		type == Token::SelfSub ||
		type == Token::Mul ||
		type == Token::BitAnd ||
		type == Token::SelfSub ||
		type == Token::BracketRight ||
		type == Token::Subscript ||
		type == Token::SubscriptRight ||
		type == Token::BraceRight ||
		type >= Token::SelfAddLeft && type <= Token::SizeOf ||
		type == Token::CompoundElementIndex)
		return true;
	return false;
}

bool ILCCodeParser::isBinocularOperator(Token::Type type) const
{
	if (type == Token::Point ||
		type == Token::Arrow ||
		type >= Token::Mul && type <= Token::Or ||
		type >= Token::Assign && type <= Token::OrAssign ||
		type == Token::Condition || type == Token::Select)
		return true;
	return false;
}

bool ILCCodeParser::isLeftCalculateOperator(Token::Type type) const
{
	if (type == Token::SelfAdd || type == Token::SelfSub ||
		type >= Token::Point && type <= Token::Compound ||
		type == Token::BracketRight ||
		type == Token::Subscript ||
		type == Token::SubscriptRight ||
		type == Token::BraceRight ||
		type >= Token::Mul && type <= Token::TernaryConditional ||
		type == Token::Condition || type == Token::Select ||
		type == Token::Comma)
		return true;
	return false;
}

bool ILCCodeParser::isRightCalculateOperator(Token::Type type) const
{
	if (type == Token::SelfAdd || type == Token::SelfSub ||
		type >= Token::SelfAddLeft && type <= Token::SizeOf ||
		type == Token::Mul ||
		type == Token::BitAnd ||
		type >= Token::TernaryConditional && type <= Token::OrAssign ||
		type == Token::CompoundElementIndex)
		return true;
	return false;
}

bool ILCCodeParser::isOperator(Token::Type type) const
{
	if (type == Token::SelfAdd || type == Token::SelfSub ||
		type == Token::BracketRight || type == Token::SubscriptRight ||
		type >= Token::Point && type <= Token::OrAssign ||
		type == Token::Condition || type == Token::Select ||
		type == Token::BraceRight ||
		type == Token::Subscript ||
		type == Token::CompoundElementIndex)
		return true;
	return false;
}


bool ILCCodeParser::isAssignOperator(Token::Type type)
{
	if(type == Token::SelfAdd ||
	type == Token::SelfSub ||
	type == Token::SelfAddLeft ||
	type == Token::SelfSubLeft ||
	type == Token::Assign ||
	type == Token::AddAssign ||
	type == Token::SubAssign ||
	type == Token::MulAssign ||
	type == Token::DivAssign ||
	type == Token::ModAssign ||
	type == Token::LeftShiftAssign ||
	type == Token::RightShiftAssign ||
	type == Token::AndAssign ||
	type == Token::XorAssign ||
	type == Token::OrAssign)
		return true;
    return false;
}

std::string ILCCodeParser::getErrorStr(int errorCode) const
{
    if(errorCode >= 0 && errorCode < errorStrList.size())
    {
		return errorStrList[errorCode];
    }
    return "Unknow error code.";
}

std::string ILCCodeParser::getErrorStr() const
{
    string ret = "ILCCodeParser:\nError Code: " + to_string(errorCode) + "\n";
    
	ret += "\n";

	int errorCodeIndex = -errorCode - ILCCodeParser::ErrorCommentTailNotFound;

	if(errorCodeIndex >= 0 && errorCodeIndex < errorStrList.size())
    {
		ret += errorStrList[errorCodeIndex];
    }
	else
    {
		ret += "Unknow error code.";
    }

	return ret;
}


Statement* ILCCodeParser::createStatement(Statement::Type type, Statement* parent)
{
	Statement* ret = nullptr;

	if (type == Statement::If) {
		ret = new StmtIf();
	}
	else if (type == Statement::Else) {
		ret = new StmtElse();
	}
	else if (type == Statement::ElseIf) {
		ret = new StmtElseIf();
	}
	else if (type == Statement::While) {
		ret = new StmtWhile();
	}
	else if (type == Statement::DoWhile) {
		ret = new StmtDoWhile();
	}
	else if (type == Statement::For) {
		ret = new StmtFor();
	}
	else if (type == Statement::Continue) {
		ret = new StmtContinue();
	}
	else if (type == Statement::Switch) {
		ret = new StmtSwitch();
	}
	else if (type == Statement::Case) {
		ret = new StmtCase();
	}
	else if (type == Statement::Default) {
		ret = new StmtDefault();
	}
	else if (type == Statement::Break) {
		ret = new StmtBreak();
	}
	else if (type == Statement::Return) {
		ret = new StmtReturn();
	}
	else if (type == Statement::BraceStatement) {
		ret = new Statement();
	}
	else if (type == Statement::LocalVariable) {
		ret = new StmtLocalVariable();
	}
	else if (type == Statement::ExpressionWithSemicolon) {
		ret = new StmtExpression();
	}
	else {
		return nullptr;
	}
	ret->parentStatement = parent;
	ret->type = type;
	ret->depth = parent->depth + 1;
	parent->childStatements.push_back(ret);
	return ret;
}


Expression* ILCCodeParser::createExpression(Token::Type type, string expStr, bool isExp)
{
	Expression* ret = new Expression();
	ret->type = type;
	ret->tokenStr = expStr;
	ret->expressionStr = expStr;

	ret->isExpression = isExp;
	return ret;
}
namespace
{
const int SETEXTENDDATATYPE_MIN_STRUCT_TOKEN_SIZE = 7;
}
int ILCCodeParser::setExtendDataType(vector<string> types) const
{
	for (int i = 0; i < types.size(); i++)
	{
		string typeStr = types[i];
		if (typeStr.empty())
			return -ErrorExtendDataTypeInvalid;

		//解析一个数据类型的字符串
		vector<string> tokens;
		vector<bool> tokenIsWord;
        setExtendDataTypeA(typeStr, tokens, tokenIsWord);

		if(tokens.empty())
			return -ErrorExtendDataTypeInvalid;

		if (tokens[0] != "struct")
		{
            int res = setExtendDataTypeB(tokens, tokenIsWord);
			if(res != 0)
                return res;
		}
		else{
			int res = setExtendDataTypeC(tokens, tokenIsWord);
			if(res != 0)
                return res;
		}
	}
	return 1;
}

void ILCCodeParser::setCurrentError(int errorCode)
{
	ILCCodeParser::errorCode = errorCode;
}

void ILCCodeParser::setExtendDataTypeA(const string &typeStr, vector<string> &tokens, vector<bool> &tokenIsWord) const
{
    int charIndex = 0;
	string curToken;
	while (charIndex < typeStr.length())
	{
		char c = typeStr[charIndex];
		if (c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_')
		{
			curToken += c;
		}
		else if(c == ' ' || c == '\n' || c == '\r' || c == '\t')
		{
			if (!curToken.empty())
			{
				tokens.push_back(curToken);
                tokenIsWord.push_back(!(curToken[0] >= '0' && curToken[0] <= '9'));
				curToken = "";
			}
		}
		else
		{
			if (!curToken.empty())
			{
				tokens.push_back(curToken);
                tokenIsWord.push_back(!(curToken[0] >= '0' && curToken[0] <= '9'));
				curToken = "";
			}
			curToken += c;
			tokens.push_back(curToken);
			tokenIsWord.push_back(false);
			curToken = "";
		}
		charIndex++;
	}
	if (!curToken.empty())
	{
		tokens.push_back(curToken);
		if (curToken[0] >= '0' && curToken[0] <= '9')
			tokenIsWord.push_back(false);
		else
			tokenIsWord.push_back(true);
		curToken = "";
	}
}

int ILCCodeParser::setExtendDataTypeB(const vector<string> &tokens, const vector<bool> &tokenIsWord) const
{
    bool invalid = false;
	string curTypeStr;
	for (int tokenIndex = 0; tokenIndex < tokens.size(); tokenIndex++)
	{
		//判断非结构体类型的类型是否合法，可能含有多个token，比如 long long
		if (!tokenIsWord[tokenIndex])
		{
			invalid = true;
			break;
		}
		curTypeStr += tokens[tokenIndex];
		if (tokenIndex != tokens.size() - 1)
		{
			curTypeStr += " ";
		}
	}
	if (!invalid)
	{
		if (std::find(ILParser::allDataTypeList.begin(), ILParser::allDataTypeList.end(),
			curTypeStr) != ILParser::allDataTypeList.end())
			return -ErrorExtendDataTypeRedefined;
		ILParser::allDataTypeList.push_back(curTypeStr);
	}
    return 0;
}

int ILCCodeParser::setExtendDataTypeC(const vector<string> &tokens, const vector<bool> &tokenIsWord) const
{
    int tokenSize = tokens.size();
	if (tokenSize < SETEXTENDDATATYPE_MIN_STRUCT_TOKEN_SIZE){
		return -ErrorExtendDataTypeInvalid;
	}else if (!tokenIsWord[1]){
		return -ErrorExtendDataTypeInvalid;
	}else if (tokens[CONST_NUM_2] != "{"){
		return -ErrorExtendDataTypeInvalid;
    }
	ILStructDefine* newStructType = new ILStructDefine();
	newStructType->name = tokens[1];
	int tokenIndex = CONST_NUM_3;
	while (tokenIndex < tokenSize){
		ILMember* newStructMember = new ILMember();
		newStructType->members.push_back(newStructMember);
        int res = setExtendDataTypeCA(tokens, tokenIsWord, tokenIndex,newStructMember);
		if(res != 0){
            return releaseILObjectAndReturn(newStructType, res);
        }
		//吸收成员名后面的数组大小定义
        bool findArraySize = (tokens[tokenIndex] != ";");
        while (findArraySize) {
			if (tokenIndex + CONST_NUM_3 >= tokenSize ||  //[之后至少应该有三个token，"num ] ;"
                tokens[tokenIndex] != "[" || tokens[tokenIndex + CONST_NUM_2] != "]") {
				return releaseILObjectAndReturn(newStructType, -ErrorExtendDataTypeInvalid);
            }
			newStructMember->arraySize.push_back(stringToInt(tokens[tokenIndex + 1]));
			tokenIndex += CONST_NUM_3;
			if (tokens[tokenIndex] == ";"){
				findArraySize = false;
            }
		}
		tokenIndex++;
		if (tokenIndex >= tokenSize){
			return releaseILObjectAndReturn(newStructType, -ErrorExtendDataTypeInvalid);
        }else if (tokens[tokenIndex] == "}") {
			tokenIndex++;
			break;
		}
	}
	if (tokenIndex == tokenSize || tokenIndex == tokenSize - 1 && tokens[tokenIndex] == ";") {	
		ILParser::structTypeList.push_back(newStructType);
		if (std::find(ILParser::allDataTypeList.begin(), ILParser::allDataTypeList.end(),
			"struct " + newStructType->name) != ILParser::allDataTypeList.end()){
			return releaseILObjectAndReturn(newStructType, -ErrorExtendDataTypeRedefined);
        }
		ILParser::allDataTypeList.push_back("struct " + newStructType->name);
	} else {
		return releaseILObjectAndReturn(newStructType, -ErrorExtendDataTypeInvalid);
    }
    return 0;
}

int ILCCodeParser::setExtendDataTypeCA(const vector<string> &tokens, const vector<bool> &tokenIsWord, int &tokenIndex, ILMember* newStructMember) const
{
    int tokenSize = tokens.size();
    //根据每个分号前的单词个数统计成员数据类型占用的单词个数，为分号前的单词个数减一
	int typeTokenCount = 0;
	for (int tempTokenIndex = tokenIndex; tempTokenIndex < tokenSize; tempTokenIndex++) {
		if (tokens[tempTokenIndex] == ";"){
			break;
		} else if (tokenIsWord[tempTokenIndex]){
			typeTokenCount++;
        }
	}
	typeTokenCount--;
	if (typeTokenCount <= 0){
		return -ErrorExtendDataTypeInvalid;
    }
	//判断成员数据类型是否合法
	for (int tempTokenIndex = tokenIndex; tempTokenIndex < tokenIndex + typeTokenCount; tempTokenIndex++) {
		if (!tokenIsWord[tempTokenIndex]){
			return -ErrorExtendDataTypeInvalid;
        }
		newStructMember->type += tokens[tempTokenIndex];
		if (tempTokenIndex != tokenIndex + typeTokenCount - 1){
			newStructMember->type += " ";
        }
	}
	tokenIndex += typeTokenCount;
	if (tokenIndex >= tokenSize){
		return -ErrorExtendDataTypeInvalid;
    }
	//尝试吸收*号，表示这是一个指针变量
	newStructMember->isAddress = 0;
	while (tokens[tokenIndex] == "*") {
		newStructMember->isAddress++;
		tokenIndex++;
	}
    if (!tokenIsWord[tokenIndex]) {
		return -ErrorExtendDataTypeInvalid;
	}
	//指针之后应该是成员名
	if (!tokenIsWord[tokenIndex]){
		return -ErrorExtendDataTypeInvalid;
    }
	newStructMember->name = tokens[tokenIndex];
	tokenIndex++;
    if (tokenIndex >= tokenSize){
		return -ErrorExtendDataTypeInvalid;
    }
    return 0;
}

bool ILCCodeParser::isEmptyStatement(Statement* stmt) const
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

int ILCCodeParser::translateStatement(Statement* stmt, Statement* parent, bool isNewStmtExpression) const
{
	int ret = 0;
	if (stmt == statementRoot){
		for (int i = 0; i < stmt->childStatements.size(); i++)
		{
			ret = translateStatement(stmt->childStatements[i], parent);
			if (ret < 0) return ret;
		}
	}
	else if (stmt->type == Statement::If && (ret = translateStmtIf(stmt, parent)) != 0) {
	}
	else if (stmt->type == Statement::Else && (ret = translateStmtElse(stmt, parent)) != 0) {
	}
	else if (stmt->type == Statement::ElseIf && (ret = translateStmtElseIf(stmt, parent)) != 0) {
	}
	else if (stmt->type == Statement::While && (ret = translateStmtWhile(stmt, parent)) != 0) {
	}
	else if (stmt->type == Statement::DoWhile && (ret = translateStmtDoWhile(stmt, parent)) != 0) {
	}
	else if (stmt->type == Statement::For && (ret = translateStmtFor(stmt, parent)) != 0) {
	}
	else if (stmt->type == Statement::Continue && (ret = translateStmtContinue(parent)) != 0) {
	}
	else if (stmt->type == Statement::Switch && (ret = translateStmtSwitch(stmt, parent)) != 0) {
	}
	else if (stmt->type == Statement::Case && (ret = translateStmtCase(stmt, parent)) != 0) {
	}
	else if (stmt->type == Statement::Default && (ret = translateStmtDefault(parent)) != 0) {
	}
	else if (stmt->type == Statement::Break && (ret = translateStmtBreak(parent)) != 0) {
	}
	else if (stmt->type == Statement::Return && (ret = translateStmtReturn(stmt, parent)) != 0) {
	}
	else if (stmt->type == Statement::BraceStatement && (ret = translateStmtBraceStatement(stmt, parent)) != 0) {
	}
	else if (stmt->type == Statement::LocalVariable && (ret = translateStmtLocalVariable(stmt, parent)) != 0) {
	}
	else if (stmt->type == Statement::ExpressionWithSemicolon && (ret = translateStmtExpressionWithSemicolon(stmt, parent, isNewStmtExpression)) != 0) {
	}
	else {
		return 1;
	}
    if (ret < 0) 
        return ret;
	return 1;
	
}

int ILCCodeParser::translateStmtIf(Statement* stmt, Statement* parent) const
{
	StmtIf* statement = new StmtIf();
	statement->type = Statement::If;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	translateExpression(stmt->innerExpressions[0], statement, false);

	for (int i = 0; i < stmt->childStatements.size(); i++)//由于后面可能链接if elseif else这种组合，也就是后面会有多个语句块，所以用循环遍历
	{
		int res = translateStatement(stmt->childStatements[i], statement);
        if(res < 0)
            return releaseStatementAndReturn(statement, res);
	}

	statement->condition = statement->innerExpressions[0];
	statement->statements = statement->childStatements;

	parent->childStatements.push_back(statement);
	return 1;
}

int ILCCodeParser::translateStmtElse(Statement* stmt, Statement* parent) const
{
	StmtElse* statement = new StmtElse();
	statement->type = Statement::Else;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	for (int i = 0; i < stmt->childStatements.size(); i++)//由于后面可能链接if elseif else这种组合，也就是后面会有多个语句块，所以用循环遍历
	{
		int res = translateStatement(stmt->childStatements[i], statement);
        if(res < 0)
            return releaseStatementAndReturn(statement, res);
	}

	statement->statements = statement->childStatements;

	parent->childStatements.push_back(statement);
	return 1;
}

int ILCCodeParser::translateStmtElseIf(Statement* stmt, Statement* parent) const
{
	StmtElseIf* statement = new StmtElseIf();
	statement->type = Statement::ElseIf;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	translateExpression(stmt->innerExpressions[0], statement, false);

	for (int i = 0; i < stmt->childStatements.size(); i++)//由于后面可能链接if elseif else这种组合，也就是后面会有多个语句块，所以用循环遍历
	{
		int res = translateStatement(stmt->childStatements[i], statement);
        if(res < 0)
            return releaseStatementAndReturn(statement, res);
	}

	statement->condition = statement->innerExpressions[0];
	statement->statements = statement->childStatements;

	parent->childStatements.push_back(statement);

	return 1;
}

int ILCCodeParser::translateStmtWhile(Statement* stmt, Statement* parent) const
{
	StmtWhile* statement = new StmtWhile();
	statement->type = Statement::While;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	translateExpression(stmt->innerExpressions[0], statement, false);

	for (int i = 0; i < stmt->childStatements.size(); i++)//由于后面可能链接if elseif else这种组合，也就是后面会有多个语句块，所以用循环遍历
	{
		int res = translateStatement(stmt->childStatements[i], statement);
        if(res < 0)
            return releaseStatementAndReturn(statement, res);
	}

	statement->condition = statement->innerExpressions[0];
	statement->statements = statement->childStatements;

	parent->childStatements.push_back(statement);

	return 1;
}

int ILCCodeParser::translateStmtDoWhile(Statement* stmt, Statement* parent) const
{
	StmtDoWhile* statement = new StmtDoWhile();
	statement->type = Statement::DoWhile;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	translateExpression(stmt->innerExpressions[0], statement, false);

	for (int i = 0; i < stmt->childStatements.size(); i++)//由于后面可能链接if elseif else这种组合，也就是后面会有多个语句块，所以用循环遍历
	{
		int res = translateStatement(stmt->childStatements[i], statement);
        if(res < 0)
            return releaseStatementAndReturn(statement, res);
	}

	statement->condition = statement->innerExpressions[0];
	statement->statements = statement->childStatements;

	parent->childStatements.push_back(statement);

	return 1;
}

int ILCCodeParser::translateStmtFor(Statement* stmt, Statement* parent) const
{
    int res;
	StmtFor* statement = new StmtFor();
	statement->type = Statement::For;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	Statement* statementInitial = new Statement(Statement::For, parent->depth + CONST_NUM_2, statement);
	statement->childStatements.push_back(statementInitial);
	if (!isEmptyStatement(stmt->childStatements[0]))
	{
        if((res = translateStatement(stmt->childStatements[0], statementInitial)) < 0)
            return releaseStatementAndReturn(statement, res);
	}


	Statement* statementCondition = new Statement(Statement::For, parent->depth + CONST_NUM_2, statement);
	statement->childStatements.push_back(statementCondition);
	if (!isEmptyStatement(stmt->childStatements[1]) && 
		stmt->childStatements[1]->innerExpressions.size() > 0 && 
		stmt->childStatements[1]->innerExpressions[0] &&
		!stmt->childStatements[1]->innerExpressions[0]->expressionStr.empty())
	{
        if((res = translateExpression(stmt->childStatements[1]->innerExpressions[0], statementCondition, false)) < 0)
            return releaseStatementAndReturn(statement, res);
	}


	Statement* statementIterator = new Statement(Statement::For, parent->depth + CONST_NUM_2, statement);
	statement->childStatements.push_back(statementIterator);
	if (!stmt->innerExpressions.empty())
	{
		for (int i = 0; i < stmt->innerExpressions.size(); i++)//由于后面可能要执行多个迭代语句，比如“i++,j++”，所以用循环遍历
		{
            if((res = translateExpression(stmt->innerExpressions[i], statementIterator, true)) < 0)
                return releaseStatementAndReturn(statement, res);
		}
	}


	for (int i = CONST_NUM_2; i < stmt->childStatements.size(); i++)//由于后面可能链接if elseif else这种组合，也就是后面会有多个语句块，所以用循环遍历
	{
        if((res = translateStatement(stmt->childStatements[i], statement)) < 0)
            return releaseStatementAndReturn(statement, res);
	}
    

	statement->initial = statement->childStatements[0]->childStatements;
	if(statement->childStatements[1]->innerExpressions.size() > 0 && statement->childStatements[1]->innerExpressions[0])
		statement->condition = statement->childStatements[1]->innerExpressions[0];
	statement->iterator = statement->childStatements[CONST_NUM_2]->childStatements;
	for(int i = CONST_NUM_3; i < statement->childStatements.size();i++)
	{
		statement->statements.push_back(statement->childStatements[i]);
	}

	parent->childStatements.push_back(statement);

	return 1;
}

int ILCCodeParser::translateStmtContinue(Statement* parent) const
{
	StmtContinue* statement = new StmtContinue();
	statement->type = Statement::Continue;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	parent->childStatements.push_back(statement);
	return 1;
}

int ILCCodeParser::translateStmtSwitch(Statement* stmt, Statement* parent) const
{
	StmtSwitch* statement = new StmtSwitch();
	statement->type = Statement::Switch;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	translateExpression(stmt->innerExpressions[0], statement, false);

	for (int i = 0; i < stmt->childStatements.size(); i++)//由于后面可能链接if elseif else这种组合，也就是后面会有多个语句块，所以用循环遍历
	{
		int res = translateStatement(stmt->childStatements[i], statement);
        if(res < 0)
            return releaseStatementAndReturn(statement, res);
	}

	statement->condition = statement->innerExpressions[0];
	statement->statements = statement->childStatements;

	parent->childStatements.push_back(statement);

	return 1;
}

int ILCCodeParser::translateStmtCase(Statement* stmt, Statement* parent) const
{
	StmtCase* statement = new StmtCase();
	statement->type = Statement::Case;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	int res = translateExpression(stmt->innerExpressions[0], statement, false);
    if(res < 0)
        return releaseStatementAndReturn(statement, res);

	statement->value = statement->innerExpressions[0];
	
	parent->childStatements.push_back(statement);

	return 1;
}

int ILCCodeParser::translateStmtDefault(Statement* parent) const
{
	StmtDefault* statement = new StmtDefault();
	statement->type = Statement::Default;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	parent->childStatements.push_back(statement);

	return 1;
}

int ILCCodeParser::translateStmtBreak(Statement* parent) const
{
	StmtBreak* statement = new StmtBreak();
	statement->type = Statement::Break;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	parent->childStatements.push_back(statement);
	return 1;
}

int ILCCodeParser::translateStmtReturn(Statement* stmt, Statement* parent) const
{
	StmtReturn* statement = new StmtReturn();
	statement->type = Statement::Return;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;

	if(!stmt->innerExpressions.empty())
	{
		int res = translateExpression(stmt->innerExpressions[0], statement, false);
        if(res < 0)
            return releaseStatementAndReturn(statement, res);
		statement->expression = statement->innerExpressions[0];
	}
	else
	{
		statement->expression = nullptr;
	}
	parent->childStatements.push_back(statement);

	return 1;
}

int ILCCodeParser::translateStmtBraceStatement(Statement* stmt, Statement* parent) const
{
	if (isEmptyStatement(stmt))
		return 0;
	//不单独构建BraceStatement的节点，因为已经由父节点包含了层次关系了
	for (int i = 0; i < stmt->childStatements.size(); i++)
	{
		translateStatement(stmt->childStatements[i], parent);
	}
	return 1;
}

int ILCCodeParser::translateStmtLocalVariable(Statement* stmt, Statement* parent) const
{
	Expression* dataType = stmt->innerExpressions[0];
	string dataTypeStr = dataType->expressionStr;
	int expCount = stmt->innerExpressions.size();
	int expIndex = 1;
	while (expIndex < expCount) {
		string variableName;
		int isPointer = 0;
		vector<int> arraySize;
		Expression* valueExp = nullptr;
		string valueStr;
        bool handleSingleVarDec = false;
        int res = translateStmtLocalVariableA(stmt, expIndex, valueExp, valueStr, isPointer, variableName, arraySize, handleSingleVarDec);
        if(res != 0){
            return res;
        }
		
		if (!handleSingleVarDec) {
            continue;
		}
        string arraySizeStr;
	    for (int i = 0; i < arraySize.size(); i++) {
		    if (i > 0 && arraySize[i] <= 0) {
			    return -ErrorInvalidArraySize;
            } else if (i == 0 && (arraySize[i] == 0 || arraySize[i] == -1 && (valueExp == nullptr || valueExp->type != Token::Compound))) {
				return -ErrorInvalidArraySize;
			} else if(i == 0 && arraySize[i] == -1 && valueExp) {
				arraySize[i] = valueExp->childExpressions.size();
			}
		    arraySizeStr += to_string(arraySize[i]);
		    if (i < arraySize.size() - 1){
			    arraySizeStr += ",";
            }
	    }
        res = translateStmtLocalVariableB(valueExp, dataTypeStr, valueStr, isPointer, variableName, arraySize, parent, arraySizeStr);
        if(res != 0){
            return res;
        }
	}
	return 1;
}

int ILCCodeParser::translateStmtLocalVariableA(const Statement* stmt, int &expIndex, Expression* &valueExp, 
    string &valueStr, int &isPointer, string &variableName, vector<int> &arraySize, 
    bool &handleSingleVarDec) const
{
    int expCount = stmt->innerExpressions.size();
	if (stmt->innerExpressions[expIndex]->type == Token::References)
		return -ErrorIntermediateLanguageDoesNotSupportReferenceType;
	while (stmt->innerExpressions[expIndex]->type == Token::Pointer)
	{
		if (stmt->innerExpressions[expIndex]->expressionStr.size() > 1)
		{
		    return -ErrorIntermediateLanguageDoesNotSupportMultiPointer;
		}
		else
		{
			isPointer++;
			expIndex++;
		}
	}
	variableName = stmt->innerExpressions[expIndex]->expressionStr;
	expIndex++;
	
	while (expIndex < expCount && stmt->innerExpressions[expIndex]->type == Token::Dimension)
	{

		string dimension = stmt->innerExpressions[expIndex]->expressionStr;
		dimension = dimension.substr(1, dimension.length() - CONST_NUM_2);
		if (dimension.empty())
			arraySize.push_back(-1);
		else
			arraySize.push_back(stringToInt(dimension));
		expIndex++;
	}
	if (expIndex < expCount){
		if (stmt->innerExpressions[expIndex]->type == Token::Comma){
			expIndex++;
			handleSingleVarDec = true;
		} else if (stmt->innerExpressions[expIndex]->type == Token::Assign) {
			expIndex++;
			valueExp = stmt->innerExpressions[expIndex];
			valueStr = stmt->innerExpressions[expIndex]->expressionStr;
			expIndex++;
			if (expIndex < expCount && stmt->innerExpressions[expIndex]->type == Token::Comma) {
				expIndex++;
			}
			handleSingleVarDec = true;
		} else {
			handleSingleVarDec = true;
		}
	} else {
		handleSingleVarDec = true;
	}
    return 0;
}

int ILCCodeParser::translateStmtLocalVariableB(Expression* &valueExp, const string &dataTypeStr, const string &valueStr,
    int isPointer, const string &variableName, const vector<int> &arraySize, Statement* parent,
    string arraySizeStr) const
{
	StmtLocalVariable* statement = new StmtLocalVariable();
	statement->type = Statement::LocalVariable;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;
	parent->childStatements.push_back(statement);
	statement->name = variableName;
	statement->dataType = dataTypeStr;
    if (stringStartsWith(statement->dataType, "struct "))
        statement->dataType = statement->dataType.substr(CONST_NUM_7, statement->dataType.length() - CONST_NUM_7);
    else if (stringStartsWith(statement->dataType, "enum "))
        statement->dataType = statement->dataType.substr(CONST_NUM_5, statement->dataType.length() - CONST_NUM_5);
    
	if (isPointer){
		statement->isAddress = isPointer;
    }
	if(!arraySizeStr.empty()) {
		vector<string> arraySizeStrSplit = stringSplit(arraySizeStr, ",");
		for(int i = 0;i < arraySizeStrSplit.size();i++) {
			statement->arraySize.push_back(stringToInt(arraySizeStrSplit[i]));
		}
	}
	if(!valueStr.empty() && valueExp && valueExp->type != Token::Compound) {
		int res = translateExpression(valueExp, statement, false);
        if (res < 0){
			return releaseStatementAndReturn(statement, res);
        }
		statement->defaultValue = statement->innerExpressions[0];
	}
	if (valueExp && valueExp->type == Token::Compound){
		Expression* variableNameExp = new Expression();
		variableNameExp->expressionStr = variableName;
		variableNameExp->isExpression = true;
		variableNameExp->parentExpression = nullptr;
		variableNameExp->tokenStr = variableName;
		variableNameExp->type = Token::Variable;
		variableNameExp->updateExpressionStr();
		int res = translateStmtVariableInitValue(variableNameExp, dataTypeStr, arraySize, valueExp, -1, parent);
		variableNameExp->release();
        delete variableNameExp;
		if (res < 0){
			return releaseStatementAndReturn(statement, res);
        }
	}
    return 0;
}

//childExpIndex 为-1表示将一整个Compound赋值给某数组或结构体
int ILCCodeParser::translateStmtVariableInitValue(const Expression* varNameExp, string dataType, vector<int> arraySize, Expression* valueExp, int childExpIndex, Statement* parent) const
{
    int res;
    if(stringStartsWith(dataType, "struct "))
        dataType = dataType.substr(CONST_NUM_7, dataType.length() - CONST_NUM_7);
	bool isStruct = isStructType(dataType);
	ILStructDefine* structType = ILParser::findStructType(dataType);
	if (!arraySize.empty())//是数组则吸收数组
	{
        if((res = translateStmtVariableInitValueA(varNameExp, dataType, arraySize, valueExp, childExpIndex, parent)) != 0)
            return res;
		if (childExpIndex != -1)
		{
			Expression* compound = createExpression(Token::Compound, "", true);
			int count = (arraySize[0] < valueExp->childExpressions.size() - childExpIndex) ? arraySize[0] : (valueExp->childExpressions.size() - childExpIndex);
			for (int i = 0; i < count; i++)
			{
				compound->childExpressions.push_back(valueExp->childExpressions[i + childExpIndex]);
				compound->expressionStr += ((i != 0) ? "," : "");
				compound->expressionStr += valueExp->childExpressions[i + childExpIndex]->expressionStr;
			}
			compound->expressionStr = "{" + compound->expressionStr + "}";
			valueExp->childExpressions.erase(valueExp->childExpressions.begin() + childExpIndex, valueExp->childExpressions.begin() + childExpIndex + count);
			valueExp->childExpressions.insert(valueExp->childExpressions.begin() + childExpIndex, compound);
		}
	}
	else if (isStruct && structType)
	{
        if((res = translateStmtVariableInitValueB(varNameExp, valueExp, childExpIndex, parent, structType)) != 0)
            return res;
		if (childExpIndex != -1)
		{
			Expression* compound = createExpression(Token::Compound, "", true);
			int count = (structType->members.size() < valueExp->childExpressions.size() - childExpIndex) ? structType->members.size() : (valueExp->childExpressions.size() - childExpIndex);
			for (int i = 0; i < count; i++)
			{
				compound->childExpressions.push_back(valueExp->childExpressions[i + childExpIndex]);
				compound->expressionStr += ((i != 0) ? "," : "");
				compound->expressionStr += valueExp->childExpressions[i + childExpIndex]->expressionStr;
			}
			compound->expressionStr = "{" + compound->expressionStr + "}";
			valueExp->childExpressions.erase(valueExp->childExpressions.begin() + childExpIndex, valueExp->childExpressions.begin() + childExpIndex + count);
			valueExp->childExpressions.insert(valueExp->childExpressions.begin() + childExpIndex, compound);

		}
	}
	else
	{
        if((res = translateStmtVariableInitValueC(varNameExp, valueExp, childExpIndex, parent)) != 0)
            return res;
	}
    return 1;
}

int ILCCodeParser::translateStmtVariableInitValueA(const Expression* varNameExp, string dataType, 
    vector<int> arraySize, Expression* valueExp, int childExpIndex, Statement* parent) const
{
    int res = 0;
    int offset = childExpIndex == -1 ? 0 : childExpIndex;
    for (int i = 0; i < arraySize[0] && i + offset < valueExp->childExpressions.size(); i++) {
	    if (valueExp->childExpressions[i + offset]->type == Token::CompoundElement) {
		    string indexStr = valueExp->childExpressions[i + offset]->childExpressions[0]->expressionStr;
		    if (indexStr[0] != '[' || indexStr[indexStr.length() - CONST_NUM_2] != ']')
			    return -ErrorStatementVariableInvalidAssignment;
	    }
	    if (valueExp->childExpressions[i + offset]->type == Token::Compound) {
		    vector<int> arraySizeTemp;
            arraySizeTemp.insert(arraySizeTemp.begin(), arraySize.begin() + 1, arraySize.end());
		    Expression* variableNameExp = getExtendSubscriptExp(varNameExp, i);
            if(!variableNameExp)
                return -ErrorInner;
		    else if ((res = translateStmtVariableInitValue(variableNameExp, dataType, arraySizeTemp,
                valueExp->childExpressions[i + offset], -1, parent)) < 0){
			    return releaseExpressionAndReturn(variableNameExp, res);
            }
            releaseExpression(variableNameExp);
	    } else if (valueExp->childExpressions[i + offset]->type == Token::CompoundElement && valueExp->childExpressions[i + offset]->childExpressions[1]->type == Token::Compound) {
		    vector<int> arraySizeTemp;
            arraySizeTemp.insert(arraySizeTemp.begin(), arraySize.begin() + 1, arraySize.end());
		    Expression* variableNameExp = getExtendSubscriptExp(varNameExp, i);
            if(!variableNameExp)
                return -ErrorInner;
		    else if((res = translateStmtVariableInitValue(variableNameExp, dataType, arraySizeTemp,
                valueExp->childExpressions[i + offset]->childExpressions[1], -1, parent)) < 0){
			    return releaseExpressionAndReturn(variableNameExp, res);
            }
            releaseExpression(variableNameExp);
	    } else {
		    vector<int> arraySizeTemp;
            arraySizeTemp.insert(arraySizeTemp.begin(), arraySize.begin() + 1, arraySize.end());
		    Expression* variableNameExp = getExtendSubscriptExp(varNameExp, i);
            if(!variableNameExp)
                return -ErrorInner;
		    else if((res = translateStmtVariableInitValue(variableNameExp, dataType, arraySizeTemp, valueExp,
                i + offset, parent)) < 0){
			    return releaseExpressionAndReturn(variableNameExp, res);
            }
            releaseExpression(variableNameExp);
	    }
    }
    return 0;
}

int ILCCodeParser::translateStmtVariableInitValueB(const Expression* varNameExp, 
    Expression* valueExp, int childExpIndex, Statement* parent, 
    const ILStructDefine* structType) const
{
	int offset = childExpIndex == -1 ? 0 : childExpIndex;
	for (int i = 0; i < structType->members.size(); i++)
	{
		if (i + offset >= valueExp->childExpressions.size())
			break;

		if(valueExp->childExpressions[i + offset]->type == Token::CompoundElement &&
			valueExp->childExpressions[i + offset]->childExpressions[0]->expressionStr != "." + structType->members[i]->name + "=")
			return -ErrorStatementVariableInvalidAssignment;

		if (valueExp->childExpressions[i + offset]->type == Token::Compound)
		{
			Expression* variableNameExp = getExtendMemberExp(varNameExp, structType->members[i]->name);
            if(!variableNameExp)
                return -ErrorInner;
			int res = translateStmtVariableInitValue(variableNameExp, structType->members[i]->type, 
                structType->members[i]->arraySize, valueExp->childExpressions[i + offset], -1, parent);
            //comment = true;
		    releaseExpression(variableNameExp);
			if (res < 0)
				return res;
		}
		else if (valueExp->childExpressions[i + offset]->type == Token::CompoundElement && valueExp->childExpressions[i + offset]->childExpressions[1]->type == Token::Compound)
		{
			Expression* variableNameExp = getExtendMemberExp(varNameExp, structType->members[i]->name);
            if(!variableNameExp)
                return -ErrorInner;
			int res = translateStmtVariableInitValue(variableNameExp, structType->members[i]->type, 
                structType->members[i]->arraySize,
                valueExp->childExpressions[i + offset]->childExpressions[1], -1, parent);
            //comment = true;
		    releaseExpression(variableNameExp);
			if (res < 0)
				return res;
		}
		else
		{
			Expression* variableNameExp = getExtendMemberExp(varNameExp, structType->members[i]->name);
            if(!variableNameExp)
                return -ErrorInner;
			int res = translateStmtVariableInitValue(variableNameExp, structType->members[i]->type, 
                structType->members[i]->arraySize, valueExp, i + offset, parent);
            //comment = true;
		    releaseExpression(variableNameExp);
			if (res < 0)
				return res;
		}
	}
    return 0;
}

int ILCCodeParser::translateStmtVariableInitValueC(const Expression* varNameExp,
    Expression* valueExp, int childExpIndex, Statement* parent) const
{
	if (childExpIndex == -1)
		return -ErrorStatementVariableInvalidAssignment;
	
	
	string expStr = varNameExp->expressionStr + "=";
	if (valueExp->childExpressions[childExpIndex]->type == Token::CompoundElement)
		expStr += valueExp->childExpressions[childExpIndex]->childExpressions[1]->expressionStr;
	else
		expStr += valueExp->childExpressions[childExpIndex]->expressionStr;
	
	StmtExpression* statement = new StmtExpression();
	statement->type = Statement::ExpressionWithSemicolon;
	statement->depth = parent->depth + 1;
	statement->parentStatement = parent;
	parent->childStatements.push_back(statement);


	Expression* expRoot = new Expression();
	expRoot->parentExpression = nullptr;
	expRoot->isExpression = true;
	expRoot->tokenStr = "=";
	expRoot->type = Token::Assign;
	
	Expression* expName = varNameExp->clone();
    if(!expName)
    {
        releaseStatement(statement);
        releaseExpression(expRoot);
        return -ErrorInner;
    }
	expName->parentExpression = expRoot;


	Expression* expValue = nullptr;
	if (valueExp->childExpressions[childExpIndex]->type == Token::CompoundElement)
		expValue = valueExp->childExpressions[childExpIndex]->childExpressions[1]->clone();
	else
		expValue = valueExp->childExpressions[childExpIndex];

    if(!expValue)
    {
        releaseStatement(statement);
        releaseExpression(expRoot);
        releaseExpression(expName);
        return -ErrorInner;
    }
    
    expValue->parentExpression = expRoot;

	expRoot->childExpressions.push_back(expName);
	expRoot->childExpressions.push_back(expValue);

	expRoot->updateExpressionStr();

	statement->innerExpressions.push_back(expRoot);

	statement->expression = expRoot;

	return 1;
}

int ILCCodeParser::translateStmtExpressionWithSemicolon(Statement* stmt, Statement* parent, 
    bool isNewStmtExpression) const
{
	//不单独构建ExpressionWithSemicolon的XML节点，直接表示里面的表达式
	for (int i = 0; i < stmt->innerExpressions.size(); i++)
	{
		//StmtExpression* statement = new StmtExpression();
		//statement->type = Statement::ExpressionWithSemicolon;
		//statement->depth = parent->depth + 1;
		//statement->parentStatement = parent;

		int res = translateExpression(stmt->innerExpressions[i], parent, isNewStmtExpression);
		if (res < 0)
			return res;

		//statement->expression = statement->innerExpressions[0];
		//
		//parent->childStatements.push_back(statement);
	}
	return 0;
}

int ILCCodeParser::translateExpression(const Expression* exp, Statement* parent, bool isNewStmtExpression) const
{
	if (exp->type == Token::Assign &&
		exp->childExpressions[1]->type == Token::TypeConvert &&
		exp->childExpressions[1]->childExpressions[0]->type == Token::DataType &&
		exp->childExpressions[1]->childExpressions[1]->type == Token::Compound &&
		isStructType(exp->childExpressions[1]->childExpressions[0]->expressionStr))
	{
		Expression* variableNameExp = new Expression(); //只用来临时存储变量名等信息
		variableNameExp->expressionStr = exp->childExpressions[0]->expressionStr;
		variableNameExp->isExpression = true;
		variableNameExp->parentExpression = nullptr;
		variableNameExp->tokenStr = exp->childExpressions[0]->expressionStr;
		variableNameExp->type = Token::Variable;
		variableNameExp->updateExpressionStr();
		int res = translateStmtVariableInitValue(variableNameExp, 
			exp->childExpressions[1]->childExpressions[0]->expressionStr, vector<int>(), 
            exp->childExpressions[1]->childExpressions[1], -1, parent);
        releaseExpression(variableNameExp);
		if (res < 0){
		    return res;
		}
	}
	else
	{
		if(isNewStmtExpression)
		{
			StmtExpression* statement = new StmtExpression();
			statement->type = Statement::ExpressionWithSemicolon;
			statement->depth = parent->depth + 1;
			statement->parentStatement = parent;
			parent->childStatements.push_back(statement);

			Expression* expRoot = exp->clone();

			statement->innerExpressions.push_back(expRoot);
			statement->expression = expRoot;
		}
		else
		{
			Expression* expRoot = exp->clone();
			parent->innerExpressions.push_back(expRoot);
		}
		
	}
	return 1;
}

Expression* ILCCodeParser::getExtendSubscriptExp(const Expression* exp, int subscriptId) const
{
	Expression* variableNameExp = new Expression();
	variableNameExp->isExpression = true;
	variableNameExp->parentExpression = nullptr;
	variableNameExp->tokenStr = "[";
	variableNameExp->type = Token::ArrayIndex;

	Expression* variableNameExpChild1 = exp->clone();
    if(!variableNameExpChild1)
    {
        releaseExpression(variableNameExp);
        return nullptr;
    }

	variableNameExpChild1->parentExpression = variableNameExp;

    Expression* variableNameExpChild2 = new Expression();
	variableNameExpChild2->parentExpression = variableNameExp;
	variableNameExpChild2->isExpression = false;
	variableNameExpChild2->tokenStr = "[";
	variableNameExpChild2->type = Token::Subscript;

	Expression* variableNameExpChild3 = new Expression();
	variableNameExpChild3->parentExpression = variableNameExpChild2;
	variableNameExpChild3->isExpression = true;
	variableNameExpChild3->tokenStr = to_string(subscriptId);
	variableNameExpChild3->type = Token::Number;
	variableNameExpChild2->childExpressions.push_back(variableNameExpChild3);


	variableNameExp->childExpressions.push_back(variableNameExpChild1);
	variableNameExp->childExpressions.push_back(variableNameExpChild2);

	variableNameExp->updateExpressionStr();

	return variableNameExp;
}

Expression* ILCCodeParser::getExtendMemberExp(const Expression* exp, string memberName) const
{
	Expression* variableNameExp = new Expression();
	variableNameExp->isExpression = true;
	variableNameExp->parentExpression = nullptr;
	variableNameExp->tokenStr = ".";
	variableNameExp->type = Token::Point;

	Expression* variableNameExpChild1 = exp->clone();
    if(!variableNameExpChild1)
    {
        releaseExpression(variableNameExp);
        return nullptr;
    }

	variableNameExpChild1->parentExpression = variableNameExp;

	Expression* variableNameExpChild2 = new Expression();
	variableNameExpChild2->parentExpression = variableNameExp;
	variableNameExpChild2->isExpression = true;
	variableNameExpChild2->tokenStr = memberName;
	variableNameExpChild2->type = Token::Variable;

	variableNameExp->childExpressions.push_back(variableNameExpChild1);
	variableNameExp->childExpressions.push_back(variableNameExpChild2);

	variableNameExp->updateExpressionStr();

	return variableNameExp;
}

void ILCCodeParser::releaseStatement(Statement* stmt) const
{
    stmt->release();
    delete stmt;
}

void ILCCodeParser::releaseExpression(Expression* exp) const
{
    exp->release();
    delete exp;
}

int ILCCodeParser::releaseExpressionAndReturn(Expression* exp, int retCode) const
{
    exp->release();
    delete exp;
    return retCode;
}

int ILCCodeParser::releaseILObjectAndReturn(ILObject* obj, int retCode) const
{
    obj->release();
    delete obj;
    return retCode;
}

int ILCCodeParser::releaseStatementAndReturn(Statement* stmt, int retCode) const
{
    stmt->release();
    delete stmt;
    return retCode;
}
