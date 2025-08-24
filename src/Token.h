
#ifndef INTERPETER_CPP_TOKEN_H
#define INTERPETER_CPP_TOKEN_H

#include <string>
#include <variant>







enum class  TokenType
{
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
	// One or two character tokens.
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,
	// Literals.
	IDENTIFIER, STRING, NUMBER,
	// Keywords.
	AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
	PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
	EOF_TOKEN
};



std::string tokenTypeToString(TokenType type);



class Token
{
public:
	Token(TokenType type, const std::string& lexeme,
		 const std::variant<std::nullptr_t,double,std::string,bool>& literal,
		 int line);
	std::string toString();
	const std::string& getLexme() const;
	int getLine() const ;
	TokenType getType() const;
	const std::variant<std::nullptr_t,double,std::string,bool>& getLiteral() const;

private:
	TokenType type;
	std::string lexeme;
	const std::variant<std::nullptr_t,double,std::string,bool> literal;
	int line;

	std::string literalToString();
};


#endif //INTERPETER_CPP_TOKEN_H