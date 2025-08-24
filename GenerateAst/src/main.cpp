//
// Created by 123ja on 8/19/2025.
//

#include <iostream>
#include  <string>
#include  <vector>
#include <sstream>
#include <fstream>
#include <cctype>
#include  <algorithm>

std::string& ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
	return s;
}

std::string& rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
	return s;
}

std::string& trim(std::string &s) {
	return ltrim(rtrim(s));
}
std::string toLower(std::string s)
{
	for(auto& c : s)
	{
		c = std::tolower(c);
	}
	return  s;
}
std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(s);
	std::string token;
	while (std::getline(ss, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}
void defineVisitor(std::ofstream& file,const std::string& baseName,std::vector<std::string>types)
{
	file<<"template <typename R>"<<std::endl;
	file<<"class Visitor"<<baseName<<std::endl;
	file<<"{"<<std::endl;
	file<<"public:"<<std::endl;
	for (const std::string& type: types)
	{


		std::string typeName = split(type,':')[0];
		trim(typeName);
		file<<"virtual R visit"<<typeName<<baseName<<"( "<<baseName<<typeName<<"& "<<toLower(baseName)<<") = 0;"<<std::endl;
	}
	file<<"};"<<std::endl;
}
void defineType(std::ofstream& file,std::string baseName,std::string className,std::string fieldList)
{
	trim(className);

	file<<"class "<<baseName<<className<<" : public "<<baseName<<std::endl;
	file<<"{"<<std::endl;
	file<<"public:"<<std::endl<<std::endl;
	//constructor
	file<<baseName<<className<<"("<<fieldList<<"):"<<std::endl;
	std::vector<std::string> fields = split(fieldList,',');

	for (size_t i = 0; i < fields.size(); ++i)
	{
		auto gustyn = split(fields[i], ' ');
		std::string name = gustyn[2];
		file << name << "(" << name << ")";

		if (i != fields.size() - 1)
		{
			file << ",";
		}
	}
	file<<"{"<<std::endl;

	file<<"}"<<std::endl;
	// end of constructor

	//fields

	for (const std::string& field: fields)
	{
		file<<field<<";"<<std::endl;
	}
	file<<"\n\n";

	file<<"template <typename R>"<<std::endl;
	file<<"R accept(Visitor"<<baseName<<"<R>& visitor)"<<std::endl;
	file<<"{"<<std::endl;
	file<<"return visitor.visit"<<className<<baseName<<"(*this);"<<std::endl;
	file<<"}"<<std::endl;
	file<<"};"<<std::endl;;
}


void defineAst(
	const std::string& outputDir,
	const std::string& baseName,
	const std::vector<std::string>& types)
{

	std::string path = outputDir + "/"+baseName+".h";
	std::ofstream file(path);
	file<<"#pragma once"<<std::endl;
	file<<"#include <memory>"<<std::endl;
	file<<"#include <vector>"<<std::endl;
	file<<"#include <exception>"<<std::endl;
	file<<"#include \"Obj.h \""<<std::endl;
	file<<"#include \"Token.h\""<<std::endl;
	file<<"using namespace std;"<<std::endl;
	file<<"template <typename R>"<<std::endl;
	file<<"class Visitor"<<baseName<<";"<<std::endl<<std::endl;
	for (const std::string& type: types)
	{
		std::string className = split(type,':')[0];
		std::string fields = split(type,':')[1];
		file<<"class "<<baseName<<className<<";"<<std::endl;
	}


	file<<" class "<<baseName<<std::endl;

	file<<"{"<<std::endl;
	file<<"public:"<<std::endl;
	file<<"virtual ~"<<baseName<<"() = default;"<<std::endl;
	file<<"template <typename R>"<<std::endl;
	file<<" R accept(Visitor"<<baseName<<"<R>& visitor);"<<std::endl;

	file<<"};"<<std::endl;

	for (const std::string& type: types)
	{
		std::string className = split(type,':')[0];
		std::string fields = split(type,':')[1];
		defineType(file,baseName,className,fields);
	}

	defineVisitor(file,baseName,types);

	file<<"template <typename R>"<<std::endl;
	file<<" R "<<baseName<<"::accept(Visitor"<<baseName<<"<R>& visitor)"<<std::endl;
	file<<"{"<<std::endl;
	for (const std::string& type: types)
	{
		std::string className = split(type,':')[0];
		trim(className);
		std::string fields = split(type,':')[1];

		file<<"if (auto* expr = dynamic_cast<"<<baseName<<className<<"*>(this)){"<<std::endl;
		file<<"\rreturn visitor.visit"<<className<<baseName<<"(*expr);}"<<std::endl;
	}
	file<<"throw std::logic_error(\"Unknown expression type\");"<<std::endl;
	file<<"}"<<std::endl;
	file.close();
}


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout<<"usage: generate_ast <output directory>";
		exit(64);
	}

	std::string outputDir = argv[1];
	defineAst(outputDir,"Expr",{
		  "Assign : Token name, shared_ptr<Expr> value",
		  "Binary   : shared_ptr<Expr> left, Token op, shared_ptr<Expr> right",
		  "Grouping : shared_ptr<Expr> expression",
		  "Literal  : Obj value",
		  "Logical: shared_ptr<Expr> left, Token op, shared_ptr<Expr> right",
		  "Unary    : Token op, shared_ptr<Expr> right",
		  "Variable : Token name",
		  "Call     : shared_ptr<Expr> calle, Token paren, vector<shared_ptr<Expr>> arguments",
			"Get    : shared_ptr<Expr> object, Token name",
			"Set   : shared_ptr<Expr> object, Token name, shared_ptr<Expr> value",
			"This  : Token keyword",
			"Super : Token keyword, Token method"
	});

	defineAst(outputDir,"Stmt",{
	"Expression : shared_ptr<Expr> expression",
	"Print	   : shared_ptr<Expr> expression",
	"Var		   : Token name, shared_ptr<Expr> initializer",
		"Class  : Token name, shared_ptr<ExprVariable> superclass, vector<shared_ptr<StmtFunction>> methods",
	"Block	   : vector<shared_ptr<Stmt>> statements",
	"if	   : shared_ptr<Expr> condition, shared_ptr<Stmt> thenBranch, shared_ptr<Stmt> elseBranch",
	"while  : shared_ptr<Expr> condition, shared_ptr<Stmt> body",
		"Return     : Token keyword, shared_ptr<Expr> value",
		"Function : Token name, vector<Token> params, vector<shared_ptr<Stmt>> body"
	});
}
