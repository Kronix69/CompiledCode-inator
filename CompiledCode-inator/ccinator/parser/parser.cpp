#include "parser.h"
#include <numeric>
#include <algorithm>
#include <http/http.h>
#include <iostream>
#include <random>
#include <time.h>
namespace ccinator
{
	std::string to_lower(const std::string& token)
	{
		std::string result = token;

		std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });

		return result;
	}

	void verb_add_ing(std::string& verb)
	{
		if (verb.length() >= 3 && to_lower(verb).substr(verb.length() - 2, 2) == "ed" && to_lower(verb).substr(verb.length() - 3, 3) != "eed")
			verb.replace(verb.end() - 2, verb.end(), "ing");
		else if (verb.length() >= 1 && verb[verb.length() - 1] == 'e')
			verb.replace(verb.end() - 1, verb.end(), "ing");
	//	else if (to_lower(verb) != "fucking" && verb.length() >= 3 && to_lower(verb).substr(verb.length() - 3, 3) == "ing" )
		//	verb.replace(verb.end() - 2, verb.end(), "");
		else if(verb.length() >= 3 && to_lower(verb).substr(verb.length() - 3, 3) != "ing")
			verb += "ing";
	}

	bool compiledinate_verb(const std::string& verb, std::string& verbresult)
	{
		auto result = http::get("https://conjugator.reverso.net/conjugation-english-verb-" + verb + ".html");

		if (result.find("Approximate result") != std::string::npos)
			return false;

		auto pos = result.find("tooltip=\"Existing infinitive\">");
		if (pos == std::string::npos)
			return false;
		pos += 30;
		auto endpos = std::find(result.begin() + pos, result.end(), '<');

		auto pastpos = result.find("Preterite</p><ul class=\"wrap - verbs - listing top2\"><li><i class=\"graytxt\">I </i><i class=\"hglhOver\">");
		pastpos += 101;
		auto pastendpos = std::find(result.begin() + pastpos, result.end(), ' ');

		auto pastverbresult = result.substr(pastpos, (pastendpos - result.begin()) - pastpos);
		//if (verb != result.substr(pos, (endpos - result.begin()) - pos))
		//	verbresult = result.substr(pos, (endpos - result.begin()) - pos);
		if (verb.length() >= 3 && to_lower(verb).substr(verb.length() - 3, 3) != "ing")
		{
			verbresult = result.substr(pos, (endpos - result.begin()) - pos);
			if (((rand() % 2 == 1) && (verb != verbresult)))
			{
				verb_add_ing(verbresult);
			}
		}
		else
			verbresult = verb;

		if (verb == pastverbresult && verbresult != pastverbresult)
			verbresult = "was " + verbresult;

		return true;
	}

	void verify_tobe(lexer& lexer, std::size_t& index, std::string& result, bool& success)
	{
		if (success)
			return;

		auto& token = lexer.get_token(index);

		if (to_lower(token) == "im" || to_lower(token) == "i'm")
		{
			result = "im is";
			success = true;
			return;
		}
		else if (to_lower(token) == "your")
		{
			result = "you";
			success = true;
			return;
		}
		else if (to_lower(token) == "can't" || to_lower(token) == "cant")
		{
			result = "not can";
			success = true;
			return;
		}
		else if (to_lower(token) == "you're")
		{
			result = "you is";
			success = true;
			return;
		}
		else if (to_lower(token) == "we're")
		{
			result = "we is";
			success = true;
			return;
		}
		else if (to_lower(token) == "they're")
		{
			result = "the is";
			success = true;
			return;
		}
		else if (to_lower(token) == "i've")
		{
			result = "im is have";
			success = true;
			return;
		}
		else if (to_lower(token) == "ain't" || to_lower(token) == "aint")
		{
			result = "not";
			success = true;
			return;
		}
		else if (to_lower(token) == "you've")
		{
			result = "you is have";
			success = true;
			return;
		}
		else if (to_lower(token) == "aren't " || to_lower(token) == "arent")
		{
			result = "not is";
			success = true;
			return;
		}
		else if (to_lower(token) == "i'll")
		{
			result = "im is will";
			success = true;
			return;
		}
		else if (to_lower(token) == "i'd")
		{
			result = "im is would";
			success = true;
			return;
		}

		if (token.length() >= 2 && to_lower(token).substr(token.length() - 2, 2) == "'d")
			token.replace(token.end() - 2, token.end(), " is would");

		if (token.length() >= 3 && to_lower(token).substr(token.length() - 3, 3) == "'ll")
			token.replace(token.end() - 3, token.end(), " is will");

		if (to_lower(token) == "who" && lexer.get_token(index + 1) != "is")
		{
			result = "who is";
			success = true;
			return;
		}
		else if (to_lower(token) == "who" && lexer.get_token(index + 1) == "is")
		{
			result = "who";
			index++;
			success = true;
			return;
		}

		if (to_lower(token) == "i")
		{
			auto& self_cont = lexer.get_token(index + 1);

			/*Present*/
			if (to_lower(self_cont) == "am")
			{
				result = "im is";
				index ++;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "can")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = "im is can " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "couldn't")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = "im is could no " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "can't" || to_lower(self_cont) == "cant")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = "im is no can " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "don't" || to_lower(self_cont) == "dont" || to_lower(self_cont) == "won't" || to_lower(self_cont) == "wont")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = "im is no " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (self_cont.find("ed") != std::string::npos)
			{
				verb_add_ing(self_cont);
				result = "im is " + self_cont;
				if (lexer.get_token(index + 2) != "")
					result = result + " at";
				if (lexer.get_token(index + 2) == "in")
					index++;
				index++;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "did")
			{
				result = "im is did";
				index++;
				success = true;
				return;
			}

			 /*Past*/
			else if (to_lower(self_cont) == "was")
			{
				result = "was";
				index++;
				success = true;
				return;
			}

				result = "am";
				success = true;
				return;
		}
		if (to_lower(token) == "you" || to_lower(token) == "they" || to_lower(token) == "we")
		{
			auto& self_cont = lexer.get_token(index + 1);

			/*Present*/
			if (to_lower(self_cont) == "are")
			{
				result = token + " is";
				index++;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "can")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = token + " is can " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "couldn't")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = token + " is could no " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "can't" || to_lower(self_cont) == "cant")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = token + " is no can " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "don't" || to_lower(self_cont) == "dont" || to_lower(self_cont) == "won't" || to_lower(self_cont) == "wont")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = token + " is no " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "want")
			{
				result = token + " is";
				success = true;
				return;
			}
			else if (self_cont.find("ed") != std::string::npos)
			{
				verb_add_ing(self_cont);
				result = token + " is " + self_cont;
				if (lexer.get_token(index + 2) != "")
					result = result + " at";
				if (lexer.get_token(index + 2) == "in")
					index++;
				index++;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "did")
			{
				result = token + " is did";
				index++;
				success = true;
				return;
			}

			/*Past*/
			else if (to_lower(self_cont) == "were")
			{
				result = token + " is was";
				index++;
				success = true;
				return;
			}

			else
			{
			/*	std::string res;
				auto bl = compiledinate_verb(to_lower(self_cont), res);

				if (to_lower(token) == "you" && (bl == false || self_cont == ""))
				{
					result = token;
					success = true;
					return;
				}

				result = token + " is";*/
				result = token;
				success = true;
				return;
			}
		}

		if (to_lower(token) == "he" || to_lower(token) == "she" || to_lower(token) == "it" || to_lower(token) == "it")
		{
			auto& self_cont = lexer.get_token(index + 1);

			/*Present*/
			if (to_lower(self_cont) == "can")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = token + " is can " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "couldn't")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = token + " is could no " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "can't" || to_lower(self_cont) == "cant")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = token + " no can " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "doesn't" || to_lower(self_cont) == "doesnt" || to_lower(self_cont) == "won't" || to_lower(self_cont) == "wont")
			{
				auto& verb = lexer.get_token(index + 2);
				verb_add_ing(verb);
				result = token + " no " + verb;
				index += 2;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "wants")
			{
				result = token + " is want";
				success = true;
				return;
			}
			else if (self_cont.find("ed") != std::string::npos)
			{
				verb_add_ing(self_cont);

				result = token + " " + self_cont;
				if (lexer.get_token(index + 2) != "")
					result = result + " at";
				index++;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "did")
			{
				result = token + " is did";
				index++;
				success = true;
				return;
			}

			/*Past*/
			else if (to_lower(self_cont) == "was")
			{
				result = token + " is was";
				index++;
				success = true;
				return;
			}

			/*else
			{
				result = token;
				success = true;
				return;
			}*/
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
				return;
			}

			std::string verbresult = "";

			auto verbsuc = compiledinate_verb(to_lower(self_cont), verbresult);

			if (verbsuc)
			{
				std::string verbresult2 = "";
				auto previousverb = lexer.get_token(index - 1);
				auto prevsuc = compiledinate_verb(to_lower(previousverb), verbresult2);

				//verb_add_ing(verbresult);
				result = "";
				//index++;
				success = true;
			    
				if (!prevsuc)
					result = "to";

				return;
			}

			result = "at";
			success = true;
			return;
		}
		if (to_lower(token) == "in")
		{
			auto& self_cont = lexer.get_token(index + 1);
			auto& previousverb = lexer.get_token(index - 1);

			if (to_lower(self_cont) == "the")
			{
				result = "in";
				index++;
				success = true;
				return;
			}
			else if (to_lower(self_cont) == "a")
			{
				result = "in";
				success = true;
				return;
			}

			std::string verbresult = "";

			auto verbsuc = compiledinate_verb(to_lower(self_cont), verbresult);

			if (verbsuc)
			{
				std::string verbresult2 = "";
				auto prevsuc = compiledinate_verb(to_lower(previousverb), verbresult2);

				//verb_add_ing(verbresult);
				result = verbresult;
				index++;
				success = true;

				if (!prevsuc)
					result = "in " + result;
				else
					result = "at " + result;

				return;
			}

			result = "at";
			success = true;
			return;
		}
		else if (to_lower(token) == "the")
		{
			result = "";
			success = true;
			return;
		}
		else if (to_lower(token) == "custom" || to_lower(token) == "customality")
		{
			result = "@Customality#5880 @Customality#5880 @Customality#5880 @Customality#5880";
			success = true;
			return;
		}
		else if (to_lower(token) == "not")
		{
			result = "no";
			success = true;
			return;
		}
		else if (to_lower(token) == "this")
		{
			result = "";
			success = true;
			return;
		}
		else if (to_lower(token) == "it's")
		{
			result = "is";
			success = true;
			return;
		}
		else if (to_lower(token) == "don't")
		{
			result = "not";
			success = true;
			return;
		}
		else if (to_lower(token) == "a")
		{
			result = "";
			success = true;
			return;
		}
		else if (to_lower(token) == "so")
		{
			result = "much";
			success = true;
			return;
		}
		else if (to_lower(token) == "didn't")
		{
			result = "not did";
			success = true;
			return;
		}
		else if (to_lower(token) == "have" && to_lower(lexer.get_token(index + 1)) == "to")
		{
			result = "need";
			success = true;
			return;
		}
		else if (to_lower(token) == "couldn't")
		{
			result = "not could";
			success = true;
			return;
		}
		else if (to_lower(token) == "being")
		{
		result = "be";
		success = true;
		return;
		}
		else if (to_lower(token) == "be")
		{
		result = "being";
		success = true;
		return;
		}
		else if (to_lower(token) == "easily")
		{
		result = "easy";
		success = true;
		return;
        }
		else if (to_lower(token) == "did")
		{
			result = "is";
			success = true;
			return;
		}
		else if (to_lower(token) == "my")
		{
			result = "mine";
			success = true;
			return;
		}
		else if (to_lower(token) == "is")
		{
		result = "";
		success = true;
		return;
		}
		else if (to_lower(token) == "are")
		{
		result = "is";
		success = true;
		return;
		}
		else if (to_lower(token) == "were")
		{
		result = "is was";
		success = true;
		return;
		}
		else if (to_lower(token) == "was")
		{
		result = "is was";
		success = true;
		return;
		}
		success = false;
	}

	void check_verb(lexer& lexer, std::size_t& index, std::string& result, bool& success)
	{
		if (success)
			return;

		auto token = to_lower(lexer.get_token(index));
		auto prevprep = to_lower(lexer.get_token(index - 1));

		if (prevprep == "the" || prevprep == "a")
			return;

		if (token == "go")
		{
			auto& self_cont = lexer.get_token(index + 1);

			result = "going";
			success = true;

			return;
		}
		else if (token == "gonna")
		{
			auto& self_cont = lexer.get_token(index + 1);

			result = "going";
			success = true;

			return;
		}
		else
		{
			std::string verbresult = "";
			auto verbsuc = compiledinate_verb(to_lower(token), verbresult);

			if (verbsuc)
			{
				auto& self_cont = lexer.get_token(index + 1);
				auto previous = lexer.get_token(index - 2) + " " + lexer.get_token(index - 1);

				if(to_lower(self_cont) == "me" && (lexer.get_token(index - 3) != "is" && previous != "going to") && previous != "is gonna")
				{
					verb_add_ing(verbresult);
					result = verbresult + " at " + self_cont;
					int idx = -1;
					auto tkn = to_lower(lexer.get_token(index + idx));
					if (tkn == "to" || tkn == "the")
						idx = -2;
					auto verbsuc = compiledinate_verb(to_lower(lexer.get_token(index + idx)), verbresult);
					if(!verbsuc)
					result = "is " + result;
					index++;
					success = true;
					return;
				}

				ret:
				result = verbresult;
				success = true;
				return;
			}
		}

		success = false;
	}

	void check_token(lexer& lexer, std::size_t& index, std::string& result)
	{
		std::string token_replacement("");
		bool success = false;

		auto& token = lexer.get_token(index);

		if (token.length() >= 2 && to_lower(token).substr(token.length() - 2, 2) == "'s")
			token.replace(token.end() - 2, token.end(), "");

		verify_tobe(lexer, index, token_replacement, success);
		check_preposition(lexer, index, token_replacement, success);
		check_verb(lexer, index, token_replacement, success);

		if (!success)
			token_replacement = lexer.get_token(index);
			
		if(!token_replacement.empty())
		result += token_replacement + " ";
	}

	std::string parser::parse()
	{
		srand(time(NULL));

		lexer lexer(content);
		lexer.split_string();

		auto& list = lexer.get_token_list();

		std::size_t i = 0;

		std::string result("");

		if (to_lower(lexer.get_token(i)) == "i" && to_lower(lexer.get_token(i + 1)) == "am")
			result = "am ", i = 2;
		else if(to_lower(lexer.get_token(i)) == "im" || to_lower(lexer.get_token(i)) == "i'm")
			result = "am ", i = 1;
		
		for (; i < list.size(); i++)
		{
			check_token(lexer, i, result);
		}

		/*for (auto& elem : list)
			result += elem + " ";*/

		return result;
	}
}
