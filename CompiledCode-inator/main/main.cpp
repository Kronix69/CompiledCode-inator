#include "main.h"
#include <ccinator/parser/parser.h>
#include <iostream>

int main()
{
	std::string out;
	std::getline(std::cin, out);

	ccinator::parser parser(out);

	std::cout << parser.parse() << "\n";

	return 0;
}