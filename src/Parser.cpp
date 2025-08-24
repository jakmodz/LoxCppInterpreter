//
// Created by 123ja on 8/19/2025.
//

#include "Parser.h"



#include "lox.h"
#include  "Obj.h"
Parser::Parser(const std::vector<Token> &tokens):tokens(tokens)
{
	this->current = 0;
}

std::vector<std::shared_ptr<Stmt>> Parser::parse()
{

	std::vector<shared_ptr<Stmt>> statements;
	while (!isAtEnd())
	{
		auto stmt = declaration();
		statements.push_back(stmt);
	}
	return statements;
}

std::shared_ptr<Stmt> Parser::declaration()
{
	try
	{
		if (match({TokenType::VAR}))
		{
			return varDeclaration();
		}
		if (match({TokenType::CLASS}))
		{
			return classDeclaraion();
		}
		if (match({TokenType::FUN}))
		{
			return function("function");
		}
		if (match({TokenType::RETURN}))
		{
			return returnStatement();
		}
		return statement();
	}
	catch (const ParseError& error)
	{
		synchronize();
	}
	return nullptr;
}

std::shared_ptr<Stmt> Parser::varDeclaration()
{
	Token name = consume(TokenType::IDENTIFIER,"Except variable name");

	std::shared_ptr<Expr> initializer = nullptr;

	if (match({TokenType::EQUAL}))
	{
		initializer = expression();
	}
	consume(TokenType::SEMICOLON,"Expect ';' after variable declaration.");
	return make_shared<StmtVar>(name,initializer);
}

std::shared_ptr<Stmt> Parser::statement()
{
	if (match({TokenType::PRINT}))
	{
		return printStatement();
	}
	else if (match({TokenType::LEFT_BRACE}))
	{
		return std::make_shared<StmtBlock>(block());
	}
	else if ( match({TokenType::IF}))
	{
		return ifStatement();
	}
	else if (match({TokenType::WHILE}))
	{
		return whileStatement();
	}
	else if (match({TokenType::FOR}))
	{
		return forStatement();
	}
	return expresionStatement();
}

std::shared_ptr<Stmt> Parser::printStatement()
{
	std::shared_ptr<Expr> value = expression();
	consume(TokenType::SEMICOLON,"excepted ';' after value ");
	return std::make_shared<StmtPrint>(value);
}


std::shared_ptr<Stmt> Parser::expresionStatement()
{
	std::shared_ptr<Expr> expr = expression();
	consume(TokenType::SEMICOLON,"excepted ';' after  expression.");
	return std::make_shared<StmtExpression>(expr);
}

std::vector<shared_ptr<Stmt>> Parser::block()
{
	std::vector<shared_ptr<Stmt>> statements;

	while (!check({TokenType::RIGHT_BRACE})&& !isAtEnd())
	{
		statements.push_back(declaration());
	}

	consume(TokenType::RIGHT_BRACE,"excpeted '}' after new scope.");
	return statements;
}

shared_ptr<Stmt> Parser::ifStatement()
{
	consume(TokenType::LEFT_PAREN,"Expect '(' after 'if'.");
	shared_ptr<Expr> expr = expression();
	consume(TokenType::RIGHT_PAREN,"Expect ')' after if condition.");

	shared_ptr<Stmt> thenBranch = statement();
	shared_ptr<Stmt> elseBranch = nullptr;
	if (match({TokenType::ELSE}))
	{
		elseBranch = statement();
	}
	return make_shared<Stmtif>(expr,thenBranch,elseBranch);
}

shared_ptr<Stmt> Parser::whileStatement()
{
	consume(TokenType::LEFT_PAREN,"Except '(' after 'while'.");
	shared_ptr<Expr> condition = expression();
	consume(TokenType::RIGHT_PAREN,"except ')' after conditino");
	shared_ptr<Stmt> body = statement();

	return make_shared<Stmtwhile>(condition,body);
}

shared_ptr<Stmt> Parser::forStatement()
{
	consume(TokenType::LEFT_PAREN,"Except '(' after 'for'.");
	shared_ptr<Stmt> initializer;
	if (match({TokenType::SEMICOLON}))
	{
		initializer = nullptr;
	}
	else if (match({TokenType::VAR}))
	{
		initializer = varDeclaration();
	}
	else
	{
		initializer = expresionStatement();
	}

	shared_ptr<Expr> condition = nullptr;
	if (!check(TokenType::SEMICOLON))
	{
		condition= expression();
	}

	consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

	shared_ptr<Expr> increment= nullptr;
	if (!check(TokenType::RIGHT_PAREN))
	{
		increment = expression();
	}
	consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");
	shared_ptr<Stmt> body = statement();


	if (increment != nullptr)
	{
		std::vector<shared_ptr<Stmt>> v ={body,make_shared<StmtExpression>(increment)};
		body = make_shared<StmtBlock>(v);
	}
	if (condition == nullptr)
	{
		condition = make_shared<ExprLiteral>(Obj(true));
	}
	body = make_shared<Stmtwhile>(condition,body);

	if (initializer != nullptr)
	{
		std::vector<shared_ptr<Stmt>> v ={initializer,body};
		body = make_shared<StmtBlock>(v);
	}

	return  body;
}

