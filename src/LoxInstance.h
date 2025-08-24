//
// Created by 123ja on 8/24/2025.
//

#ifndef INTERPETER_CPP_LOXINSTANCE_H
#define INTERPETER_CPP_LOXINSTANCE_H
#include "LoxClass.h"
#include <map>
#include "Obj.h"
class LoxInstance: public std::enable_shared_from_this<LoxInstance>
{
public:
	LoxInstance(LoxClass klass);

	LoxClass& getKlass();
	Obj& get(const Token& name);
	void set(const Token& name,const Obj& value);
private:
	LoxClass klass;
	std::map<std::string,Obj> fields;
};


#endif //INTERPETER_CPP_LOXINSTANCE_H