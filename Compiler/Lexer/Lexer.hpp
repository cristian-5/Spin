
/*!
 *
 *    + --------------------------------------- +
 *    |  Lexer.hpp                              |
 *    |                                         |
 *    |                  Lexer                  |
 *    |                                         |
 *    |  Created by Cristian A.                 |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
 */

#ifndef STACKLEXER
#define STACKLEXER

#include <iostream>
#include <string>
#include <regex>

#include "../Collection/Collection.hpp"

#include "../Token/Token.hpp"
#include "../Token/Rule.hpp"

using String = std::string;
using SMatch = std::smatch;
using Regex = std::regex;
using RegexError = std::regex_error;
using UInt32 = std::uint32_t;

#define regexSearch std::regex_search
#define regexReplace = std::regex_replace

#define subString substr

using namespace Collection;

/*! @brief Namespace Stack */
namespace Stack {

	/*! @brief Lexer Class. */
	class Lexer {

	private:

		/* MARK: - Regexes */

		/*!
		 *   @brief Matches a regex.
		 *   @param rgx Regex String.
		 *   @param input Input String.
		 *   @returns The matched String.
		 */
		String match(String rgx, String input) {
			String result;
			try {
				Regex regex(rgx);
				SMatch match;
				regexSearch(input, match, regex);
				if (match.size() > 0) {
					result = match.str(0);
				} else return "";
			} catch (RegexError & e) {
				return "";
			}
			return result;
		}

		/*!
		 *   @brief Matches a regex and returns
		 *   the first capture group.
		 *   @param rgx Regex String.
		 *   @param input Input String.
		 *   @returns The first matched group.
		 */
		String matchClose(String rgx, String input) {
			String result;
			try {
				Regex regex(rgx);
				SMatch match;
				regexSearch(input, match, regex);
				if (match.size() > 1) {
					result = match.str(1);
				} else return "";
			} catch (RegexError & e) {
				return "";
			}
			return result;
		}

		/*!
		 *   @brief Matches a regex and returns
		 *   all captures group in a list.
		 *   @param rgx Regex String.
		 *   @param input Input String.
		 *   @returns The matched groups.
		 */
		StrongList<String> matchGroupClose(String rgx, String input) {
			StrongList<String> result;
			try {
				Regex regex(rgx);
				SMatch match;
				regexSearch(input, match, regex);
				if (match.size() > 1) {
					for (int i = 1; i < match.size(); i++) {
						result.link(match.str(i));
					}
				} else return StrongList<String>();
			} catch (RegexError & e) {
				return StrongList<String>();
			}
			return result;
		}

		/*!
		 *   @brief Matches a regex at the
		 *   beginning of a String.
		 *   @param rgx Regex String.
		 *   @param input Input String.
		 *   @returns The matched String.
		 */
		String matchStart(String rgx, String input) {
			return match("^" + rgx, input);
		}

		/*!
		 *   @brief Matches a regex at the
		 *   beginning of a String and returns
		 *   the first capture group.
		 *   @param rgx Regex String.
		 *   @param input Input String.
		 *   @returns The first matched group.
		 */
		String matchCloseStart(String rgx, String input) {
			return matchClose("^" + rgx, input);
		}

		/*!
		 *   @brief Replaces every occurrence of the
		 *   matched regex with the given String.
		 *   @param rgx Regex String.
		 *   @param input Input String.
		 *   @param replace Input Replace.
		 *   @returns The replaced String.
		 */
		/*String replaceMatches(String rgx, String input, String replace) {
			Regex regex(rgx);
			String result = input;
			while (regexSearch(result, regex)) {
				regexReplace(result, regex, replace);
			}
			return result;
		}*/

		/* MARK: - Private Data */

		StrongList<Rule> grammar = StrongList<Rule>();

	public:

		String data = "";

		Lexer(String data = "") {

			this -> data = data;

			grammar.link(Rule("([ \\t\\n\\r]+)", empty));
			grammar.link(Rule("([A-Za-z][A-Za-z0-9_\\-]+)", identifier));

			grammar.link(Rule("(-?[0-9]+\\.[0-9]+)", realLiteral));
			grammar.link(Rule("(-?[0-9]+)", integerLiteral));
			grammar.link(Rule("\"(\\$\\{.*\\}|\\.|[^\"\\\\])*\"", stringLiteral));
			grammar.link(Rule("'(\\$\\{.\\}|\\.|[^'\\\\])'", charLiteral));
			grammar.link(Rule("(true|false)", boolLiteral));


		}

		StrongList<Token> tokenize() {
			StrongList<Token> tokens = StrongList<Token>();
			String data = this -> data;
			UInt32 pos = 0;
			while (data.length() > 0) {
				bool tokenized = false;
				for (UInt32 i = 0; i < grammar.count(); i++) {
					String result = matchCloseStart(grammar[i].pattern, data);
					if (result.length() != 0) {
						Token token = Token(result, grammar[i].type, pos);
						tokens.link(token);
						pos += result.length();
						data = data.subString(result.length());
						std::cout << data << std::endl;
						tokenized = true;
						break;
					}
				}
				if (!tokenized) {
					std::cout << "Error on character: " << pos << std::endl;
					std::cout << "Found unexpected token." << std::endl;
					return StrongList<Token>();
				}
			}
			return tokens;
		}

	};

}

#endif
