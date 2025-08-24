//
// Created by 123ja on 8/24/2025.
//

#include "LoxInstance.h"
#include "LoxInstance.h"
#include "LoxFunction.h"
#include "RuntimeError.h"
LoxInstance::LoxInstance(LoxClass klass):klass(klass)
{

}

LoxClass & LoxInstance::getKlass()
{
	return klass;
}

Obj & LoxInstance::get(const Token &name)
{
	if (fields.contains(name.getLexme()))
	{
		return  fields[name.getLexme()];
	}
	auto method = klass.findMethod(name.getLexme());
	if (method != nullptr)
	{
		auto boundMethod = method->bind(shared_from_this());
		fields[name.getLexme()] = Obj(boundMethod);
		return fields[name.getLexme()];
	}


	throw RuntimeError(name,"Undefined property '"+name.getLexme()+"'.");
}

void LoxInstance::set(const Token &name, const Obj &value)
{
	fields[name.getLexme()] = value;
}
