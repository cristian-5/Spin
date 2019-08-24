
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

#define INVERTED "[^A-Za-z0-9_]"

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

			{ "([0-9]+(?:\\.[0-9]+(?:[eE][0-9]+)?)?i)", TokenType::imaginaryLiteral },
			{ "([0-9]+\\.[0-9]+(?:[eE][0-9]+)?)", TokenType::realLiteral },
			{ "((?:0[x][0-9A-Fa-f]+)|(?:0b[01]+)|(?:0o[0-7]+)|(?:0d[0-9]+)|(?:[0-9]+))", TokenType::intLiteral },
			{ "(\"(?:[^\\\\\"]|\\\\[\"\\\\0abfnrtv]|\\\\0x[0-9A-Fa-f]{2})*\")", TokenType::stringLiteral },
			{ "('(?:[^\\\\]|\\\\0x[0-9A-Fa-f]{2}|\\\\['\\\\0abfnrtv])')", TokenType::charLiteral },
			{ "(#[A-Fa-f0-9]{6}(?:[A-Fa-f0-9][A-Fa-f0-9])?|#[A-Fa-f0-9]{3,4})" INVERTED, TokenType::colourLiteral },
			{ "(true|false)" INVERTED, TokenType::boolLiteral },

			{ "(<[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*\\|[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*>)", TokenType::braketSymbol },
			{ "(<[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*\\|)", TokenType::braSymbol },
			{ "(\\|[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*>)", TokenType::ketSymbol },
			
			{ "(<[01]+\\|[01]+>)", TokenType::braketLiteral },
			{ "(<[01]+\\|)", TokenType::braLiteral },
			{ "(\\|[01]+>)", TokenType::ketLiteral },

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
			{ "(\\|)", TokenType::pipe },
			{ "(\\#)", TokenType::hashtag },
			{ "(\\&=)", TokenType::ampersandEqual },
			{ "(\\&)", TokenType::ampersand },
			{ "(\\%=)", TokenType::modulusEqual },
			{ "(\\%)", TokenType::modulus },
			{ "(\\$)", TokenType::dollar },
			{ "(\\^=)", TokenType::hatEqual },
			{ "(\\^)", TokenType::hat },

			{ "(\\()", TokenType::openRoundBracket },
			{ "(\\))", TokenType::closeRoundBracket },
			{ "(\\[)", TokenType::openSquareBracket },
			{ "(\\])", TokenType::closeSquareBracket },
			{ "(\\{)", TokenType::openCurlyBracket },
			{ "(\\})", TokenType::closeCurlyBracket },

			{ "(try)" INVERTED, TokenType::tryKeyword },
			{ "(catch)" INVERTED, TokenType::catchKeyword },
			{ "(throw)" INVERTED, TokenType::throwKeyword },
			{ "(throws)" INVERTED, TokenType::throwsKeyword },
			{ "(avoid)" INVERTED, TokenType::avoidKeyword },

			{ "(if)" INVERTED, TokenType::ifKeyword },
			{ "(else)" INVERTED, TokenType::elseKeyword },
			{ "(switch)" INVERTED, TokenType::ifKeyword },
			{ "(case)" INVERTED, TokenType::caseKeyword },
			{ "(default)" INVERTED, TokenType::defaultKeyword },
			{ "(while)" INVERTED, TokenType::whileKeyword },
			{ "(do)" INVERTED, TokenType::doKeyword },
			{ "(loop)" INVERTED, TokenType::loopKeyword },
			{ "(for)" INVERTED, TokenType::forKeyword },
			{ "(repeat)" INVERTED, TokenType::repeatKeyword },
			{ "(until)" INVERTED, TokenType::untilKeyword },
			{ "(break)" INVERTED, TokenType::breakKeyword },
			{ "(continue)" INVERTED, TokenType::continueKeyword },

			{ "(import)" INVERTED, TokenType::importKeyword },
			{ "(func)" INVERTED, TokenType::funcKeyword },
			{ "(proc)" INVERTED, TokenType::procKeyword },
			{ "(static)" INVERTED, TokenType::staticKeyword },
			{ "(class)" INVERTED, TokenType::classKeyword },
			{ "(enumerator)" INVERTED, TokenType::enumKeyword },
			{ "(structure)" INVERTED, TokenType::structKeyword },
			{ "(exception)" INVERTED, TokenType::exceptKeyword },
			{ "(private)" INVERTED, TokenType::privateKeyword },
			{ "(public)" INVERTED, TokenType::publicKeyword },
			{ "(ref)" INVERTED, TokenType::refKeyword },
			{ "(cpy)" INVERTED, TokenType::cpyKeyword },
			{ "(const)" INVERTED, TokenType::constKeyword },
			{ "(null)" INVERTED, TokenType::nullLiteral },
			{ "(nevermind)" INVERTED, TokenType::nevermind },
			{ "(return)" INVERTED, TokenType::returnKeyword },

			{ "([A-Za-z_][A-Za-z0-9_]*)" INVERTED, TokenType::symbol },

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
			Boolean previousInvalid = false;
			while (data.length() > 0) {
				Boolean tokenised = false;
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
