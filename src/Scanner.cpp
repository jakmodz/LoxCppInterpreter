//
// Created by 123ja on 8/11/2025.
//

#include "Scanner.h"
#include <format>
#include <unordered_map>
#include "lox.h"

Keywords::Keywords()
{
    keywords["and"]    = TokenType::AND;
    keywords["class"]  = TokenType::CLASS;
    keywords["else"]   = TokenType::ELSE;
    keywords["false"]  = TokenType::FALSE;
    keywords["for"]    = TokenType::FOR;
    keywords["fun"]    = TokenType::FUN;
    keywords["if"]     = TokenType::IF;
    keywords["nil"]    = TokenType::NIL;
    keywords["or"]     = TokenType::OR;
    keywords["print"]  = TokenType::PRINT;
    keywords["return"] = TokenType::RETURN;
    keywords["super"]  = TokenType::SUPER;
    keywords["this"]   = TokenType::THIS;
    keywords["true"]   = TokenType::TRUE;
    keywords["var"]    = TokenType::VAR;
    keywords["while"]  = TokenType::WHILE;
}

Scanner::Scanner(const std::string &src)
{
    this->src = src;
    this->start = 0;
    this->current = 0;
    this->line = 1;
}

bool Scanner::isAtEnd()
{
    return current >= src.length();
}

char Scanner::advance()
{
    return src.at(current++);
}

void Scanner::addToken(TokenType type)
{
    addToken(type, nullptr);
}

void Scanner::addToken(TokenType type, const std::variant<std::nullptr_t, double, std::string, bool>& literal)
{
    std::string text = src.substr(start, current - start);
    tokens.push_back(Token(type, text, literal, line));
}

bool Scanner::match(char expected)
{
    if (isAtEnd()) return false;
    if (src.at(current) != expected) return false;

    current++;
    return true;
}

char Scanner::peek()
{
    if (isAtEnd()) return '\0';
    return src.at(current);
}

char Scanner::peekNext()
{
    if (current + 1 >= src.length()) return '\0';
    return src.at(current + 1);
}

void Scanner::string()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd())
    {
        error(line, "Unterminated string");
        return;
    }

    // The closing "
    advance();

    // Trim the surrounding quotes
    std::string value = src.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
}

bool Scanner::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Scanner::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

void Scanner::identifier()
{
    while (isAlphaNumeric(peek())) advance();

    std::string text = src.substr(start, current - start);

    auto it = keywords.keywords.find(text);
    if (it != keywords.keywords.end())
    {
        addToken(it->second);
    }
    else
    {
        addToken(TokenType::IDENTIFIER);
    }
}

void Scanner::number()
{
    while (isDigit(peek())) advance();

    // Look for a fractional part
    if (peek() == '.' && isDigit(peekNext()))
    {
        // Consume the "."
        advance();
        while (isDigit(peek())) advance();
    }

    addToken(TokenType::NUMBER, std::stod(src.substr(start, current - start)));
}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }

    tokens.push_back(Token(TokenType::EOF_TOKEN, "", nullptr, this->line));
    return this->tokens;
}

void Scanner::scanToken()
{
    char c = advance();
    switch (c)
    {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('/'))
            {
                // A comment goes until the end of the line
                while (peek() != '\n' && !isAtEnd()) advance();
            }
            else
            {
                addToken(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\t':
        case '\r':
            // Ignore whitespace
            break;
        case '\n':
            line++;
            break;
        case '"':
            string();
            break;
        default:
            if (isDigit(c))
            {
                number();
            }
            else if (isAlpha(c))
            {
                identifier();
            }
            else
            {
                error(line, "Unexpected character");
            }
            break;
    }
}