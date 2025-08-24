//
// Created by 123ja on 8/19/2025.
//

#pragma once

#include <memory>
#include <variant>
#include <string>




// Forward declaration
class LoxCallable;
class LoxInstance;
class LoxFunction;
enum Type
{
	Num,
	Str,
	Bool,
	Null,
	Fn,
	Class,
};

class Obj
{
public:
	Obj(const std::variant<std::nullptr_t, double, std::string, bool>& variant);
	Obj(const std::variant<std::nullptr_t, double, std::string, bool>& variant, Type type);
	Obj(std::shared_ptr<LoxCallable> callable);
	Obj(std::shared_ptr<LoxInstance> instance);
	Obj(std::shared_ptr<LoxFunction> function);

	Obj();
	Obj(const Obj &other);
	explicit Obj(double value);
	explicit Obj(const std::string& value);
	explicit Obj(bool value);
	explicit Obj(std::nullptr_t);
	Obj& operator=(const Obj& other);
	~Obj() = default;

	Type getType() const;
	double getNumber() const;
	const std::string& getString() const;
	bool getBoolean() const;
	std::shared_ptr<LoxCallable> getCallable() const;
	std::shared_ptr<LoxInstance> getInstance() const;
	bool isTypeOf(Type type) const;

private:
	std::variant<std::nullptr_t, double, std::string, bool> value;
	std::shared_ptr<LoxCallable> callable;
	std::shared_ptr<LoxInstance> instance = nullptr;
	Type current;
};
