//
// Created by 123ja on 8/17/2025.
//

#include "lox.h"
#include  <vector>
#include  "Scanner.h"
#include "Token.h"
#include "Parser.h"
#include "Resolver.h"
#include  "AstPrinter.h"
#include "Interpreter.h"
bool hadError = false;
bool hadRuntimeError = false;
Interpreter interpreter;
void report(int line ,const std::string& where,const std::string& message)
{
	std::cout<<"[line "<<line<<"] Error"<<where<<": "<<message<<std::endl;
	hadError = true;
}

void error(int line, const std::string& message)
{
	report(line,"",message);
}


void run(std::string src)
{
	Scanner scanner(src);
	std::vector<Token> tokens = scanner.scanTokens();
	Parser parser(tokens);
	auto statements = parser.parse();

	if (hadError) return;

	Resolver resolver = Resolver(interpreter);
	resolver.resolve(statements);

	if (hadError) return;

	interpreter.interpret(statements);

	if (hadRuntimeError) exit(70);

}

void run_prompt()
{
	while (true)
	{
		std::cout<<">\n";
		std::string line;
		std::cin>> line;
		if (line.empty()) break;
		run(line);


	}
}

void runFile(char* path)
{
	std::ifstream ifs(path);
	if (!ifs) {
		std::cerr << "Failed to open file: " << path << std::endl;
		return;
	}

	std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	run(content);
}

void errorL(Token token, const std::string &message)
{
	if (token.getType() == TokenType::EOF_TOKEN)
	{
		report(token.getLine(),"at end ",message);
	}
	else
	{
		report(token.getLine(), " at '" + token.getLexme() + "'", message);
	}
}
void runtimeError(const RuntimeError &error)
{
	std::cerr<<error.what()<<"\n[line"<<error.getToken().getLine()<<"]";
	hadRuntimeError = true;
}