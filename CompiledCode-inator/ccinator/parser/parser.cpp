#include "parser.h"
#include <numeric>
#include <algorithm>

namespace ccinator
{
	std::string to_lower(const std::string& token)
	{
		std::string result = token;

		std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });

		return result;
	}

	void verify_tobe(lexer& lexer, std::size_t& index, std::string& result, bool& success)
	{
		if (success)
			return;

		auto& token = lexer.get_token(index);

		if (to_lower(token) == "i")
		{
			auto& self_cont = lexer.get_token(index + 1);

			if (to_lower(self_cont) == "am")
			{
				result = "im is";
				index ++;
				success = true;
			}
			else if (to_lower(self_cont) == "want")
			{
				result = "im is";
				success = true;
			}
		}

		success = false;
	}

	void check_preposition(lexer& lexer, std::size_t& index, std::string& result, bool& success)
	{
		if (success)
			return;

		auto& token = lexer.get_token(index);

		if (to_lower(token) == "to")
		{
			auto& self_cont = lexer.get_token(index + 1);

			if (to_lower(self_cont) == "the")
			{
				result = "at";
				index++;
				success = true;
			}

			result = "";
			success = true;
		}
		success = false;
	}

	void check_token(lexer& lexer, std::size_t& index, std::string& result)
	{
		std::string token_replacement("");
		bool success = false;

		verify_tobe(lexer, index, token_replacement, success);
		check_preposition(lexer, index, token_replacement, success);

		if (!success)
			token_replacement = lexer.get_token(index);
			
		if(!token_replacement.empty())
		result += token_replacement + " ";
	}

	std::string parser::parse()
	{
		lexer lexer(content);
		lexer.split_string();

		auto& list = lexer.get_token_list();

		std::string result("");
		
		for (std::size_t i = 0; i < list.size(); i++)
		{
			check_token(lexer, i, result);
		}

		/*for (auto& elem : list)
			result += elem + " ";*/

		return result;
	}
}
