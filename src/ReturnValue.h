//
// ReturnValue.h - Exception for early return from functions
//

#ifndef INTERPETER_CPP_RETURNVALUE_H
#define INTERPETER_CPP_RETURNVALUE_H

#include "Obj.h"
#include <exception>

class ReturnValue : public std::exception
{
public:
	Obj value;

	explicit ReturnValue(Obj value);
	const char* what() const noexcept override;
};

#endif //INTERPETER_CPP_RETURNVALUE_H