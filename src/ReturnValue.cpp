//
// ReturnValue.cpp - Exception for early return from functions
//

#include "ReturnValue.h"

ReturnValue::ReturnValue(Obj value) : value(value)
{
}

const char* ReturnValue::what() const noexcept
{
	return "Function return";
}