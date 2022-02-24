#include "Statement.h"

#include <algorithm>
#include <iostream>

#include "Expression.h"
#include "ILObject.h"

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
using namespace std;

vector<string> Statement::statementTypeName = { 
	"If", "Else", "ElseIf", "While",
	"DoWhile", "For", "Continue", "Switch", "Case", 
	"Default", "Break", "Return", "BraceStatement", 
	"LocalVariable", "ExpressionWithSemicolon",
	"BatchCalculation", "Assemble",
	"Unknown",
};


Statement::Statement()
{
}

Statement::Statement(Type type, const Statement* parent)
{
    this->type = type;
    this->parentStatement = const_cast<Statement*>(parent);
}

Statement::Statement(Type type, int depth, const Statement* parent)
{
    this->type = type;
    this->depth = depth;
    this->parentStatement = const_cast<Statement*>(parent);
}

Statement::Statement(Type type)
{
    this->type = type;
}

Statement::Statement(const Statement* parent)
{
    this->parentStatement = const_cast<Statement*>(parent);
}

Statement::~Statement()
{
}

Statement* Statement::getNextStatement() const
{
    vector<Statement*>* stmtList = nullptr;
    if(parentStatement && parentStatement->type == Statement::If) {
        stmtList = &(static_cast<StmtIf*>(parentStatement)->statements);
	} else if(parentStatement && parentStatement->type == Statement::Else) {
        stmtList = &(static_cast<StmtElse*>(parentStatement)->statements);
	} else if(parentStatement && parentStatement->type == Statement::ElseIf) {
        stmtList = &(static_cast<StmtElseIf*>(parentStatement)->statements);
	} else if(parentStatement && parentStatement->type == Statement::While) {
        stmtList = &(static_cast<StmtWhile*>(parentStatement)->statements);
	} else if(parentStatement && parentStatement->type == Statement::DoWhile) {
        stmtList = &(static_cast<StmtDoWhile*>(parentStatement)->statements);
	} else if(parentStatement && parentStatement->type == Statement::Switch) {
        stmtList = &(static_cast<StmtSwitch*>(parentStatement)->statements);
	} else if(parentStatement && (parentStatement->type == Statement::Continue ||
		parentStatement->type == Statement::Case || parentStatement->type == Statement::Default ||
		parentStatement->type == Statement::Return || parentStatement->type == Statement::LocalVariable ||
		parentStatement->type == Statement::ExpressionWithSemicolon ||
        parentStatement->type == Statement::BatchCalculation ||
        parentStatement->type == Statement::Assemble)) {
		return nullptr;
	} else if(parentStatement && parentStatement->type == Statement::For) {
		StmtFor* stmtTemp = static_cast<StmtFor*>(parentStatement);
        auto iter1 = find(stmtTemp->initial.begin(), stmtTemp->initial.end(), this);
        auto iter2 = find(stmtTemp->statements.begin(), stmtTemp->statements.end(), this);
        auto iter3 = find(stmtTemp->iterator.begin(), stmtTemp->iterator.end(), this);
		if(iter1 == stmtTemp->initial.end() && iter2 == stmtTemp->statements.end() && iter3 == stmtTemp->iterator.end())
			return nullptr;
		
		if(iter1 != stmtTemp->initial.end()) {
            stmtList = &(stmtTemp->initial);
		} else if(iter2 != stmtTemp->statements.end()) {
            stmtList = &(stmtTemp->statements);
		} else if(iter3 != stmtTemp->iterator.end()) {
            stmtList = &(stmtTemp->iterator);
		}
	} else if(parentStatement){
        stmtList = &(parentStatement->childStatements);
	}
    if(stmtList) {
        auto iter = find(stmtList->begin(), stmtList->end(), this);
		if(iter == stmtList->end())
			return nullptr;
		iter++;
		return iter == stmtList->end() ? nullptr : *iter;
    }
	return nullptr;
}

