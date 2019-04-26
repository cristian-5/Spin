
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
#define regexReplace std::regex_replace

#define subString substr

using namespace Collection;

/* MARK: - Macros */

#define INVERTED "[^A-Za-z0-9_]"

/*! @brief Namespace Stack */
namespace Stack {

	/*!
	 *   @brief Invalid Token Exception.
	 *   Raised when the token is not matched.
	 *   @author Cristian A.
	 */
	class InvalidTokenException: public std::exception {

	private:

		UInt32 position = 0;

	public:

		UInt32 getPosition() { return position; }

		InvalidTokenException(UInt32 character):
		std::exception(), position(character) { }

	};

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
			String result = "";
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
			String result = "";
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
			StrongList<String> result = StrongList<String>();
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
		 *   match with the given String.
		 *   @param match Match String.
		 *   @param input Input String.
		 *   @param replace Input Replace.
		 *   @returns The replaced String.
		 */
		String replaceMatches(String match, String input, String replace) {
			if (match.length() == 0) return input;
			if (input.length() == 0) return input;
			Int32 position = input.find(match);
			while (position != String::npos) {
				input.replace(position, match.length(), replace);
				position = input.find(match);
			}
			return input;
		}

		/* MARK: - Private Data */

		StrongList<Rule> grammar = StrongList<Rule>();

	public:

		String input = "";

		Lexer(String data = "") {

			input = data;

			grammar.link(Rule("([ \\t\\n]+)", empty));
			grammar.link(Rule("(\\/\\*+[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/)", comment));
			grammar.link(Rule("(\\/[\\/]+.*)", comment));

			grammar.link(Rule("(-?[0-9]+\\.[0-9]+)", realLiteral));
			grammar.link(Rule("(-?[0-9]+)", integerLiteral));
			grammar.link(Rule("(\"(?:[^\\\\\"]|\\\\[\"\\\\0abfnrtv]|\\\\x[0-9A-Fa-f][0-9A-Fa-f])*\")", stringLiteral));
			grammar.link(Rule("('(?:[^\\\\]|\\\\x[0-9A-Fa-f][0-9A-Fa-f]|\\\\['\\\\0abfnrtv])')", charLiteral));
			grammar.link(Rule("(true|false)" INVERTED, boolLiteral));

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
			grammar.link(Rule("(|)", pipe));
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

			grammar.link(Rule("(try)" INVERTED, tryKeyword));
			grammar.link(Rule("(catch)" INVERTED, catchKeyword));
			grammar.link(Rule("(throw)" INVERTED, throwKeyword));
			grammar.link(Rule("(throws)" INVERTED, throwsKeyword));
			grammar.link(Rule("(avoid)" INVERTED, avoidKeyword));

			grammar.link(Rule("(if)" INVERTED, ifKeyword));
			grammar.link(Rule("(switch)" INVERTED, ifKeyword));
			grammar.link(Rule("(case)" INVERTED, caseKeyword));
			grammar.link(Rule("(default)" INVERTED, defaultKeyword));
			grammar.link(Rule("(while)" INVERTED, whileKeyword));
			grammar.link(Rule("(do)" INVERTED, doKeyword));
			grammar.link(Rule("(loop)" INVERTED, loopKeyword));
			grammar.link(Rule("(for)" INVERTED, forKeyword));
			grammar.link(Rule("(repeat)" INVERTED, repeatKeyword));
			grammar.link(Rule("(until)" INVERTED, untilKeyword));
			grammar.link(Rule("(break)" INVERTED, breakKeyword));
			grammar.link(Rule("(continue)" INVERTED, continueKeyword));

			grammar.link(Rule("(func|function)" INVERTED, funcKeyword));
			grammar.link(Rule("(proc|procedure)" INVERTED, procKeyword));
			grammar.link(Rule("(static)" INVERTED, staticKeyword));
			grammar.link(Rule("(class)" INVERTED, classKeyword));
			grammar.link(Rule("(enum|enumerator)" INVERTED, enumKeyword));
			grammar.link(Rule("(struct|structure)" INVERTED, structKeyword));
			grammar.link(Rule("(except|exception)" INVERTED, exceptionKeyword));
			grammar.link(Rule("(private)" INVERTED, privateKeyword));
			grammar.link(Rule("(public)" INVERTED, publicKeyword));
			grammar.link(Rule("(inout)" INVERTED, inoutKeyword));
			grammar.link(Rule("(frozen)" INVERTED, frozenKeyword));
			grammar.link(Rule("(null)" INVERTED, nullKeyword));
			grammar.link(Rule("(return)" INVERTED, returnKeyword));

			grammar.link(Rule("([A-Za-z_][A-Za-z0-9_]+)" INVERTED, identifier));

		}

		StrongList<Token> tokenize() {
			String data = input + " ";
			data = replaceMatches("\n", input, " ");
			std::cout << data << std::endl;
			StrongList<Token> tokens = StrongList<Token>();
			UInt32 pos = 0;
			tokens.link(Token("newFile", newFile, pos));
			while (data.length() > 0) {
				bool tokenized = false;
				for (UInt32 i = 0; i < grammar.count(); i++) {
					String result = matchCloseStart(grammar[i].pattern, data);
					if (result.length() > 0) {
						tokenized = true;
						data = data.subString(result.length());
						pos += result.length();
						if (grammar[i].type == empty) break;
						Token token = Token(result, grammar[i].type, pos);
						tokens.link(token);
						break;
					}
				}
				if (!tokenized) throw InvalidTokenException(pos);
			}
			tokens.link(Token("endOfFile", endOfFile, pos + 1));
			return tokens;
		}

	};

}

#endif
