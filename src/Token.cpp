//
// Created by 123ja on 8/9/2025.
//

#include "Token.h"
#include <iostream>
std::string tokenTypeToString(TokenType type)
{
	switch (type)
	{
		case TokenType::LEFT_PAREN: return "LEFT_PAREN";
		case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
		case TokenType::LEFT_BRACE: return "LEFT_BRACE";
		case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
		case TokenType::COMMA: return "COMMA";
		case TokenType::DOT: return "DOT";
		case TokenType::MINUS: return "MINUS";
		case TokenType::PLUS: return "PLUS";
		case TokenType::SEMICOLON: return "SEMICOLON";
		case TokenType::SLASH: return "SLASH";
		case TokenType::STAR: return "STAR";
		case TokenType::BANG: return "BANG";
		case TokenType::BANG_EQUAL: return "BANG_EQUAL";
		case TokenType::EQUAL: return "EQUAL";
		case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
		case TokenType::GREATER: return "GREATER";
		case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
		case TokenType::LESS: return "LESS";
		case TokenType::LESS_EQUAL: return "LESS_EQUAL";
		case TokenType::IDENTIFIER: return "IDENTIFIER";
		case TokenType::STRING: return "STRING";
		case TokenType::NUMBER: return "NUMBER";
		case TokenType::AND: return "AND";
		case TokenType::CLASS: return "CLASS";
		case TokenType::ELSE: return "ELSE";
		case TokenType::FALSE: return "FALSE";
		case TokenType::FOR: return "FOR";
		case TokenType::FUN: return "FUN";
		case TokenType::IF: return "IF";
		case TokenType::NIL: return "NIL";
		case TokenType::OR: return "OR";
		case TokenType::PRINT: return "PRINT";
		case TokenType::RETURN: return "RETURN";
		case TokenType::SUPER: return "SUPER";
		case TokenType::THIS: return "THIS";
		case TokenType::TRUE: return "TRUE";
		case TokenType::VAR: return "VAR";
		case TokenType::WHILE: return "WHILE";
		case TokenType::EOF_TOKEN: return "EOF";
		default: return "UNKNOWN";
	}
}

Token::Token(TokenType type, const std::string &lexeme,
	const std::variant<std::nullptr_t, double, std::string, bool> &literal, int line): literal(literal)
{
	this->line = line;
	this->lexeme =lexeme;
	this->type = type;

}



std::string Token::toString()
{
	return tokenTypeToString(this->type)+" "+lexeme+" "+literalToString();
}

const std::string & Token::getLexme() const
{
	return lexeme;
}

int Token::getLine() const
{
	return line;
}

TokenType Token::getType() const
{
	return type;
}

const std::variant<std::nullptr_t, double, std::string, bool> &Token::getLiteral() const
{
	return this->literal;
}

std::string Token::literalToString()
{
	if (std::holds_alternative<double>(this->literal))
	{
		return std::to_string((std::get<double>(this->literal)));
	}
	else if (std::holds_alternative<std::string>(this->literal))
	{
		return std::get<std::string>(this->literal);
	}
	else
	{
		return std::string("NULL");
	}
}
