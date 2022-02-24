#pragma once
#include <map>

#include "Statement.h"



class StmtBatchCalculation:
	public Statement
{
public:
    StmtBatchCalculation();
    explicit StmtBatchCalculation(const Statement* parent);
    ~StmtBatchCalculation() override;

    class StmtBatchCalculationInput
    {
    public:
	    std::string name;
	    std::string type;
	    int isAddress = 0;
	    std::vector<int> arraySize;
        Expression* defaultValue = nullptr;
    };

    class StmtBatchCalculationOutput
    {
    public:
	    std::string name;
	    std::string type;
	    int isAddress = 0;
	    std::vector<int> arraySize;
        Expression* defaultValue = nullptr;
    };

    std::vector<StmtBatchCalculationInput> inputs;
    std::vector<StmtBatchCalculationOutput> outputs;

    enum OperationType
    {
        TypeAdd,
        TypeSub,
        TypeMul,
        TypeDiv,
        TypeLShr,
        TypeAShr,
        TypeShl,
        TypeAnd,
        TypeOr,
        TypeXor,
        TypeDotMul,
        TypeCrossMul,
        TypeFFT,
        TypeIFFT,
        TypeDCT,
        TypeIDCT,
        TypeUnknown,
    };
    OperationType operationType = TypeUnknown;
    static std::map<std::string, OperationType> operationTypeStrToTypeMap;
    static std::map<OperationType, std::string> operationTypeToTypeStrMap;

    std::string dataType;


    enum ModifierType
    {
        TypeNone,
        TypeSaturate,
        TypeHalf,
        TypeDouble,
        TypeRound,
    };
    ModifierType modifierType = TypeNone;
    static std::map<std::string, ModifierType> modifierTypeStrToTypeMap;
    static std::map<ModifierType, std::string> modifierTypeToTypeStrMap;


    enum ShapeType
    {
        TypeNormal,
        TypeLong,
        TypeWide,
        TypeNarrow,
    };
    ShapeType shapeType = TypeNormal;
    static std::map<std::string, ShapeType> shapeTypeStrToTypeMap;
    static std::map<ShapeType, std::string> shapeTypeToTypeStrMap;
    
	void release() override;
};