#include "lexer.h"
#include <regex>

namespace ccinator
{
	std::string emptystr("");

	void lexer::split_string()
	{
		std::regex reg("[\\n\\r\\s.]+");

		tokens = std::vector<std::string>(
			     std::sregex_token_iterator(content.begin(), content.end(), reg, -1),
			     std::sregex_token_iterator()
			     );
	}

	std::vector<std::string>& lexer::get_token_list()
	{
		return tokens;
	}

	void lexer::move_index(std::int32_t index)
	{
		this->index += index;
	}

	std::string& lexer::get_token(std::int32_t index)
	{
		if (this->index + index < tokens.size())
			return tokens.at(this->index + index);
		else
			return emptystr;
	}
}

