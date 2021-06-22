#include "main.h"
#include <ccinator/parser/parser.h>
#include <iostream>
#include <fstream>

void consoleparse()
{
	std::string str;
	std::getline(std::cin, str);

	ccinator::parser parser(str);

	std::cout << parser.parse();
}


int main()
{
	std::ifstream in("input.in");
	std::ofstream out("output.in");
	std::ofstream chk("check.ck");

	std::string str;

	in.seekg(0, std::ios::end);
	str.reserve(in.tellg());
	in.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());

	ccinator::parser parser(str);

	out << parser.parse();
	chk << "done";

	in.close();
	out.close();
	chk.close();

	return 0;
}