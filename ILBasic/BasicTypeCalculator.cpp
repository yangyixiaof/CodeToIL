#include "BasicTypeCalculator.h"
#include <algorithm>

#include "../Common/Utility.h"

using namespace  std;

namespace
{
    const int CONST_NUM_4 = 4;
    const int CONST_NUM_7 = 7;
}
std::vector<std::string> BasicTypeCalculator::hiddenCast = {
    "bool", "char", "signed char", "unsigned char", "short", "signed short", 
    "unsigned short", "int", "signed", "signed int", "long", "signed long", 
    "unsigned", "unsigned int", "unsigned long", "long long", "signed long long",
    "unsigned long long",
    "float", "double", "long double"
};

std::map<Token::Type, std::string> BasicTypeCalculator::expressionBasicTypeMap = {
    pair<Token::Type, string>(Token::Not, "bool"),
    pair<Token::Type, string>(Token::And, "bool"),
    pair<Token::Type, string>(Token::Or, "bool"),
    pair<Token::Type, string>(Token::Less, "bool"),
    pair<Token::Type, string>(Token::LessEqual, "bool"),
    pair<Token::Type, string>(Token::Greater, "bool"),
    pair<Token::Type, string>(Token::GreaterEqual, "bool"),
    pair<Token::Type, string>(Token::Equal, "bool"),
    pair<Token::Type, string>(Token::NotEqual, "bool"),
    pair<Token::Type, string>(Token::Char, "char"),
    pair<Token::Type, string>(Token::SizeOf, "unsigned int"),
};

int BasicTypeCalculator::findIndex(const std::string& type1) {
	int tindex1 = -1;
	std::vector<std::string>::iterator it1 = std::find(hiddenCast.begin(), hiddenCast.end(), type1);
	if (it1 != hiddenCast.end()) {
		tindex1 = it1 - hiddenCast.begin();
	}
	return tindex1;
}

std::string BasicTypeCalculator::trimLastInt(const std::string& tp) {
    if(tp.empty())
        return tp;
	std::string realType1 = tp;
	std::vector<std::string> tpss = stringSplit(realType1, " ");
	int tpsize = tpss.size();
	int validSize = tpsize;
	if (tpss[0] == "unsigned" || tpss[0] == "signed") {
		validSize -= 1;
	}
	if (tpss[tpsize - 1] == "int") {
		validSize -= 1;
		if (validSize > 0) {
			realType1 = realType1.substr(0, realType1.length() - CONST_NUM_4);
		}
	}
	return realType1;
}

std::string BasicTypeCalculator::typePrintRefine(const std::string& tp) {
	if (tp == "signed char") {
		return tp;
	}
	if (tp == "signed") {
		return "int";
	}
	if (tp == "unsigned") {
		return "unsigned int";
	}

	std::string realType1 = tp;
	if (stringStartsWith(realType1, "signed ")) {
		realType1 = realType1.substr(CONST_NUM_7, realType1.length() - CONST_NUM_7);
	}

	realType1 = trimLastInt(realType1);
	return realType1;
}

std::string BasicTypeCalculator::typeSecondPrintRefine(const std::string& tp, const std::string& tp2) {
	std::string realType1 = tp;
	
	if (realType1 == "unsigned int" && (tp2 == "long" || tp2 == "unsigned long")) {
	    realType1 = "unsigned long";
	}

	return realType1;
}

std::string BasicTypeCalculator::typeSearchRefine(const std::string& tp) {
	std::string realType1 = trimLastInt(tp);
	return realType1;
}

std::string BasicTypeCalculator::invalidType(const std::string& type1, const std::string& type2, const Token::Type op)
{
	if (op == Token::Type::Mod || op == Token::Type::BitNot || op == Token::Type::BitAnd ||
        op == Token::Type::BitOr || op == Token::Type::BitXor || op == Token::Type::LeftShift ||
        op == Token::Type::RightShift ||  op == Token::Type::ModAssign || op == Token::Type::XorAssign ||
        op == Token::Type::RightShiftAssign || op == Token::Type::LeftShiftAssign  ||
        op == Token::Type::AndAssign || op == Token::Type::OrAssign)
    {
		if (type1 == "float" || type1 == "double" || type1 == "long double" ||
            type2 == "float" || type2 == "double" || type2 == "long double") {
			return "invalid";
		}
	}
	return "";
}

std::string BasicTypeCalculator::getExpResultType(const std::string& type1, const std::string& type2, const Token::Type op)
{
    if(BasicTypeCalculator::expressionBasicTypeMap.find(op) != BasicTypeCalculator::expressionBasicTypeMap.end())
    {
        return BasicTypeCalculator::expressionBasicTypeMap.at(op);
    }

	std::string realSearchType1 = typeSearchRefine(type1);
	std::string realPrintType1 = typePrintRefine(type1);
	int tindex1 = findIndex(realSearchType1);
	std::string realSearchType2 = typeSearchRefine(type2);
	std::string realPrintType2 = typePrintRefine(type2);
	int tindex2 = findIndex(realSearchType2);

	std::string at = invalidType(realPrintType1, realPrintType2, op);
	if (at == "invalid") {
		return at;
	}

	int tindexInt = findIndex("int");
    
	if (op == Token::Type::Add || op == Token::Type::Sub || op == Token::Type::Mul ||
        op == Token::Type::Div || op == Token::Type::Mod || op == Token::Type::BitNot ||
        op == Token::Type::BitAnd || op == Token::Type::BitOr || op == Token::Type::BitXor ||
        op == Token::Type::TernaryConditional) {
		
		int maxIndex = tindex2 > tindex1 ? tindex2 : tindex1;
		std::string result = tindex2 > tindex1 ? realPrintType2 : realPrintType1;
		std::string oppResult = tindex2 > tindex1 ? realPrintType1 : realPrintType2;
		result = typeSecondPrintRefine(result, oppResult);
		if (op == Token::Type::TernaryConditional && realPrintType1 == realPrintType2) {
	        return result;
		}
		result = tindexInt > maxIndex ? "int" : result;
		return result;
	}
	if (op == Token::Type::LeftShift || op == Token::Type::RightShift) {
		std::string result = tindexInt > tindex1 ? "int" : realPrintType1;
		return result;
	}
	
	//if (op == Token::Type::Negative || op == Token::Type::Positive || op == Token::Type::SelfAdd ||
    //    op == Token::Type::SelfAddLeft || op == Token::Type::SelfSub || op == Token::Type::SelfSubLeft) {
	//	// return type1;
	//}
	//if (op == Token::Type::Assign || op == Token::Type::AddAssign || op == Token::Type::SubAssign ||
    //    op == Token::Type::MulAssign || op == Token::Type::DivAssign || op == Token::Type::ModAssign ||
    //    op == Token::Type::XorAssign || op == Token::Type::RightShiftAssign ||
    //    op == Token::Type::LeftShiftAssign || op == Token::Type::AndAssign ||
    //    op == Token::Type::OrAssign) {
	//	// return type1;
	//}
    //comment = true;
	return realPrintType1;
}
