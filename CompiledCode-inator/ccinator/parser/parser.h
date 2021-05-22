#pragma once
#include "../lexer/lexer.h"

namespace ccinator
{
	class parser
	{
		std::string content;

	public:
		std::string parse();
		
		parser(std::string content) : content(content) {}
	};
}
