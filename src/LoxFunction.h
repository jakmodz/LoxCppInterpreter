//
// Created by 123ja on 8/23/2025.
//

#ifndef INTERPETER_CPP_LOXFUNCTION_H
#define INTERPETER_CPP_LOXFUNCTION_H


#include "LoxCallable.h"
#include "Stmt.h"
#include "Env.h"
#include <memory>
#include "Stmt.h"
class LoxFunction : public LoxCallable
{

public:
	LoxFunction(std::shared_ptr<StmtFunction> declaration, std::shared_ptr<Env> closure,bool isInitializer );
	std::shared_ptr<LoxFunction> bind(std::shared_ptr<LoxInstance> instance);
	int arity() override;
	Obj call(Interpreter& interpreter, std::vector<Obj> arguments) override;
	std::string toString() override;
private:
	std::shared_ptr<StmtFunction> declaration;
	std::shared_ptr<Env> closure;
	bool isInitializer ;
};

#endif //INTERPETER_CPP_LOXFUNCTION_H