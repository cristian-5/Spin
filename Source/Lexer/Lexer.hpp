
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

#include "../Aliases/Aliases.hpp"
#include "../Collection/StrongList.hpp"
#include "../Linker/FileHandler.hpp"
#include "../Token/Token.hpp"

#include "Regex.hpp"

using namespace Collection;
using namespace RegexTools;

#define INVERTED "[^A-Za-z0-9_]"

namespace Stack {

	/*!
	 *   @brief Invalid Token Exception.
	 *   Raised when the token is not matched.
	 */
	class InvalidTokenException: public Exception {
		private:
		FilePosition pos = { 0, 0 };
		String fileName = "";
		public:
		FilePosition getPosition() { return pos; }
		String getFileName() { return fileName; }
		InvalidTokenException(FilePosition position, String name):
		Exception(), pos(position), fileName(name) { }
	};

	class Lexer {

		private:

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

		void generateTokens() {

			const UInt32 tokenCount = 74;

			TokenRule rules[tokenCount] = {

				TokenRule("([ \\t\\n]+)", TokenType::empty),
				TokenRule("(\\/\\*+[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/)", TokenType::comment),

				TokenRule("([0-9]+(?:\\.[0-9]+)?i)", TokenType::imaginaryLiteral),
				TokenRule("([0-9]+\\.[0-9]+)", TokenType::realLiteral),
				TokenRule("((?:0[x][0-9A-Fa-f]+)|(?:0b[01]+)|(?:0o[0-7]+)|(?:0d[0-9]+)|(?:[0-9]+))", TokenType::intLiteral),
				TokenRule("(\"(?:[^\\\\\"]|\\\\[\"\\\\0abfnrtv]|\\\\x[0-9A-Fa-f][0-9A-Fa-f])*\")", TokenType::stringLiteral),
				TokenRule("('(?:[^\\\\]|\\\\x[0-9A-Fa-f][0-9A-Fa-f]|\\\\['\\\\0abfnrtv])')", TokenType::charLiteral),
				TokenRule("(#[A-Fa-f0-9]{6}|#[A-Fa-f0-9]{3})" INVERTED, TokenType::colourLiteral),
				TokenRule("(true|false)" INVERTED, TokenType::boolLiteral),

				TokenRule("(<[01]+\\|[01]+>)", TokenType::bra_ketLiteral),
				TokenRule("(<[01]+\\|)", TokenType::braLiteral),
				TokenRule("(\\|[01]+>)", TokenType::ketLiteral),

				TokenRule("(\\:)", TokenType::colon),
				TokenRule("(\\;)", TokenType::semicolon),
				TokenRule("(\\,)", TokenType::comma),
				TokenRule("(\\.)", TokenType::dot),
				TokenRule("(<)", TokenType::minor),
				TokenRule("(>)", TokenType::major),
				TokenRule("(=)", TokenType::equal),
				TokenRule("(\\?)", TokenType::questionMark),
				TokenRule("(\\!)", TokenType::exclamationMark),

				TokenRule("(\\+)", TokenType::plus),
				TokenRule("(-)", TokenType::minus),
				TokenRule("(\\*)", TokenType::star),
				TokenRule("(\\\\)", TokenType::backslash),
				TokenRule("(\\/)", TokenType::slash),
				TokenRule("(@)", TokenType::at),
				TokenRule("(|)", TokenType::pipe),
				TokenRule("(#)", TokenType::hashtag),
				TokenRule("(&)", TokenType::ampersand),
				TokenRule("(%)", TokenType::modulus),
				TokenRule("($)", TokenType::dollar),
				TokenRule("(^)", TokenType::hat),

				TokenRule("(\\()", TokenType::openRoundBracket),
				TokenRule("(\\))", TokenType::closeRoundBracket),
				TokenRule("(\\[)", TokenType::openSquareBracket),
				TokenRule("(\\])", TokenType::closeSquareBracket),
				TokenRule("(\\{)", TokenType::openCurlyBracket),
				TokenRule("(\\})", TokenType::closeCurlyBracket),

				TokenRule("(try)" INVERTED, TokenType::tryKeyword),
				TokenRule("(catch)" INVERTED, TokenType::catchKeyword),
				TokenRule("(throw)" INVERTED, TokenType::throwKeyword),
				TokenRule("(throws)" INVERTED, TokenType::throwsKeyword),
				TokenRule("(avoid)" INVERTED, TokenType::avoidKeyword),

				TokenRule("(if)" INVERTED, TokenType::ifKeyword),
				TokenRule("(else)" INVERTED, TokenType::elseKeyword),
				TokenRule("(switch)" INVERTED, TokenType::ifKeyword),
				TokenRule("(case)" INVERTED, TokenType::caseKeyword),
				TokenRule("(default)" INVERTED, TokenType::defaultKeyword),
				TokenRule("(while)" INVERTED, TokenType::whileKeyword),
				TokenRule("(do)" INVERTED, TokenType::doKeyword),
				TokenRule("(loop)" INVERTED, TokenType::loopKeyword),
				TokenRule("(for)" INVERTED, TokenType::forKeyword),
				TokenRule("(repeat)" INVERTED, TokenType::repeatKeyword),
				TokenRule("(until)" INVERTED, TokenType::untilKeyword),
				TokenRule("(break)" INVERTED, TokenType::breakKeyword),
				TokenRule("(continue)" INVERTED, TokenType::continueKeyword),

				TokenRule("(import)" INVERTED, TokenType::importKeyword),
				TokenRule("(func)" INVERTED, TokenType::funcKeyword),
				TokenRule("(proc)" INVERTED, TokenType::procKeyword),
				TokenRule("(static)" INVERTED, TokenType::staticKeyword),
				TokenRule("(class)" INVERTED, TokenType::classKeyword),
				TokenRule("(enumerator)" INVERTED, TokenType::enumKeyword),
				TokenRule("(structure)" INVERTED, TokenType::structKeyword),
				TokenRule("(exception)" INVERTED, TokenType::exceptKeyword),
				TokenRule("(private)" INVERTED, TokenType::privateKeyword),
				TokenRule("(public)" INVERTED, TokenType::publicKeyword),
				TokenRule("(ref)" INVERTED, TokenType::refKeyword),
				TokenRule("(cpy)" INVERTED, TokenType::cpyKeyword),
				TokenRule("(const)" INVERTED, TokenType::constKeyword),
				TokenRule("(null)" INVERTED, TokenType::nullLiteral),
				TokenRule("(nope?)" INVERTED, TokenType::nop),
				TokenRule("(return)" INVERTED, TokenType::returnKeyword),

				TokenRule("([A-Za-z_][A-Za-z0-9_]*)" INVERTED, TokenType::symbol),

			};

			for (UInt32 i = 0; i < tokenCount; i++) {
				TokenRule temp = rules[i];
				grammar.link(temp);
			}

		}

