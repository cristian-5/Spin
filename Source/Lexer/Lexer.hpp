
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
#include "../Linker/Linker.hpp"
#include "../Token/Token.hpp"

#include "Regex.hpp"

namespace Stack {

	class Lexer {

		private:

		String handleComments(String input) const {
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

		ArrayList<TokenRule> grammar = {

			{ "([ \\t\\n]+)", TokenType::empty },

			{ "(\\/\\*+[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/)", TokenType::comment },

			{ "([0-9]+(?:\\.[0-9]+(?:[eE][-]?[0-9]+)?)?i)", TokenType::imaginaryLiteral },
			{ "([0-9]+\\.[0-9]+(?:[eE][-]?[0-9]+)?)", TokenType::realLiteral },
			{ "((?:0[x][0-9A-Fa-f]+)|(?:0b[01]+)|(?:0o[0-7]+)|(?:0d[0-9]+)|(?:[0-9]+))", TokenType::intLiteral },
			{ "(\"(?:[^\\\\\"]|\\\\[\"\\\\0abfnrtv]|\\\\0x[0-9A-Fa-f]{2})*\")", TokenType::stringLiteral },
			{ "('(?:[^\\\\]|\\\\0x[0-9A-Fa-f]{2}|\\\\['\\\\0abfnrtv])')", TokenType::charLiteral },
			{ "(#[A-Fa-f0-9]{6}(?:[A-Fa-f0-9][A-Fa-f0-9])?|#[A-Fa-f0-9]{3,4})\\b", TokenType::colourLiteral },
			{ "(false|true)\\b", TokenType::boolLiteral },

			{ "(<[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*\\|[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*>)", TokenType::braketSymbol },
			{ "(<[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*\\|)", TokenType::braSymbol },
			{ "(\\|[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*>)", TokenType::ketSymbol },

			{ "(\\:)", TokenType::colon },
			{ "(\\;)", TokenType::semicolon },
			{ "(\\,)", TokenType::comma },
			{ "(\\.)", TokenType::dot },
			{ "(<>)", TokenType::diamond },
			{ "(<=)", TokenType::minorEqual },
			{ "(<)", TokenType::minor },
			{ "(>=)", TokenType::majorEqual },
			{ "(>)", TokenType::major },
			{ "(==)", TokenType::equality },
			{ "(=)", TokenType::equal },
			{ "(\\?)", TokenType::questionMark },
			{ "(\\!=)", TokenType::inequality },
			{ "(\\!)", TokenType::exclamationMark },

			{ "(\\+=)", TokenType::plusEqual },
			{ "(\\+)", TokenType::plus },
			{ "(\\-=)", TokenType::minusEqual },
			{ "(\\-)", TokenType::minus },
			{ "(\\~=)", TokenType::tildeEqual },
			{ "(\\~)", TokenType::tilde },
			{ "(\\*=)", TokenType::starEqual },
			{ "(\\*)", TokenType::star },
			{ "(\\\\)", TokenType::backslash },
			{ "(\\/=)", TokenType::slashEqual },
			{ "(\\/)", TokenType::slash },
			{ "(\\@)", TokenType::at },
			{ "(\\|=)", TokenType::pipeEqual },
			{ "(\\|\\|)", TokenType::OR },
			{ "(\\|)", TokenType::pipe },
			{ "(\\#)", TokenType::hashtag },
			{ "(\\&=)", TokenType::ampersandEqual },
			{ "(\\&\\&)", TokenType::AND },
			{ "(\\&)", TokenType::ampersand },
			{ "(\\%=)", TokenType::modulusEqual },
			{ "(\\%)", TokenType::modulus },
			{ "(\\$)", TokenType::dollar },
			{ "(\\^=)", TokenType::hatEqual },
			{ "(\\^)", TokenType::hat },

			{ "(\\()", TokenType::openParenthesis },
			{ "(\\))", TokenType::closeParenthesis },
			{ "(\\[)", TokenType::openBracket },
			{ "(\\])", TokenType::closeBracket },
			{ "(\\{)", TokenType::openBrace },
			{ "(\\})", TokenType::closeBrace },

			{ "(try)\\b", TokenType::tryKeyword },
			{ "(catch)\\b", TokenType::catchKeyword },
			{ "(throw)\\b", TokenType::throwKeyword },
			{ "(throws)\\b", TokenType::throwsKeyword },

			{ "(print)\\b", TokenType::printKeyword },

			{ "(if)\\b", TokenType::ifKeyword },
			{ "(else)\\b", TokenType::elseKeyword },
			{ "(switch)\\b", TokenType::switchKeyword },
			{ "(case)\\b", TokenType::caseKeyword },
			{ "(default)\\b", TokenType::defaultKeyword },
			{ "(while)\\b", TokenType::whileKeyword },
			{ "(do)\\b", TokenType::doKeyword },
			{ "(loop)\\b", TokenType::loopKeyword },
			{ "(for)\\b", TokenType::forKeyword },
			{ "(repeat)\\b", TokenType::repeatKeyword },
			{ "(until)\\b", TokenType::untilKeyword },
			{ "(break)\\b", TokenType::breakKeyword },
			{ "(continue)\\b", TokenType::continueKeyword },

			{ "(import)\\b", TokenType::importKeyword },
			{ "(func)\\b", TokenType::funcKeyword },
			{ "(proc)\\b", TokenType::procKeyword },
			{ "(static)\\b", TokenType::staticKeyword },
			{ "(class)\\b", TokenType::classKeyword },
			{ "(enumerator)\\b", TokenType::enumKeyword },
			{ "(structure)\\b", TokenType::structKeyword },
			{ "(exception)\\b", TokenType::exceptKeyword },
			{ "(private)\\b", TokenType::privateKeyword },
			{ "(public)\\b", TokenType::publicKeyword },
			{ "(ref)\\b", TokenType::refKeyword },
			{ "(cpy)\\b", TokenType::cpyKeyword },
			{ "(const)\\b", TokenType::constKeyword },
			{ "(empty)\\b", TokenType::emptyLiteral },
			{ "(rest)\\b", TokenType::restKeyword },
			{ "(return)\\b", TokenType::returnKeyword },

			{ "(Bool|Byte|Character|Colour|Complex|Imaginary|Integer|Real|String)\\b", TokenType::basicType },

			{ "([A-Za-z_][A-Za-z0-9_]*)\\b", TokenType::symbol },

		};

		Lexer() = default;
		~Lexer() = default;

		public:

		Lexer(const Lexer &) = delete;
		Lexer(Lexer &&) = delete;
		Lexer & operator = (const Lexer &) = delete;
		Lexer & operator = (Lexer &&) = delete;

		static Lexer * self() {
			static Lexer instance;
			return & instance;
		}

		ArrayList<Token> * tokenise(String * input, String fileName = "Unknown File") const {
			// Handle Last Token:
			String data = (* input) + "\n";
			// Handle Single Line Comments:
			data = handleComments(data);
			// Handle EndLines:
			data = RegexTools::replaceMatches("\n", data, " ");
			ArrayList<Token> * tokens = new ArrayList<Token>();
			UInt32 pos = 0;
			Token temp = Token("beginFile", TokenType::beginFile, 0);
			tokens -> push(temp);
			Bool previousInvalid = false;
			while (data.length() > 0) {
				Bool tokenised = false;
				for (TokenRule rule : grammar) {
					String result = RegexTools::matchCloseStart(rule.pattern, data);
					if (result.length() > 0) {
						tokenised = true;
						data = data.subString(result.length());
						temp = Token(result, rule.type, pos);
						pos += result.length();
						previousInvalid = false;
						if (rule.type == TokenType::empty) break;
						if (rule.type == TokenType::comment) break;
						tokens -> push(temp); break;
					}
				}
				if (!tokenised) {
					String s = "-"; s[0] = data[0];
					if (!previousInvalid) {
						tokens -> push({ s, TokenType::invalid, pos });
						previousInvalid = true;
					} else {
						tokens -> at(tokens -> size() - 1).lexeme += s;
					}
					pos += 1;
					data.erase(data.begin());
				}
			}
			tokens -> push({ "endFile", TokenType::endFile, 0 });
			tokens -> shrinkToFit();
			return tokens;
		}

	};

}

#endif
