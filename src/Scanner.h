//
// Created by 123ja on 8/11/2025.
//

#ifndef INTERPETER_CPP_SCANNER_H
#define INTERPETER_CPP_SCANNER_H
#include  <string>
#include <vector>
#include  <map>
#include "Token.h"

class Keywords {
public:
	std::map<std::string, TokenType> keywords;

	Keywords();

};



class Scanner
{
public:
	Scanner(const std::string& src);

	std::vector<Token> scanTokens();

private:
	std::string src;
	std::vector<Token> tokens;
	size_t start;
	size_t current;
	size_t line;
	Keywords keywords;

	void scanToken();
	bool isAtEnd();
	char advance();
	void addToken(TokenType type);
	void addToken(TokenType type,const std::variant<std::nullptr_t,double,std::string,bool>& literal);
	bool match(char excepted);
	char peek();
	char peekNext();
	void string();
	static bool isDigit(char c);
	static bool isAlpha(char c);
	static bool isAlphaNumeric(char c);
	void number();
	void identifier();
};


#endif //INTERPETER_CPP_SCANNER_H