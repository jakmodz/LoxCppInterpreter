//
// Created by 123ja on 8/21/2025.
//

#ifndef INTERPETER_CPP_ENV_H
#define INTERPETER_CPP_ENV_H
#include <map>
#include <string>

#include "Obj.h"
#include "Token.h"


class Env :public std::enable_shared_from_this<Env>
{
public:
	Env();
	Env(std::shared_ptr<Env>enclosing);

	void define(const Token& name,const Obj& obj);

	Obj& get(const Token& name);
	Obj& getAt(int distance,const std::string& name);
	void assign(const Token& name,const Obj& val);
	void assignAt(int distance,const Token& name,const Obj& val);
	std::shared_ptr<Env> ancestor(int distance);
	std::shared_ptr<Env> enclosing;
private:
	std::map<std::string,Obj> values;

};


#endif //INTERPETER_CPP_EN