Statement* Statement::getPreviousStatement() const
{
    vector<Statement*>* stmtList = nullptr;
    if(parentStatement && parentStatement->type == Statement::If) {
        stmtList = &(static_cast<StmtIf*>(parentStatement)->statements);
	} else if(parentStatement && parentStatement->type == Statement::Else) {
        stmtList = &(static_cast<StmtElse*>(parentStatement)->statements);
	} else if(parentStatement && parentStatement->type == Statement::ElseIf) {
        stmtList = &(static_cast<StmtElseIf*>(parentStatement)->statements);
	} else if(parentStatement && parentStatement->type == Statement::While) {
        stmtList = &(static_cast<StmtWhile*>(parentStatement)->statements);
	} else if(parentStatement && parentStatement->type == Statement::DoWhile) {
        stmtList = &(static_cast<StmtDoWhile*>(parentStatement)->statements);
	} else if(parentStatement && parentStatement->type == Statement::Switch) {
        stmtList = &(static_cast<StmtSwitch*>(parentStatement)->statements);
	} else if(parentStatement && (parentStatement->type == Statement::Continue ||
		parentStatement->type == Statement::Case || parentStatement->type == Statement::Default ||
		parentStatement->type == Statement::Return || parentStatement->type == Statement::LocalVariable ||
		parentStatement->type == Statement::ExpressionWithSemicolon ||
        parentStatement->type == Statement::BatchCalculation ||
        parentStatement->type == Statement::Assemble)) {
		return nullptr;
	} else if(parentStatement && parentStatement->type == Statement::For) {
		StmtFor* stmtTemp = static_cast<StmtFor*>(parentStatement);
        auto iter1 = find(stmtTemp->initial.begin(), stmtTemp->initial.end(), this);
        auto iter2 = find(stmtTemp->statements.begin(), stmtTemp->statements.end(), this);
        auto iter3 = find(stmtTemp->iterator.begin(), stmtTemp->iterator.end(), this);
		if(iter1 == stmtTemp->initial.end() && iter2 == stmtTemp->statements.end() && iter3 == stmtTemp->iterator.end())
			return nullptr;
		
		if(iter1 != stmtTemp->initial.end()) {
            stmtList = &(stmtTemp->initial);
		} else if(iter2 != stmtTemp->statements.end()) {
            stmtList = &(stmtTemp->statements);
		} else if(iter3 != stmtTemp->iterator.end()) {
            stmtList = &(stmtTemp->iterator);
		}
	} else if(parentStatement){
        stmtList = &(parentStatement->childStatements);
	}
    if(stmtList) {
        auto iter = find(stmtList->begin(), stmtList->end(), this);
        if(iter == stmtList->end() || iter == stmtList->begin())
			return nullptr;
		iter--;
		return *iter;
    }
	return nullptr;
}

Expression* Statement::getValueExpression() const
{
    if(this->type == Statement::If) {
		StmtIf* stmtTemp = static_cast<StmtIf*>(const_cast<Statement*>(this));
        return stmtTemp->condition;
	} else if(this->type == Statement::ElseIf) {
		StmtElseIf* stmtTemp = static_cast<StmtElseIf*>(const_cast<Statement*>(this));
        return stmtTemp->condition;
	} else if(this->type == Statement::While) {
		StmtWhile* stmtTemp = static_cast<StmtWhile*>(const_cast<Statement*>(this));
        return stmtTemp->condition;
	} else if(this->type == Statement::Switch) {
		StmtSwitch* stmtTemp = static_cast<StmtSwitch*>(const_cast<Statement*>(this));
        return stmtTemp->condition;
	} else if(this->type == Statement::Case) {
		StmtCase* stmtTemp = static_cast<StmtCase*>(const_cast<Statement*>(this));
        return stmtTemp->value;
	} else if(this->type == Statement::Return) {
		StmtReturn* stmtTemp = static_cast<StmtReturn*>(const_cast<Statement*>(this));
        return stmtTemp->expression;
	} else if(this->type == Statement::LocalVariable) {
		StmtLocalVariable* stmtTemp = static_cast<StmtLocalVariable*>(const_cast<Statement*>(this));
        return stmtTemp->defaultValue;
	} else if(this->type == Statement::ExpressionWithSemicolon) {
		StmtExpression* stmtTemp = static_cast<StmtExpression*>(const_cast<Statement*>(this));
        return stmtTemp->expression;
	} else if(this->type == Statement::DoWhile) {
		StmtDoWhile* stmtTemp = static_cast<StmtDoWhile*>(const_cast<Statement*>(this));
        return stmtTemp->condition;
	} else if(this->type == Statement::For) {
		StmtFor* stmtTemp = static_cast<StmtFor*>(const_cast<Statement*>(this));
        return stmtTemp->condition;
	}
    return nullptr;
}

