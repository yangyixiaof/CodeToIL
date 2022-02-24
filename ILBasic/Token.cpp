#include "Token.h"
using namespace std;

vector<string> Token::tokenNameList = {
		"SelfAdd", "SelfSub",				
		"BracketLeft", "BracketRight",		
		"SubscriptLeft", "SubscriptRight",	
		"Point",							
		"Arrow",						
		"Compound",						
		"SelfAddLeft", "SelfSubLeft",	
		"Negative", "Positive",			
		"Not", "BitNot",				
		"TypeConvert",					
		"Indirection",					
		"AddressOf",					
		"SizeOf",						
		"Mul", "Div", "Mod",			
		"Add", "Sub",					
		"LeftShift", "RightShift",		
		"Less", "LessEqual",			
		"Greater", "GreaterEqual",		
		"Equal", "NotEqual",			
		"BitAnd",						
		"BitXor",						
		"BitOr",						
		"And",							
		"Or",							
		"TernaryConditional",				
		"Assign",							
		"AddAssign", "SubAssign",			
		"MulAssign", "DivAssign", "ModAssign",
		"LeftShiftAssign",				
		"RightShiftAssign",				
		"AndAssign", "XorAssign", "OrAssign",
		"Comma",							
		"Number", "Word", "String", "Char",	
		"Variable", "Function",				
		"Member",							
		"If", "Else", "While", "For", "Do",		
		"Continue", "Switch", "Case",
		"Default", "Break", "Return",
		"ElseIf",					
		"Condition", "Select",		
		"BraceLeft", "BraceRight",	
		"Semicolon",				
		"DataType",					
		"Bracket",					
		"Subscript",				
		"CompoundElement",			
		"CompoundElementIndex",		
		"ArrayIndex",				
		"References",				
		"Pointer",					
		"Dimension",				
		"Unknow"
};

vector<string> Token::tokenStrList = {
		"++", "--",				
		"(", ")",		
		"[", "]",	
		".",							
		"->",						
		"Compound",						
		"++", "++",	
		"-", "+",			
		"!", "~",				
		"TypeConvert",					
		"*",					
		"&",					
		"SizeOf",						
		"*", "/", "%",			
		"+", "-",					
		"<<", ">>",		
		"<", "<=",			
		">", ">=",		
		"==", "!=",			
		"&",						
		"^",						
		"|",						
		"&&",							
		"||",							
		"TernaryConditional",				
		"=",							
		"+=", "-=",			
		"*=", "/=", "%=",
		"<<=",				
		">>=",				
		"&=", "^=", "|=",
		",",							
		"Number", "Word", "String", "Char",	
		"Variable", "Function",				
		"Member",							
		"if", "else", "while", "for", "do",		
		"continue", "switch", "case",
		"default", "break", "return",
		"else if",					
		"?", ":",		
		"{", "}",	
		";",				
		"DataType",					
		"Bracket",					
		"Subscript",				
		"CompoundElement",			
		"CompoundElementIndex",		
		"ArrayIndex",				
		"References",				
		"Pointer",					
		"Dimension",				
		"Unknow"
};

Token::Token(Token::Type type, std::string value) : type(type), value(value)
{
}

Token::~Token()
{
}