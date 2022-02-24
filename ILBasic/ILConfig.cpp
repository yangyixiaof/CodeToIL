#include "ILConfig.h"

ILConfig::ILConfig()
{
    this->objType = ILObject::TypeConfig;
}

ILConfig::ILConfig(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeConfig;
}

ILConfig::~ILConfig()
{
}