std::vector<Statement*>* Statement::getStatementList() const
{
    if(this->type == Statement::If) {
		StmtIf* stmtTemp = static_cast<StmtIf*>(const_cast<Statement*>(this));
        return &(stmtTemp->statements);
	} else if(this->type == Statement::ElseIf) {
		StmtElseIf* stmtTemp = static_cast<StmtElseIf*>(const_cast<Statement*>(this));
        return &(stmtTemp->statements);
	} else if(this->type == Statement::Else) {
		StmtElse* stmtTemp = static_cast<StmtElse*>(const_cast<Statement*>(this));
        return &(stmtTemp->statements);
	} else if(this->type == Statement::While) {
		StmtWhile* stmtTemp = static_cast<StmtWhile*>(const_cast<Statement*>(this));
        return &(stmtTemp->statements);
	} else if(this->type == Statement::Switch) {
		StmtSwitch* stmtTemp = static_cast<StmtSwitch*>(const_cast<Statement*>(this));
        return &(stmtTemp->statements);
	} else if(this->type == Statement::DoWhile) {
		StmtDoWhile* stmtTemp = static_cast<StmtDoWhile*>(const_cast<Statement*>(this));
        return &(stmtTemp->statements);
	} else if(this->type == Statement::For) {
		StmtFor* stmtTemp = static_cast<StmtFor*>(const_cast<Statement*>(this));
        return &(stmtTemp->statements);
	} else {
	    return &(const_cast<Statement*>(this)->childStatements);
	}
    return nullptr;
}

void Statement::print() const
{
	for (int i = 0; i < this->depth - 1; i++) 
		cout << "    ";
	if (this->type >= 0)
	{
		cout << statementTypeName[this->type] << ": ";
	}
    if(this->type == Statement::LocalVariable)
    {
        StmtLocalVariable* stmt = static_cast<StmtLocalVariable*>(const_cast<Statement*>(this));
        cout << stmt->dataType << " " << stmt->name << " = ";
    }
	for (int i = 0; i < this->innerExpressions.size(); i++)
	{
		if(this->type == Statement::LocalVariable && i > 0)
			cout <<this->innerExpressions[i]->expressionStr;
		else if (this->type == Statement::LocalVariable && i == 0)
			cout << this->innerExpressions[i]->expressionStr << " ";
		else
			cout << this->innerExpressions[i]->expressionStr << "  ";
	}
	cout << '\n';
	for (int i = 0; i < this->childStatements.size(); i++)
	{
		childStatements[i]->print();
	}
}

void Statement::release()
{
	for(int i = 0;i < childStatements.size();i++)
	{
		childStatements[i]->release();
        delete childStatements[i];
	}
	for(int i = 0;i < innerExpressions.size();i++)
	{
		innerExpressions[i]->release();
        delete innerExpressions[i];
	}
    //comment = true;
	//delete this;
}

std::vector<void*> Statement::getDomain() const
{
    vector<void*> ret;
    if(this->parentStatement)
        ret = this->parentStatement->getDomain();
    else if(this->parentILObject)
        ret = this->parentILObject->getDomain();

    if(this->type == Statement::DoWhile || this->type == Statement::Else || this->type == Statement::ElseIf ||
        this->type == Statement::For || this->type == Statement::If || this->type == Statement::Switch)
    {
        ret.push_back(const_cast<Statement*>(this));
    }

    return ret;
}