shared_ptr<Stmt> Parser::classDeclaraion()
{
	auto name = consume(TokenType::IDENTIFIER,"Expect class name.");
	shared_ptr<ExprVariable> superclass = nullptr;
	if (match({TokenType::LESS}))
	{
		consume(TokenType::IDENTIFIER,"Expect superclass name.");
		superclass = make_shared<ExprVariable>(previous());
	}
	consume(TokenType::LEFT_BRACE,"Expect '{' before class body.");
	std::vector<shared_ptr<StmtFunction>> methods;
	while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
	{
		methods.push_back(function("method"));
	}
	consume(TokenType::RIGHT_BRACE,"Expect '}' after class body.");
	return make_shared<StmtClass>(name,superclass,methods);
}

shared_ptr<Stmt> Parser::returnStatement()
{
	Token keyword = previous();
	std::shared_ptr<Expr> value = nullptr;

	if (!check(TokenType::SEMICOLON)) {
		value = expression();
	}

	consume(TokenType::SEMICOLON, "Expect ';' after return value.");
	return std::make_shared<StmtReturn>(keyword, value);
}



shared_ptr<StmtFunction> Parser::function(const std::string& kind)
{
	std::string str = "except "+ kind+" name.";
	Token name = consume(TokenType::IDENTIFIER,str);
	consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
	std::vector<Token> parameters;

	if (!check(TokenType::RIGHT_PAREN)) {
		do {
			if (parameters.size() >= 255) {
				error(peek(), "Can't have more than 255 parameters.");
			}
			parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
		} while (match({TokenType::COMMA}));
	}

	consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
	consume(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");

	std::vector<std::shared_ptr<Stmt>> body = block();

	return std::make_shared<StmtFunction>(name, parameters, body);
}

std::shared_ptr<Expr> Parser::assigment()
{
	std::shared_ptr<Expr> expr = orExpr();

	if (match({TokenType::EQUAL}))
	{
		Token equals = previous();
		std::shared_ptr<Expr> value = assigment();

		if (std::shared_ptr<ExprVariable> v = std::dynamic_pointer_cast<ExprVariable>(expr))
		{
			Token name = v->name;
			return make_shared<ExprAssign>(name, value);
		}
		else if (std::shared_ptr<ExprGet> get = std::dynamic_pointer_cast<ExprGet>(expr))
		{
			return make_shared<ExprSet>(get->object,get->name,value);
		}
		error(equals, "invalid assignment target");
	}
	return expr;
}

std::shared_ptr<Expr> Parser::expression()
{
	return assigment();
}

std::shared_ptr<Expr> Parser::call()
{
	shared_ptr<Expr> expr = primary();
	while (true)
	{
		if (match({TokenType::LEFT_PAREN}))
		{
			expr = finishCall(expr);
		}
		else if (match({TokenType::DOT}))
		{
			Token name = consume(TokenType::IDENTIFIER,"Expect property name after '.'.");
			expr = make_shared<ExprGet>(expr,name);
		}
		else
		{
			break;
		}
	}
	return expr;
}

std::shared_ptr<Expr> Parser::finishCall(shared_ptr<Expr> calle)
{
	std::vector<std::shared_ptr<Expr>> arguments;
	if (!check(TokenType::RIGHT_PAREN))
	{
		do
		{
			if (arguments.size() >= 255)
			{
				error(peek(), "Can't have more than 255 arguments.");
			}
			arguments.push_back(expression());
		}while (match({TokenType::COMMA}));
	}

	Token paren = consume(TokenType::RIGHT_PAREN,"Expect ')' after arguments.");
	return  make_shared<ExprCall>(calle,paren,arguments);
}

bool Parser::match(const std::vector<TokenType>&types)
{
	for (auto type: types)
	{
		if (check(type))
		{
			advance();
			return true;
		}
	}
	return false;
}

std::shared_ptr<Expr> Parser::equality()
{
	std::shared_ptr<Expr> expr= comparison();

	while (match({TokenType::BANG_EQUAL,TokenType::EQUAL_EQUAL}))
	{
		auto operatorToken = previous();
		std::shared_ptr<Expr> right= comparison();
		expr = std::make_shared<ExprBinary>(expr,operatorToken,right);
	}
	return expr;
}


std::shared_ptr<Expr> Parser::comparison()
{
	std::shared_ptr<Expr> expr= term();

	while (match({TokenType::GREATER,TokenType::GREATER_EQUAL,TokenType::LESS,TokenType::LESS_EQUAL}))
	{
		auto operatorToken = previous();
		std::shared_ptr<Expr> right= term();
		expr = std::make_shared<ExprBinary>(expr,operatorToken,right);
	}
	return expr;
}
std::shared_ptr<Expr> Parser::term()
{
	std::shared_ptr<Expr> expr= factor();

	while (match({TokenType::MINUS,TokenType::PLUS}))
	{
		auto operatorToken = previous();
		std::shared_ptr<Expr> right= factor();
		expr = std::make_shared<ExprBinary>(expr,operatorToken,right);
	}
	return expr;
}
std::shared_ptr<Expr> Parser::factor()
{
	std::shared_ptr<Expr> expr = unary();

	while (match({TokenType::SLASH,TokenType::STAR}))
	{
		auto operatorToken = previous();
		std::shared_ptr<Expr> right= unary();
		expr = std::make_shared<ExprBinary>(expr,operatorToken,right);
	}
	return expr;
}

std::shared_ptr<Expr> Parser::unary()
{
	if (match({TokenType::BANG,TokenType::MINUS}))
	{
		auto operatorToken = previous();
		std::shared_ptr<Expr> right= unary();
		return std::make_shared<ExprUnary>(operatorToken,right);
	}
	return call();
}

std::shared_ptr<Expr> Parser::primary()
{
	if (match({TokenType::FALSE})) return make_shared<ExprLiteral>(Obj(false));
	if (match({TokenType::TRUE})) return make_shared<ExprLiteral>(Obj(true));
	if (match({TokenType::NIL})) return make_shared<ExprLiteral>(Obj(nullptr));

	if (match({TokenType::NUMBER,TokenType::STRING}))
	{
		auto literal = previous().getLiteral();

		return make_shared<ExprLiteral>(Obj(literal));
	}
	if (match({TokenType::SUPER}))
	{
		Token keyword = previous();
		consume(TokenType::DOT,"Expect '.' after 'super'.");
		Token method = consume(TokenType::IDENTIFIER,"Expect superclass method name");
		return make_shared<ExprSuper>(keyword,method);
	}
	if (match({TokenType::THIS})) return make_shared<ExprThis>(previous());
	if (match({TokenType::IDENTIFIER}))
	{
		return make_shared<ExprVariable>(previous());
	}

	if (match({TokenType::LEFT_PAREN}))
	{
		std::shared_ptr<Expr> expr = expression();
		consume(TokenType::RIGHT_PAREN,"except ')' after expresion");
		return  make_shared<ExprGrouping>(expr);
	}
	throw error(peek(), "expect expression");
}

std::shared_ptr<Expr> Parser::orExpr()
{
	shared_ptr<Expr> expr = andExpr();


	while (match({TokenType::OR}))
	{
		Token operatorT = previous();
		shared_ptr<Expr> right = andExpr();
		expr = make_shared<ExprLogical>(expr,operatorT,right);
	}
	return expr;
}

std::shared_ptr<Expr> Parser::andExpr()
{
	shared_ptr<Expr> expr = equality();

	while (match({TokenType::AND}))
	{
		Token op = previous();
		shared_ptr<Expr> right = equality();
		expr = make_shared<ExprLogical>(expr,op,right);
	}
	return expr;
}


bool Parser::check(TokenType type)
{
	if (isAtEnd()) return false;
	return peek().getType() == type;
}
const Token& Parser::advance()
{
	if (!isAtEnd()) current++;
	return previous();
}
bool Parser::isAtEnd()
{
	return peek().getType() == TokenType::EOF_TOKEN;
}
const Token& Parser::peek()
{
	return tokens.at(current);
}
const Token& Parser::previous()
{
	return tokens.at(current-1);
}

Token Parser::consume(TokenType type, const std::string &message)
{
	if (check(type)) return advance();

	throw error(peek(),message);
}

ParseError Parser::error(Token token, const std::string &message)
{
	errorL(token,message);
	return ParseError(message);
}

void Parser::synchronize()
{
	advance();

	while (!isAtEnd())
	{
		if (previous().getType() == TokenType::SEMICOLON) return;

		switch (peek().getType())
		{
			case TokenType::CLASS:
			case TokenType::FUN:
			case TokenType::VAR:
			case TokenType::FOR:
			case TokenType::IF:
			case TokenType::WHILE:
			case TokenType::PRINT:
			case TokenType::RETURN:
				return;
		}
		advance();


	}
}
