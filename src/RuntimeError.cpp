//
// Created by 123ja on 8/21/2025.
//

#include "RuntimeError.h"

RuntimeError::RuntimeError(const Token &token, const std::string &message):runtime_error(message),token(token)
{

}

const char * RuntimeError::what() const noexcept
{
	return runtime_error::what();
}

const Token & RuntimeError::getToken() const
{
	return token;
}
