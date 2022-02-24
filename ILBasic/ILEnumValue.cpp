#include "ILEnumValue.h"
using namespace std;

ILEnumValue::ILEnumValue()
{
    this->objType = ILObject::TypeEnumValue;
}

ILEnumValue::ILEnumValue(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeEnumValue;
}

ILEnumValue::~ILEnumValue()
{
}

void ILEnumValue::release()
{

}