		StrongList<TokenRule> grammar = StrongList<TokenRule>();

		public:

		Lexer() { generateTokens(); }

		StrongList<Token> * tokenise(String * input, String fileName = "Unknown File") {
			// Handle Last Token:
			String data = (* input) + "\n";
			// Handle Single Line Comments:
			data = handleComments(data);
			// Handle EndLines:
			data = replaceMatches("\n", data, " ");
			StrongList<Token> * tokens = new StrongList<Token>();
			UInt32 pos = 0;
			Token temp = Token("beginFile", TokenType::beginFile, 0);
			tokens -> link(temp);
			while (data.length() > 0) {
				Boolean tokenised = false;
				for (UInt32 i = 0; i < grammar.count(); i++) {
					String result = matchCloseStart(grammar[i].pattern, data);
					if (result.length() > 0) {
						tokenised = true;
						data = data.subString(result.length());
						temp = Token(result, grammar[i].type, pos);
						pos += result.length();
						if (grammar[i].type == TokenType::empty) break;
						if (grammar[i].type == TokenType::comment) break;
						tokens -> link(temp);
						break;
					}
				}
				if (!tokenised) {
					FilePosition fp = getPosition(input, pos);
					throw InvalidTokenException(fp, fileName);
				}
			}
			temp = Token("endFile", TokenType::endFile, 0);
			tokens -> link(temp);
			return tokens;
		}

	};

}

#endif
