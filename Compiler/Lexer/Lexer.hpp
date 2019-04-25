
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
			grammar.link(Rule("(\\/\\*([^*]|[\\r\\n]|(\\*+([^*/]|[\\r\\n])))*\\*+\\/)|(\\/\\/.*$)", comment));

			grammar.link(Rule("(-?[0-9]+\\.[0-9]+)", realLiteral));
			grammar.link(Rule("(-?[0-9]+)", integerLiteral));
			// str not working, chr to try
			grammar.link(Rule("\"((?:[^\\\\\"]|\\\\['\"?\\\\0abfnrtv]|\\\\x[0-9A-Fa-f][0-9A-Fa-f])*)\"", stringLiteral));
			grammar.link(Rule("'([^\\\\']|\\\\['\"?\\\\0abfnrtv]|\\\\x[0-9A-Fa-f][0-9A-Fa-f])'", charLiteral));

			grammar.link(Rule("(true|false)[ \\t\\r\\n\\);]+", boolLiteral));

			grammar.link(Rule("(\\:)", colon));
			grammar.link(Rule("(\\;)", semicolon));
			grammar.link(Rule("(\\,)", comma));
			grammar.link(Rule("(\\.)", dot));
			grammar.link(Rule("(<)", minor));
			grammar.link(Rule("(>)", major));
			grammar.link(Rule("(=)", equal));
			grammar.link(Rule("(\\?)", questionMark));
			grammar.link(Rule("(\\!)", exclamationMark));

			grammar.link(Rule("(\\+)", plus));
			grammar.link(Rule("(-)", minus));
			grammar.link(Rule("(\\*)", star));
			grammar.link(Rule("(\\\\)", backslash));
			grammar.link(Rule("(\\/)", slash));
			grammar.link(Rule("(@)", at));
			grammar.link(Rule("(#)", hashtag));
			grammar.link(Rule("(&)", ampersand));
			grammar.link(Rule("(%)", modulus));
			grammar.link(Rule("($)", dollar));
			grammar.link(Rule("(^)", hat));

			grammar.link(Rule("(\\()", openRoundBracket));
			grammar.link(Rule("(\\))", closeRoundBracket));
			grammar.link(Rule("(\\[)", openSquareBracket));
			grammar.link(Rule("(\\])", closeSquareBracket));
			grammar.link(Rule("(\\{)", openCurlyBracket));
			grammar.link(Rule("(\\})", closeCurlyBracket));

			grammar.link(Rule("(try)[ \\t\\r\\n\\{]+", tryKeyword));
			grammar.link(Rule("(catch)[ \\t\\r\\n\\(]+", catchKeyword));
			grammar.link(Rule("(throw)[ \\t\\r\\n\\(]+", throwKeyword));
			grammar.link(Rule("(throws)[ \\t\\r\\n\\-]+", throwsKeyword));
			grammar.link(Rule("(avoid)[ \\t\\r\\n\\(]+", avoidKeyword));

			grammar.link(Rule("(if)[ \\t\\r\\n\\(]+", ifKeyword));
			grammar.link(Rule("(switch)[ \\t\\r\\n\\(]+", ifKeyword));
			grammar.link(Rule("(case)[ \\t\\r\\n\\(]+", caseKeyword));
			grammar.link(Rule("(default)[ \\t\\r\\n\\(]+", defaultKeyword));
			grammar.link(Rule("(while)[ \\t\\r\\n\\(]+", whileKeyword));
			grammar.link(Rule("(do)[ \\t\\r\\n\\{]+", doKeyword));
			grammar.link(Rule("(loop)[ \\t\\r\\n\\{]+", loopKeyword));
			grammar.link(Rule("(for)[ \\t\\r\\n\\(]+", forKeyword));
			grammar.link(Rule("(repeat)[ \\t\\r\\n\\{]+", repeatKeyword));
			grammar.link(Rule("(until)[ \\t\\r\\n\\(]+", untilKeyword));
			grammar.link(Rule("(break)[ \\t\\r\\n;]+", breakKeyword));
			grammar.link(Rule("(continue)[ \\t\\r\\n;]+", continueKeyword));

			grammar.link(Rule("(func)[ \\t\\r\\n]+", funcKeyword));
			grammar.link(Rule("(proc)[ \\t\\r\\n]+", procKeyword));
			grammar.link(Rule("(static)[ \\t\\r\\n]+", staticKeyword));
			grammar.link(Rule("(class)[ \\t\\r\\n]+", classKeyword));
			grammar.link(Rule("(enum)[ \\t\\r\\n]+", enumKeyword));
			grammar.link(Rule("(struct)[ \\t\\r\\n]+", structKeyword));
			grammar.link(Rule("(private)[ \\t\\r\\n]+", privateKeyword));
			grammar.link(Rule("(public)[ \\t\\r\\n]+", publicKeyword));
			grammar.link(Rule("(inout)[ \\t\\r\\n]+", inoutKeyword));
			grammar.link(Rule("(frozen)[ \\t\\r\\n]+", frozenKeyword));
			grammar.link(Rule("(null)[ \\t\\r\\n;]+", nullKeyword));
			grammar.link(Rule("(return)[ \\t\\r\\n;\\(]+", returnKeyword));

			grammar.link(Rule("([A-Za-z_][A-Za-z0-9_\\-]+)", identifier));

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
						tokenized = true;
						data = data.subString(result.length());
						std::cout << data << std::endl;
						pos += result.length();
						if (grammar[i].type == empty) break;
						Token token = Token(result, grammar[i].type, pos);
						tokens.link(token);
						break;
					}
				}
				if (!tokenized) {
					std::cout << "Error on character: " << pos << std::endl;
					std::cout << "Found unexpected token." << std::endl;
					return tokens;
				}
			}
			return tokens;
		}

	};

}

#endif
