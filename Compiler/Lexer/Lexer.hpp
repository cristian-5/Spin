
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
using Exception = std::exception;

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
	class InvalidTokenException: public Exception {

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
					for (Int32 i = 1; i < match.size(); i++) {
						result.link(* (new String(match.str(i))));
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

		/* MARK: - Handle Comments */

		String handleComments(String input) {
			if (input.length() == 0) return input;
			try {
				Regex regex("\\/[\\/]+.*");
				SMatch match;
				regexSearch(input, match, regex);
				while (match.size() >= 1) {
					UInt32 len = match.str(0).length();
					UInt32 pos = match.position(0);
					for (UInt32 i = pos; i < pos + len; i++) {
						input[i] = ' ';
					}
					regexSearch(input, match, regex);
				}
			} catch (RegexError & e) { }
			return input;
		}

		/* MARK: - Private Data */

		StrongList<Rule> grammar = StrongList<Rule>();

	public:

		String input = "";

		Lexer(String data = "") {

			input = data;

			grammar.link(* (new Rule("([ \\t\\n]+)", empty)));
			grammar.link(* (new Rule("(\\/\\*+[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/)", comment)));

			grammar.link(* (new Rule("(-?[0-9]+\\.[0-9]+)", realLiteral)));
			grammar.link(* (new Rule("(-?[0-9]+)", integerLiteral)));
			grammar.link(* (new Rule("(\"(?:[^\\\\\"]|\\\\[\"\\\\0abfnrtv]|\\\\x[0-9A-Fa-f][0-9A-Fa-f])*\")", stringLiteral)));
			grammar.link(* (new Rule("('(?:[^\\\\]|\\\\x[0-9A-Fa-f][0-9A-Fa-f]|\\\\['\\\\0abfnrtv])')", charLiteral)));
			grammar.link(* (new Rule("(true|false)" INVERTED, boolLiteral)));

			grammar.link(* (new Rule("(\\:)", colon)));
			grammar.link(* (new Rule("(\\;)", semicolon)));
			grammar.link(* (new Rule("(\\,)", comma)));
			grammar.link(* (new Rule("(\\.)", dot)));
			grammar.link(* (new Rule("(<)", minor)));
			grammar.link(* (new Rule("(>)", major)));
			grammar.link(* (new Rule("(=)", equal)));
			grammar.link(* (new Rule("(\\?)", questionMark)));
			grammar.link(* (new Rule("(\\!)", exclamationMark)));

			grammar.link(* (new Rule("(\\+)", plus)));
			grammar.link(* (new Rule("(-)", minus)));
			grammar.link(* (new Rule("(\\*)", star)));
			grammar.link(* (new Rule("(\\\\)", backslash)));
			grammar.link(* (new Rule("(\\/)", slash)));
			grammar.link(* (new Rule("(@)", at)));
			grammar.link(* (new Rule("(|)", pipe)));
			grammar.link(* (new Rule("(#)", hashtag)));
			grammar.link(* (new Rule("(&)", ampersand)));
			grammar.link(* (new Rule("(%)", modulus)));
			grammar.link(* (new Rule("($)", dollar)));
			grammar.link(* (new Rule("(^)", hat)));

			grammar.link(* (new Rule("(\\()", openRoundBracket)));
			grammar.link(* (new Rule("(\\))", closeRoundBracket)));
			grammar.link(* (new Rule("(\\[)", openSquareBracket)));
			grammar.link(* (new Rule("(\\])", closeSquareBracket)));
			grammar.link(* (new Rule("(\\{)", openCurlyBracket)));
			grammar.link(* (new Rule("(\\})", closeCurlyBracket)));

			grammar.link(* (new Rule("(try)" INVERTED, tryKeyword)));
			grammar.link(* (new Rule("(catch)" INVERTED, catchKeyword)));
			grammar.link(* (new Rule("(throw)" INVERTED, throwKeyword)));
			grammar.link(* (new Rule("(throws)" INVERTED, throwsKeyword)));
			grammar.link(* (new Rule("(avoid)" INVERTED, avoidKeyword)));

			grammar.link(* (new Rule("(if)" INVERTED, ifKeyword)));
			grammar.link(* (new Rule("(switch)" INVERTED, ifKeyword)));
			grammar.link(* (new Rule("(case)" INVERTED, caseKeyword)));
			grammar.link(* (new Rule("(default)" INVERTED, defaultKeyword)));
			grammar.link(* (new Rule("(while)" INVERTED, whileKeyword)));
			grammar.link(* (new Rule("(do)" INVERTED, doKeyword)));
			grammar.link(* (new Rule("(loop)" INVERTED, loopKeyword)));
			grammar.link(* (new Rule("(for)" INVERTED, forKeyword)));
			grammar.link(* (new Rule("(repeat)" INVERTED, repeatKeyword)));
			grammar.link(* (new Rule("(until)" INVERTED, untilKeyword)));
			grammar.link(* (new Rule("(break)" INVERTED, breakKeyword)));
			grammar.link(* (new Rule("(continue)" INVERTED, continueKeyword)));

			grammar.link(* (new Rule("(func)" INVERTED, funcKeyword)));
			grammar.link(* (new Rule("(proc)" INVERTED, procKeyword)));
			grammar.link(* (new Rule("(static)" INVERTED, staticKeyword)));
			grammar.link(* (new Rule("(class)" INVERTED, classKeyword)));
			grammar.link(* (new Rule("(enumerator)" INVERTED, enumKeyword)));
			grammar.link(* (new Rule("(structure)" INVERTED, structKeyword)));
			grammar.link(* (new Rule("(exception)" INVERTED, exceptKeyword)));
			grammar.link(* (new Rule("(private)" INVERTED, privateKeyword)));
			grammar.link(* (new Rule("(public)" INVERTED, publicKeyword)));
			grammar.link(* (new Rule("(inout)" INVERTED, inoutKeyword)));
			grammar.link(* (new Rule("(const)" INVERTED, constKeyword)));
			grammar.link(* (new Rule("(null)" INVERTED, nullKeyword)));
			grammar.link(* (new Rule("(nope?)" INVERTED, nop)));
			grammar.link(* (new Rule("(return)" INVERTED, returnKeyword)));

			grammar.link(* (new Rule("([A-Za-z_][A-Za-z0-9_]*)" INVERTED, identifier)));

		}

		StrongList<Token> tokenize() {
			// Handle Last Token:
			String data = input + "\n";
			// Handle Single Line Comments:
			data = handleComments(data);
			// Handle EndLines:
			data = replaceMatches("\n", data, " ");
			std::cout << data << std::endl;
			StrongList<Token> tokens = StrongList<Token>();
			UInt32 pos = 0;
			tokens.link(* (new Token("newFile", newFile, pos)));
			while (data.length() > 0) {
				bool tokenized = false;
				for (UInt32 i = 0; i < grammar.count(); i++) {
					String result = matchCloseStart(grammar[i].pattern, data);
					if (result.length() > 0) {
						tokenized = true;
						data = data.subString(result.length());
						pos += result.length();
						if (grammar[i].type == empty) break;
						if (grammar[i].type == comment) break;
						Token * token = new Token(result, grammar[i].type, pos);
						tokens.link(* token);
						break;
					}
				}
				if (!tokenized) throw InvalidTokenException(pos);
			}
			tokens.link(* (new Token("endOfFile", endOfFile, pos + 1)));
			return tokens;
		}

	};

}

#endif
