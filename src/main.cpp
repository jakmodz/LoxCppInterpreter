#include  <iostream>
#include  <string>
#include <fstream>
#include  "lox.h"
#include "Expr.h"
#include "Obj.h"
#include "memory"
#include  "AstPrinter.h"
#include "Obj.h"    // Use the safe version above
#include "Env.h"    // Use the safe version above

int main(int argc, char *argv[])
{


	runFile("../text.lox");

	if (argc > 1)
	{
		std::cout<<"Usage jlox [script]\n";
		exit(64);
	}
	else if (argc ==  1)
	{
		runFile(argv[0]);
	}
	else
	{
		run_prompt();
	}

}
