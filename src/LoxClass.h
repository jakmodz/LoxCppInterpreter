//
// Created by 123ja on 8/24/2025.
//

#ifndef INTERPETER_CPP_LOXCLASS_H
#define INTERPETER_CPP_LOXCLASS_H
#include  "LoxCallable.h"
#include <string>
class LoxFunction;
class LoxClass : public LoxCallable
{
public:
	LoxClass(const std::string& name, std::shared_ptr<LoxClass> klass, const std::map<std::string, std::shared_ptr<LoxFunction>>& methods);

	int arity() override;

	Obj call(Interpreter &interpreter, std::vector<Obj> arguments) override;

	shared_ptr<LoxFunction> findMethod(const std::string &name);
	std::string toString() override;

private:
	std::string name;
	std::map<std::string, std::shared_ptr<LoxFunction>> methods;
	std::shared_ptr<LoxClass> superclass;
};


#endif //INTERPETER_CPP_LOXCLASS_H