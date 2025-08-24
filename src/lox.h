//
// Created by 123ja on 8/17/2025.
//

#ifndef INTERPETER_CPP_LOX_H
#define INTERPETER_CPP_LOX_H
#include  <iostream>
#include  <string>
#include <fstream>

#include "RuntimeError.h"
#include  "Token.h"

void report(int line ,const std::string& where,const std::string& message);
void error(int line, const std::string& message);

void run(std::string src);

void run_prompt();



void runFile(char* path);
void errorL(Token token,const std::string& message);
void runtimeError(const RuntimeError& error);


#endif //INTERPETER_CPP_LOX_H
