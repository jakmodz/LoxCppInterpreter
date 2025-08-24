//
// Created by 123ja on 8/21/2025.
//

#include "Env.h"

#include <iostream>

#include "RuntimeError.h"

Env::Env(): values()
{
	enclosing = nullptr;
}

Env::Env(std::shared_ptr<Env> enclosing):enclosing(enclosing)
{

}

void Env::define(const Token &name, const Obj &obj)
{

	values[name.getLexme()] = obj;
}

Obj & Env::get(const Token& name)
{
	if (values.contains(name.getLexme()))
	{
		return values.at(name.getLexme());
	}
	if (enclosing != nullptr)
	{
		return enclosing->get(name);
	}

	throw RuntimeError(name,"undefined variable'" + name.getLexme()+"'.");
}

Obj & Env::getAt(int distance, const std::string &name)
{
	return ancestor(distance)->values.at(name);
}

void Env::assign(const Token &name, const Obj &val)
{
	if (values.contains(name.getLexme()))
	{
		values[name.getLexme()] = val;
		return;
	}
	if (enclosing != nullptr)
	{
		enclosing->assign(name,val);
		return;
	}
	throw RuntimeError(name,"undefined variable'" + name.getLexme()+"'.");
}

void Env::assignAt(int distance, const Token &name, const Obj &val)
{
	ancestor(distance)->values[name.getLexme()] = val;
}

std::shared_ptr<Env> Env::ancestor(int distance)
{
	auto env = shared_from_this();
	for (int i = 0; i < distance; i++)
	{
		if (env->enclosing != nullptr)
		{
			env = env->enclosing;
		}
		else
		{
			break;
		}
	}
	return env;
}