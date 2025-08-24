//
// LoxFunction.cpp - User-defined function implementation
//

#include "LoxFunction.h"
#include "Interpreter.h"
#include "ReturnValue.h"

LoxFunction::LoxFunction(std::shared_ptr<StmtFunction> declaration, std::shared_ptr<Env> closure,bool isInitializer)
	: declaration(declaration), closure(closure)
{
	this->isInitializer = isInitializer;
}

std::shared_ptr<LoxFunction> LoxFunction::bind(std::shared_ptr<LoxInstance> instance)
{
	std::shared_ptr<Env> environment = std::make_shared<Env>(closure);
	environment->define(Token(TokenType::IDENTIFIER, "this", nullptr, 0), Obj(instance));
	return std::make_shared<LoxFunction>(declaration, environment, isInitializer);
}

int LoxFunction::arity()
{
	return declaration->params.size();
}

Obj LoxFunction::call(Interpreter& interpreter, std::vector<Obj> arguments)
{

	std::shared_ptr<Env> environment = std::make_shared<Env>(closure);


	for (size_t i = 0; i < declaration->params.size(); i++) {
		environment->define(declaration->params[i], arguments[i]);
	}

	try
	{
		interpreter.executeBlock(declaration->body, environment);
	}
	catch (const ReturnValue& returnValue)
	{
		if (isInitializer) return closure->getAt(0, "this");
		return returnValue.value;
	}
	if (isInitializer) return closure->getAt(0, "this");
	return Obj();
}

std::string LoxFunction::toString()
{
	return "<fn " + declaration->name.getLexme() + ">";
}