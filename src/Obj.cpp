//
// Created by 123ja on 8/19/2025.
//

#include "Obj.h"
#include "LoxCallable.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
Obj::Obj(const std::variant<std::nullptr_t, double, std::string, bool> &variant):value(variant)
{
	if (std::holds_alternative<double>(variant))
	{
		current = Type::Num;
	}
	else if (std::holds_alternative<std::string>(variant))
	{
		current = Type::Str;
	}
	else if (std::holds_alternative<bool>(variant))
	{
		current = Type::Bool;
	}
	else
	{
		current = Type::Null;
	}
}

Obj::Obj(const std::variant<std::nullptr_t, double, std::string, bool> &variant, Type type):value(variant),current(type)
{
}

Obj::Obj(std::shared_ptr<LoxCallable> callable)
	: value(nullptr), callable(callable), current(Type::Fn)
{
}

Obj::Obj(std::shared_ptr<LoxInstance> instance): value(nullptr), instance(instance), current(Type::Class)
{

}

Obj::Obj(std::shared_ptr<LoxFunction> function)
	: value(nullptr), callable(std::static_pointer_cast<LoxCallable>(function)), current(Type::Fn)
{
}


std::shared_ptr<LoxCallable> Obj::getCallable() const
{
	if (current == Type::Fn)
	{
		return callable;
	}
	return nullptr;
}

std::shared_ptr<LoxInstance> Obj::getInstance() const
{
	if (instance != nullptr)
	{
		return instance;
	}
	return nullptr;
}



Obj::Obj(const Obj& other)
	: value(other.value), callable(other.callable), instance(other.instance), current(other.current)
{
}
Obj& Obj::operator=(const Obj& other)
{
	if (this != &other) {
		value = other.value;
		callable = other.callable;
		instance = other.instance;
		current = other.current;
	}
	return *this;
}


Obj::Obj():value(nullptr),current(Null)
{

}





Type Obj::getType() const
{
	return current;
}

double Obj::getNumber() const
{
	return std::get<double>(value);
}

const std::string & Obj::getString() const
{
	return std::get<std::string>(value);
}

bool Obj::getBoolean() const
{
	return std::get<bool>(value);
}

bool Obj::isTypeOf(Type type) const
{
	return type == this->current;
}
Obj::Obj(double value) : value(value), callable(nullptr), current(Type::Num)
{
}

Obj::Obj(const std::string& value) : value(value), callable(nullptr), current(Type::Str)
{
}

Obj::Obj(bool value) : value(value), callable(nullptr), current(Type::Bool)
{
}

Obj::Obj(std::nullptr_t) : value(nullptr), callable(nullptr), current(Type::Null)
{
}