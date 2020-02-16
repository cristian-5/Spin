
/*!
 *
 *    + --------------------------------------- +
 *    |  Lexer.cpp                              |
 *    |                                         |
 *    |              Language Lexer             |
 *    |                                         |
 *    |  Created by Cristian A.                 |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
!*/

#include "../Aliases/Prototypes/Lexer.hpp"

#ifndef SPIN_LEXER
#define SPIN_LEXER

#include "../Aliases/Prototypes/Token.hpp"
#include "../Aliases/Prototypes/Regex.hpp"

namespace Spin {

	CodeUnit::CodeUnit(Array<Token> * tokens, String * name, String * contents) {
		this -> tokens = tokens;
		this -> name = name;
		this -> contents = contents;
	}
	CodeUnit::~CodeUnit() {
		if (tokens) delete tokens;
		if (name) delete name;
		if (contents) delete contents;
	}

	Lexer::Lexer() {

		grammar = InitialiserList<TokenRule> {

			{ Regex("^([ \\t\\n]+)"), TokenType::empty },

			{ Regex("^(\\/\\*+[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/)"), TokenType::comment },

			{ Regex("^([0-9]+(?:\\.[0-9]+(?:[eE][-]?[0-9]+)?)?i)"), TokenType::imaginaryLiteral },
			{ Regex("^([0-9]+\\.[0-9]+(?:[eE][-]?[0-9]+)?)"), TokenType::realLiteral },
			{ Regex("^((?:0[x][0-9A-Fa-f]+)|(?:0b[01]+)|(?:0o[0-7]+)|(?:0d[0-9]+)|(?:[0-9]+))"), TokenType::intLiteral },
			{ Regex("^(\"(?:[^\\\\\"]|\\\\[\"\\\\0abfnrtv]|\\\\0x[0-9A-Fa-f]{2})*\")"), TokenType::stringLiteral },
			{ Regex("^('(?:[^\\\\]|\\\\0x[0-9A-Fa-f]{2}|\\\\['\\\\0abfnrtv])')"), TokenType::charLiteral },
			{ Regex("^(false|true)\\b"), TokenType::boolLiteral },

			{ Regex("^(<[01]\\|)"), TokenType::basisBraLiteral },
			{ Regex("^(\\|[01]>)"), TokenType::basisKetLiteral },

			{ Regex("^(<[A-Za-z_][A-Za-z0-9_]*\\|[A-Za-z_][A-Za-z0-9_]*>)"), TokenType::braketSymbol },
			{ Regex("^(<[A-Za-z_][A-Za-z0-9_]*\\|)"), TokenType::braSymbol },
			{ Regex("^(\\|[A-Za-z_][A-Za-z0-9_]*><[A-Za-z_][A-Za-z0-9_]*\\|)"), TokenType::ketbraSymbol },
			{ Regex("^(\\|[A-Za-z_][A-Za-z0-9_]*>)"), TokenType::ketSymbol },

			{ Regex("^(\\->)"), TokenType::arrow },
			{ Regex("^(\\::)"), TokenType::doublecolon },
			{ Regex("^(\\:)"), TokenType::colon },
			{ Regex("^(\\;)"), TokenType::semicolon },
			{ Regex("^(\\,)"), TokenType::comma },
			{ Regex("^(\\.)"), TokenType::dot },
			{ Regex("^(<=)"), TokenType::minorEqual },
			{ Regex("^(<)"), TokenType::minor },
			{ Regex("^(>=)"), TokenType::majorEqual },
			{ Regex("^(>)"), TokenType::major },
			{ Regex("^(==)"), TokenType::equality },
			{ Regex("^(=)"), TokenType::equal },
			{ Regex("^(\\?)"), TokenType::questionMark },
			{ Regex("^(\\!=)"), TokenType::inequality },
			{ Regex("^(\\!)"), TokenType::exclamationMark },

			{ Regex("^(\\+=)"), TokenType::plusEqual },
			{ Regex("^(\\+)"), TokenType::plus },
			{ Regex("^(\\-=)"), TokenType::minusEqual },
			{ Regex("^(\\-)"), TokenType::minus },
			{ Regex("^(\\~)"), TokenType::tilde },
			{ Regex("^(\\*=)"), TokenType::starEqual },
			{ Regex("^(\\*)"), TokenType::star },
			{ Regex("^(\\\\)"), TokenType::backslash },
			{ Regex("^(\\/=)"), TokenType::slashEqual },
			{ Regex("^(\\/)"), TokenType::slash },
			{ Regex("^(\\|=)"), TokenType::pipeEqual },
			{ Regex("^(\\|\\|)"), TokenType::OR },
			{ Regex("^(\\|)"), TokenType::pipe },
			{ Regex("^(\\&=)"), TokenType::ampersandEqual },
			{ Regex("^(\\&\\&)"), TokenType::AND },
			{ Regex("^(\\&)"), TokenType::ampersand },
			{ Regex("^(\\%=)"), TokenType::modulusEqual },
			{ Regex("^(\\%)"), TokenType::modulus },
			{ Regex("^(\\$=)"), TokenType::dollarEqual },
			{ Regex("^(\\$)"), TokenType::dollar },
			{ Regex("^(\\°)"), TokenType::conjugate },
			{ Regex("^(\\^)"), TokenType::transpose },
			{ Regex("^(\\')"), TokenType::dagger },

			{ Regex("^(\\()"), TokenType::openParenthesis },
			{ Regex("^(\\))"), TokenType::closeParenthesis },
			{ Regex("^(\\[)"), TokenType::openBracket },
			{ Regex("^(\\])"), TokenType::closeBracket },
			{ Regex("^(\\{)"), TokenType::openBrace },
			{ Regex("^(\\})"), TokenType::closeBrace },

			{ Regex("^(if)\\b"), TokenType::ifKeyword },
			{ Regex("^(else)\\b"), TokenType::elseKeyword },
			{ Regex("^(switch)\\b"), TokenType::switchKeyword },
			{ Regex("^(case)\\b"), TokenType::caseKeyword },
			{ Regex("^(default)\\b"), TokenType::defaultKeyword },
			{ Regex("^(while)\\b"), TokenType::whileKeyword },
			{ Regex("^(do)\\b"), TokenType::doKeyword },
			{ Regex("^(loop)\\b"), TokenType::loopKeyword },
			{ Regex("^(for)\\b"), TokenType::forKeyword },
			{ Regex("^(repeat)\\b"), TokenType::repeatKeyword },
			{ Regex("^(until)\\b"), TokenType::untilKeyword },
			{ Regex("^(break)\\b"), TokenType::breakKeyword },
			{ Regex("^(continue)\\b"), TokenType::continueKeyword },
			{ Regex("^(self)\\b"), TokenType::selfKeyword },

			{ Regex("^(new)\\b"), TokenType::newKeyword },
			{ Regex("^(delete)\\b"), TokenType::deleteKeyword },

			{ Regex("^(import)\\b"), TokenType::importKeyword },

			{ Regex("^(func)\\b"), TokenType::funcKeyword },
			{ Regex("^(proc)\\b"), TokenType::procKeyword },

			{ Regex("^(class)\\b"), TokenType::classKeyword },
			
			{ Regex("^(@(?:public))\\b"), TokenType::publicModifier },
			{ Regex("^(@(?:hidden))\\b"), TokenType::hiddenModifier },
			{ Regex("^(@(?:secure))\\b"), TokenType::secureModifier },
			{ Regex("^(@(?:static))\\b"), TokenType::staticModifier },
			{ Regex("^(@(?:shared))\\b"), TokenType::sharedModifier },

			{ Regex("^(@(?:create))\\b"), TokenType::createSpecifier },
			{ Regex("^(@(?:delete))\\b"), TokenType::deleteSpecifier },

			{ Regex("^(rest)\\b"), TokenType::restKeyword },
			{ Regex("^(return)\\b"), TokenType::returnKeyword },

			{ Regex("^(Bool|Byte|Character|Complex|Imaginary|Integer|Real|String|Vector)\\b"), TokenType::basicType },

			{ Regex("^([A-Za-z_][A-Za-z0-9_]*)\\b"), TokenType::symbol }

		};

	}
	void Lexer::removeComments(String & input) {
		if (input.empty()) return;
		try {
			Regex regex = Regex("\\/[\\/]+.*");
			SMatch match;
			std::regex_search(input, match, regex);
			while (match.size() >= 1) {
				SizeType len = match.str(0).length();
				SizeType pos = match.position(0);
				for (SizeType i = pos; i < pos + len; i += 1) {
					input[i] = ' ';
				}
				std::regex_search(input, match, regex);
			}
		} catch (RegexError & e) { }
	}
	Array<Token> * Lexer::tokenise(String * input) const {
		// Last Token Fallback:
		String data = (* input) + "\n";
		// Remove Single Line Comments:
		Lexer::removeComments(data);
		// Remove Line Endings:
		RegexTools::replaceMatches("\n", data, " ");
		Array<Token> * tokens = new Array<Token>();
		SizeType pos = 0;
		Token temp = Token("beginFile", TokenType::beginFile, 0);
		tokens -> push_back(temp);
		Bool previousInvalid = false;
		while (data.length() > 0) {
			Bool tokenised = false;
			for (TokenRule rule : grammar) {
				String result = RegexTools::findFirstGroup(rule.pattern, data);
				if (result.length() > 0) {
					tokenised = true;
					data = data.substr(result.length());
					temp = Token(result, rule.type, pos);
					pos += result.length();
					previousInvalid = false;
					if (rule.type == TokenType::empty) break;
					if (rule.type == TokenType::comment) break;
					tokens -> push_back(temp); break;
				}
			}
			if (!tokenised) {
				String s = "-"; s[0] = data[0];
				if (!previousInvalid) {
					tokens -> push_back({ s, TokenType::invalid, pos });
					previousInvalid = true;
				} else {
					tokens -> at(tokens -> size() - 1).lexeme += s;
				}
				pos += 1;
				data.erase(data.begin());
			}
		}
		tokens -> push_back({ "endFile", TokenType::endFile, 0 });
		tokens -> shrink_to_fit();
		return tokens;
	}

}

#endif
