#include  <iostream>
#include  <string>
#include  "lox.h"



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
