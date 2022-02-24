#include "StmtBatchCalculation.h"

#include "Expression.h"

using namespace std;

map<string, StmtBatchCalculation::OperationType> StmtBatchCalculation::operationTypeStrToTypeMap = {
    pair<string, OperationType>("Add", TypeAdd),
    pair<string, OperationType>("Sub", TypeSub),
    pair<string, OperationType>("Mul", TypeMul),
    pair<string, OperationType>("Div", TypeDiv),
    pair<string, OperationType>("LShr", TypeLShr),
    pair<string, OperationType>("AShr", TypeAShr),
    pair<string, OperationType>("Shl", TypeShl),
    pair<string, OperationType>("And", TypeAnd),
    pair<string, OperationType>("Or", TypeOr),
    pair<string, OperationType>("Xor", TypeXor),
    pair<string, OperationType>("DotMul", TypeDotMul),
    pair<string, OperationType>("CrossMul", TypeCrossMul),
    pair<string, OperationType>("FFT", TypeFFT),
    pair<string, OperationType>("IFFT", TypeIFFT),
    pair<string, OperationType>("DCT", TypeDCT),
    pair<string, OperationType>("IDCT", TypeIDCT),
};

map<StmtBatchCalculation::OperationType, string> StmtBatchCalculation::operationTypeToTypeStrMap = {
    pair<OperationType, string>(TypeAdd, "Add"),
    pair<OperationType, string>(TypeSub, "Sub"),
    pair<OperationType, string>(TypeMul, "Mul"),
    pair<OperationType, string>(TypeDiv, "Div"),
    pair<OperationType, string>(TypeLShr, "LShr"),
    pair<OperationType, string>(TypeAShr, "AShr"),
    pair<OperationType, string>(TypeShl, "Shl"),
    pair<OperationType, string>(TypeAnd, "And"),
    pair<OperationType, string>(TypeOr, "Or"),
    pair<OperationType, string>(TypeXor, "Xor"),
    pair<OperationType, string>(TypeDotMul, "DotMul"),
    pair<OperationType, string>(TypeCrossMul, "CrossMul"),
    pair<OperationType, string>(TypeFFT, "FFT"),
    pair<OperationType, string>(TypeIFFT, "IFFT"),
    pair<OperationType, string>(TypeDCT, "DCT"),
    pair<OperationType, string>(TypeIDCT, "IDCT"),
};

map<string, StmtBatchCalculation::ModifierType> StmtBatchCalculation::modifierTypeStrToTypeMap = {
    pair<string,ModifierType>("None", TypeNone),
    pair<string,ModifierType>("Saturat", TypeSaturate),
    pair<string,ModifierType>("Half", TypeHalf),
    pair<string,ModifierType>("Double", TypeDouble),
    pair<string,ModifierType>("Round", TypeRound),
};

map<StmtBatchCalculation::ModifierType, string> StmtBatchCalculation::modifierTypeToTypeStrMap = {
    pair<ModifierType, string>(TypeNone, "None"),
    pair<ModifierType, string>(TypeSaturate, "Saturat"),
    pair<ModifierType, string>(TypeHalf, "Half"),
    pair<ModifierType, string>(TypeDouble, "Double"),
    pair<ModifierType, string>(TypeRound, "Round"),
};

map<string, StmtBatchCalculation::ShapeType> StmtBatchCalculation::shapeTypeStrToTypeMap = {
    pair<string,ShapeType>("Normal", TypeNormal),
    pair<string,ShapeType>("Long", TypeLong),
    pair<string,ShapeType>("Wide", TypeWide),
    pair<string,ShapeType>("Narrow", TypeNarrow),
};

map<StmtBatchCalculation::ShapeType, string> StmtBatchCalculation::shapeTypeToTypeStrMap = {
    pair<ShapeType, string>(TypeNormal, "Normal"),
    pair<ShapeType, string>(TypeLong, "Long"),
    pair<ShapeType, string>(TypeWide, "Wide"),
    pair<ShapeType, string>(TypeNarrow, "Narrow"),
};

StmtBatchCalculation::StmtBatchCalculation()
{
    this->type = Statement::BatchCalculation;
}

StmtBatchCalculation::StmtBatchCalculation(const Statement* parent)
{
    this->type = Statement::BatchCalculation;
    this->parentStatement = const_cast<Statement*>(parent);
}

StmtBatchCalculation::~StmtBatchCalculation()
{
}

void StmtBatchCalculation::release()
{
    for(int i = 0; i < this->inputs.size(); i++)
    {
        if(this->inputs[i].defaultValue)
        {
            this->inputs[i].defaultValue->release();
            delete this->inputs[i].defaultValue;
        }
    }
    for(int i = 0; i < this->outputs.size(); i++)
    {
        if(this->outputs[i].defaultValue)
        {
            this->outputs[i].defaultValue->release();
            delete this->outputs[i].defaultValue;
        }
    }
    Statement::release();
}
