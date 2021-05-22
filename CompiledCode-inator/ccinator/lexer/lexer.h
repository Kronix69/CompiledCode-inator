#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace ccinator
{
	struct LexerToken
	{
		std::string content;

		LexerToken(std::string content) : content(content) {}
	};

	class lexer
	{
		std::string content;
		std::vector<std::string> tokens;
		std::size_t index = 0;

	public:
		void split_string();
		void move_index(std::int32_t index);
		std::vector<std::string>& get_token_list();
		std::string& get_token(std::int32_t index);

		lexer(std::string content) : content(content) {}
	};
}

