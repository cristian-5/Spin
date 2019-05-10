
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
#include "../Token/TRule.hpp"

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
		Exception(), position(character) { }

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

		StrongList<TokenRule> grammar = StrongList<TokenRule>();

		public:

		String input = "";

		Lexer(String data = "") {

			input = data;

			grammar.link(* (new TokenRule("([ \\t\\n]+)", empty)));
			grammar.link(* (new TokenRule("(\\/\\*+[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/)", comment)));

			grammar.link(* (new TokenRule("(-?[0-9]+\\.[0-9]+)", realLiteral)));
			grammar.link(* (new TokenRule("(-?[0-9]+)", integerLiteral)));
			grammar.link(* (new TokenRule("(\"(?:[^\\\\\"]|\\\\[\"\\\\0abfnrtv]|\\\\x[0-9A-Fa-f][0-9A-Fa-f])*\")", stringLiteral)));
			grammar.link(* (new TokenRule("('(?:[^\\\\]|\\\\x[0-9A-Fa-f][0-9A-Fa-f]|\\\\['\\\\0abfnrtv])')", charLiteral)));
			grammar.link(* (new TokenRule("(true|false)" INVERTED, boolLiteral)));

			grammar.link(* (new TokenRule("(\\:)", colon)));
			grammar.link(* (new TokenRule("(\\;)", semicolon)));
			grammar.link(* (new TokenRule("(\\,)", comma)));
			grammar.link(* (new TokenRule("(\\.)", dot)));
			grammar.link(* (new TokenRule("(<)", minor)));
			grammar.link(* (new TokenRule("(>)", major)));
			grammar.link(* (new TokenRule("(=)", equal)));
			grammar.link(* (new TokenRule("(\\?)", questionMark)));
			grammar.link(* (new TokenRule("(\\!)", exclamationMark)));

			grammar.link(* (new TokenRule("(\\+)", plus)));
			grammar.link(* (new TokenRule("(-)", minus)));
			grammar.link(* (new TokenRule("(\\*)", star)));
			grammar.link(* (new TokenRule("(\\\\)", backslash)));
			grammar.link(* (new TokenRule("(\\/)", slash)));
			grammar.link(* (new TokenRule("(@)", at)));
			grammar.link(* (new TokenRule("(|)", pipe)));
			grammar.link(* (new TokenRule("(#)", hashtag)));
			grammar.link(* (new TokenRule("(&)", ampersand)));
			grammar.link(* (new TokenRule("(%)", modulus)));
			grammar.link(* (new TokenRule("($)", dollar)));
			grammar.link(* (new TokenRule("(^)", hat)));

			grammar.link(* (new TokenRule("(\\()", openRoundBracket)));
			grammar.link(* (new TokenRule("(\\))", closeRoundBracket)));
			grammar.link(* (new TokenRule("(\\[)", openSquareBracket)));
			grammar.link(* (new TokenRule("(\\])", closeSquareBracket)));
			grammar.link(* (new TokenRule("(\\{)", openCurlyBracket)));
			grammar.link(* (new TokenRule("(\\})", closeCurlyBracket)));

			grammar.link(* (new TokenRule("(try)" INVERTED, tryKeyword)));
			grammar.link(* (new TokenRule("(catch)" INVERTED, catchKeyword)));
			grammar.link(* (new TokenRule("(throw)" INVERTED, throwKeyword)));
			grammar.link(* (new TokenRule("(throws)" INVERTED, throwsKeyword)));
			grammar.link(* (new TokenRule("(avoid)" INVERTED, avoidKeyword)));

			grammar.link(* (new TokenRule("(if)" INVERTED, ifKeyword)));
			grammar.link(* (new TokenRule("(else)" INVERTED, elseKeyword)));
			grammar.link(* (new TokenRule("(switch)" INVERTED, ifKeyword)));
			grammar.link(* (new TokenRule("(case)" INVERTED, caseKeyword)));
			grammar.link(* (new TokenRule("(default)" INVERTED, defaultKeyword)));
			grammar.link(* (new TokenRule("(while)" INVERTED, whileKeyword)));
			grammar.link(* (new TokenRule("(do)" INVERTED, doKeyword)));
			grammar.link(* (new TokenRule("(loop)" INVERTED, loopKeyword)));
			grammar.link(* (new TokenRule("(for)" INVERTED, forKeyword)));
			grammar.link(* (new TokenRule("(repeat)" INVERTED, repeatKeyword)));
			grammar.link(* (new TokenRule("(until)" INVERTED, untilKeyword)));
			grammar.link(* (new TokenRule("(break)" INVERTED, breakKeyword)));
			grammar.link(* (new TokenRule("(continue)" INVERTED, continueKeyword)));

			grammar.link(* (new TokenRule("(func)" INVERTED, funcKeyword)));
			grammar.link(* (new TokenRule("(proc)" INVERTED, procKeyword)));
			grammar.link(* (new TokenRule("(static)" INVERTED, staticKeyword)));
			grammar.link(* (new TokenRule("(class)" INVERTED, classKeyword)));
			grammar.link(* (new TokenRule("(enumerator)" INVERTED, enumKeyword)));
			grammar.link(* (new TokenRule("(structure)" INVERTED, structKeyword)));
			grammar.link(* (new TokenRule("(exception)" INVERTED, exceptKeyword)));
			grammar.link(* (new TokenRule("(private)" INVERTED, privateKeyword)));
			grammar.link(* (new TokenRule("(public)" INVERTED, publicKeyword)));
			grammar.link(* (new TokenRule("(inout)" INVERTED, inoutKeyword)));
			grammar.link(* (new TokenRule("(const)" INVERTED, constKeyword)));
			grammar.link(* (new TokenRule("(null)" INVERTED, nullKeyword)));
			grammar.link(* (new TokenRule("(nope?)" INVERTED, nop)));
			grammar.link(* (new TokenRule("(return)" INVERTED, returnKeyword)));

			grammar.link(* (new TokenRule("([A-Za-z_][A-Za-z0-9_]*)" INVERTED, identifier)));

		}

		StrongList<Token> tokenize() {
			// Handle Last Token:
			String data = input + "\n";
			// Handle Single Line Comments:
			data = handleComments(data);
			// Handle EndLines:
			data = replaceMatches("\n", data, " ");
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
