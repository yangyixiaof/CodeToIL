#pragma once
#include <string>
#include <map>
#include <vector>

#include "Token.h"

class BasicTypeCalculator
{
public:
    static std::map<Token::Type, std::string> expressionBasicTypeMap;
    static std::string getExpResultType(const std::string& type1, const std::string& type2, Token::Type op);

private:
    static std::vector<std::string> hiddenCast;

    static int findIndex(const std::string& type1);
    static std::string trimLastInt(const std::string& tp);
    static std::string typePrintRefine(const std::string& tp);
    static std::string typeSecondPrintRefine(const std::string& tp, const std::string& tp2);
    static std::string typeSearchRefine(const std::string& tp);
    static std::string invalidType(const std::string& type1, const std::string& type2, Token::Type op);
};
