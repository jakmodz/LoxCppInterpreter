//
// Created by 123ja on 8/21/2025.
//

#ifndef INTERPETER_CPP_RUNTIMEERROR_H
#define INTERPETER_CPP_RUNTIMEERROR_H
#include <stdexcept>

#include "Token.h"


class RuntimeError: std::runtime_error
{
public:
	RuntimeError(const Token& token,const std::string& message);
	const Token& getToken() const;
	const char* what() const noexcept override;

private:
	Token token;
	std::string msg;
};


#endif //INTERPETER_CPP_RUNTIMEERROR_H