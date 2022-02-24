#include "ILState.h"
using namespace std;

ILState::ILState()
{
    this->objType = ILObject::TypeState;
}

ILState::ILState(const ILObject* parent)
{
    this->parent = const_cast<ILObject*>(parent);
    this->objType = ILObject::TypeState;
}

ILState::~ILState()
{
}

void ILState::release()
{
    //comment = true;
	//delete this;
}
