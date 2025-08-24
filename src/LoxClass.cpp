//
// Created by 123ja on 8/24/2025.
//

#include "LoxClass.h"
#include <memory>

#include "LoxFunction.h"
#include "LoxInstance.h"



LoxClass::LoxClass(const std::string &name, std::shared_ptr<LoxClass> klass,
	const std::map<std::string, std::shared_ptr<LoxFunction>> &methods)
:name(name),methods(methods)
{
	this->superclass = klass;
}

int LoxClass::arity()
{
	auto initializer = findMethod("init");
	if (initializer == nullptr)
	{
		return 0;
	}
	return initializer->arity();
}

Obj LoxClass::call(Interpreter &interpreter, std::vector<Obj> arguments)
{
	std::shared_ptr<LoxInstance> loxInstance = std::make_shared<LoxInstance>(*this);
	auto initializer = findMethod("init");
	if (initializer != nullptr)
	{
		initializer->bind(loxInstance)->call(interpreter, arguments);
	}
	return Obj(loxInstance);
}

shared_ptr<LoxFunction> LoxClass::findMethod(const std::string &name)
{
	if (methods.contains(name))
	{
		return methods[name];
	}
	if (superclass != nullptr)
	{
		return superclass->findMethod(name);
	}
	return nullptr;
}

std::string LoxClass::toString()
{
	return name;
}
