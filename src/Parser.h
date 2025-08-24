
#ifndef INTERPETER_CPP_PARSER_H
#define INTERPETER_CPP_PARSER_H
#include  <vector>
#include  "Token.h"
#include "Expr.h"
#include <memory>
#include <exception>
#include "Stmt.h"

class ParseError : runtime_error
{
public:
	ParseError(const std::string& message):runtime_error(message)
	{

	}
};


class Parser
{
public:
	explicit Parser(const std::vector<Token>& tokens);

	std::vector<std::shared_ptr<Stmt>> parse();
private:
	std::vector<Token>tokens;
	size_t current;


	std::shared_ptr<Stmt> declaration();

	std::shared_ptr<Stmt> varDeclaration();
	std::shared_ptr<Stmt> statement();
	std::shared_ptr<Stmt> printStatement();
	std::shared_ptr<Stmt> expresionStatement();
	std::vector<shared_ptr<Stmt>> block();
	shared_ptr<Stmt> ifStatement();
	shared_ptr<Stmt> whileStatement();
	shared_ptr<Stmt> forStatement();
	shared_ptr<Stmt> classDeclaraion();
	shared_ptr<Stmt> returnStatement();
	shared_ptr<StmtFunction> function(const std::string& kind);
	std::shared_ptr<Expr> expression();
	std::shared_ptr<Expr> call();
	std::shared_ptr<Expr> finishCall(shared_ptr<Expr> calle);
	std::shared_ptr<Expr> equality();
	std::shared_ptr<Expr> assigment();
	std::shared_ptr<Expr> comparison();
	std::shared_ptr<Expr> term();
	std::shared_ptr<Expr> factor();
	std::shared_ptr<Expr> unary();
	std::shared_ptr<Expr> primary();
	std::shared_ptr<Expr> orExpr();
	std::shared_ptr<Expr> andExpr();

	bool match(const std::vector<TokenType>&types);
	bool check(TokenType type);
	const Token& advance();
	bool isAtEnd();
	const Token& peek();

	const Token& previous();
	Token consume(TokenType type, const std::string& message);
	ParseError error(Token token,const std::string& message);
	void synchronize();
};


#endif //INTERPETER_CPP_PARSER_H