//
// Created by 123ja on 8/22/2025.
//

#ifndef INTERPETER_CPP_LOXCALLABLE_H
#define INTERPETER_CPP_LOXCALLABLE_H
#include "Obj.h"
#include "Interpreter.h"

class Interpreter;

class LoxCallable
{
public:
	LoxCallable() = default;
	virtual ~LoxCallable() = default;
	virtual int arity() = 0;
	virtual Obj call(Interpreter& interpreter, std::vector<Obj> arguments) = 0;
	virtual std::string toString() { return "<callable>"; }
};



#endif //INTERPETER_CPP_LOXCALLABLE_H
