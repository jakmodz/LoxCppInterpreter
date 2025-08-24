//
// Created by 123ja on 8/23/2025.
//

#ifndef INTERPETER_CPP_CLOCKCALLABLE_H
#define INTERPETER_CPP_CLOCKCALLABLE_H
#include "LoxCallable.h"

class ClockCallable : public LoxCallable
{
public:
	int arity() override;
	Obj call(Interpreter& interpreter, std::vector<Obj> arguments) override;
	std::string toString() override;
};

#endif //INTERPETER_CPP_CLOCKCALLABLE_H