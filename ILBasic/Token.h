#pragma once
#include <string>
#include <vector>
#include <iostream>

class Token
{
public:

	enum Type{
		SelfAdd, SelfSub,				
		BracketLeft, BracketRight,		
		SubscriptLeft, SubscriptRight,	
		Point,							
		Arrow,							
		Compound,						

		SelfAddLeft, SelfSubLeft,		
		Negative, Positive,				
		Not, BitNot,					
		TypeConvert,					
		Indirection,					
		AddressOf,						
		SizeOf,							

		Mul, Div, Mod,					

		Add, Sub,						

		LeftShift, RightShift,			

		Less, LessEqual,				
		Greater, GreaterEqual,			

		Equal, NotEqual,				

		BitAnd,							

		BitXor,							

		BitOr,							

		And,							

		Or,								

		TernaryConditional,				

		Assign,							
		AddAssign, SubAssign,			
		MulAssign, DivAssign, ModAssign,
		LeftShiftAssign, 				
		RightShiftAssign,				
		AndAssign, XorAssign, OrAssign,	

		Comma,							



		Number, Word, String, Char,		
		Variable, Function,				
		Member,							

		If, Else, While, For, Do,		
		Continue, Switch, Case, 		
		Default, Break, Return,			
		ElseIf,							
		Condition, Select,				
		BraceLeft, BraceRight,			
		Semicolon, 						
		DataType, 						
		Bracket,						
		Subscript,						
		CompoundElement,				
		CompoundElementIndex,			
		ArrayIndex,						
		References,						
		Pointer,						
		Dimension,						
		Unknow,
	};
	static std::vector<std::string> tokenNameList;
	static std::vector<std::string> tokenStrList;
	Type type;
	std::string value;

	Token(Token::Type type, std::string value);
    
    ~Token();
};
