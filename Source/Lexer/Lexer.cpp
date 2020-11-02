
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

#include "Lexer.hpp"

#ifndef SPIN_LEXER_CPP
#define SPIN_LEXER_CPP

namespace Spin {

	const Dictionary<String, Token::Type> Lexer::reserved = {

		{ "var", Token::Type::varKeyword },
		{ "con", Token::Type::conKeyword },
		{ "vec", Token::Type::vecKeyword },
		{ "mat", Token::Type::matKeyword },

		{       "if", Token::Type::ifKeyword       },
		{     "else", Token::Type::elseKeyword     },
		{     "swap", Token::Type::swapKeyword     },
		{    "while", Token::Type::whileKeyword    },
		{       "do", Token::Type::doKeyword       },
		{     "loop", Token::Type::loopKeyword     },
		{      "for", Token::Type::forKeyword      },
		{   "repeat", Token::Type::repeatKeyword   },
		{    "until", Token::Type::untilKeyword    },
		{    "break", Token::Type::breakKeyword    },
		{ "continue", Token::Type::continueKeyword },
		{     "self", Token::Type::selfKeyword     },
		{    "sleep", Token::Type::sleepKeyword    },
		{    "clock", Token::Type::clockKeyword    },
		{   "random", Token::Type::randomKeyword   },
		{   "import", Token::Type::importKeyword   },
		{     "func", Token::Type::funcKeyword     },
		{     "proc", Token::Type::procKeyword     },
		{     "rest", Token::Type::restKeyword     },
		{   "return", Token::Type::returnKeyword   },

		{ "write", Token::Type::writeKeyword },
		{  "read", Token::Type::readKeyword  },

		{   "Boolean", Token::Type::basicType },
		{      "Byte", Token::Type::basicType },
		{ "Character", Token::Type::basicType },
		{   "Complex", Token::Type::basicType },
		{ "Imaginary", Token::Type::basicType },
		{   "Integer", Token::Type::basicType },
		{      "Real", Token::Type::basicType },
		{    "String", Token::Type::basicType },

		{ "false", Token::Type::boolLiteral },
		{  "true", Token::Type::boolLiteral },

		{  "infinity", Token::Type::realIdiom },
		{ "undefined", Token::Type::realIdiom },

	};
	const Dictionary<String, Token::Type> Lexer::specifiers = {

		{ "@public", Token::Type::publicModifier },
		{ "@hidden", Token::Type::hiddenModifier },
		{ "@secure", Token::Type::secureModifier },
		{ "@immune", Token::Type::immuneModifier },
		{ "@static", Token::Type::staticModifier },
		{ "@shared", Token::Type::sharedModifier },

		{ "@create", Token::Type::createSpecifier },
		{ "@delete", Token::Type::deleteSpecifier },

	};

	void Lexer::scanToken() {
		Character c = advance();
		switch (c) {
			case ';': addToken(";", Token::Type::semicolon); break;
			case '(': addToken("(", Token::Type::openParenthesis); break;
			case ')': addToken(")", Token::Type::closeParenthesis); break;
			case '{': addToken("{", Token::Type::openBrace); break;
			case '}': addToken("}", Token::Type::closeBrace); break;
			case '=':
				if (match('=')) addToken("==", Token::Type::equality);
				else addToken("=", Token::Type::equal); break;
			case '+':
				if (match('=')) addToken("+=", Token::Type::plusEqual);
				else addToken("+", Token::Type::plus); break;
			case '-':
				if (match('=')) addToken("-=", Token::Type::minusEqual);
				else if (match('>')) addToken("->", Token::Type::arrow);
				else addToken("-", Token::Type::minus); break;
			case '*':
				if (match('=')) addToken("*=", Token::Type::starEqual);
				else if (match('>')) addToken("*>", Token::Type::rotateR);
				else addToken("*", Token::Type::star); break;
			case '/':
				if (match('=')) {
					addToken("/=", Token::Type::slashEqual);
				} else if (match('/')) {
					while (peek() != '\n' && !isAtEnd()) advance();
				} else if (match('*')) {
					Boolean exit = false;
					while (!exit) {
						while (peek() != '*' && !isAtEnd()) advance();
						if (isAtEnd()) break;
						while (peek() == '*' && !isAtEnd()) advance();
						if (isAtEnd()) break;
						if (match('/')) { exit = true; break; }
					}
					if (!exit) {
						addToken(
							source -> substr(start, index - start),
							Token::Type::invalid
						);
					}
				} else addToken("/", Token::Type::slash); break;
			case '<':
				if (match('=')) addToken("<=", Token::Type::minorEqual);
				else if (match('<')) addToken("<<", Token::Type::shiftL);
				else if (match('*')) addToken("<*", Token::Type::rotateL);
				else if (peek() == '0' || peek() == '1') {
					scanBraLiteral();
				} else if (isAlpha(peek())) {
					scanBraKet();
				} else addToken("<", Token::Type::minor); break;
			case '>':
				if (match('=')) addToken(">=", Token::Type::majorEqual);
				else if (match('>')) {
					addToken(">>", Token::Type::shiftR);
				} else addToken(">", Token::Type::major); break;
			case '[': addToken("[", Token::Type::openBracket); break;
			case ']': addToken("]", Token::Type::closeBracket); break;
			case ',': addToken(",", Token::Type::comma); break;
			case '.': addToken(".", Token::Type::dot); break;
			case '@': scanSpecifier(); break;
			case '"': scanString(); break;
			case '\'': scanCharacter(); break;
			case '\\':
				addToken("\\", Token::Type::backslash); break;
			case '!':
				if (match('=')) addToken("!=", Token::Type::inequality);
				else addToken("!", Token::Type::exclamationMark); break;
			case '|':
				if (match('=')) addToken("|=", Token::Type::pipeEqual);
				else if (match('|')) addToken("||", Token::Type::OR);
				else if (peek() == '0' || peek() == '1') {
					scanKetLiteral();
				} else if (isAlpha(peek())) {
					scanKetBra();
				} else addToken("|", Token::Type::pipe); break;
			case '$':
				if (match('=')) addToken("$=", Token::Type::dollarEqual);
				else addToken("$", Token::Type::dollar); break;
			case '&':
				if (match('=')) addToken("&=", Token::Type::ampersandEqual);
				else if (match('&')) addToken("&&", Token::Type::AND);
				else addToken("&", Token::Type::ampersand); break;
			case '%':
				if (match('=')) addToken("%=", Token::Type::modulusEqual);
				else addToken("%", Token::Type::modulus); break;
			case ':':
				if (match(':')) addToken("::", Token::Type::doublecolon);
				else addToken(":", Token::Type::colon); break;
			case '~': addToken("~", Token::Type::tilde); break;
			case '\xC2': // ° = C2 B0 : Character 16 { shift + (à°#) }
				if (match('\xB0')) addToken("°", Token::Type::conjugate);
				else unknown.push_back(c); break;
			case '\xE2': // † = E2 80 A0 : Character 24 { option + X }
				if (match('\x80')) {
					if (match('\xA0')) addToken("†", Token::Type::dagger);
					else {
						unknown.push_back(c);
						unknown.push_back('\x80');
					}
				} else unknown.push_back(c); break;
			case '^': addToken("^", Token::Type::hat); break;
			case '?': addToken("?", Token::Type::questionMark); break;
			case '\xC6': 
				if (match('\x92')) addToken("ƒ", Token::Type::lamda);
				else unknown.push_back(c); break;
			case ' ': case '\r': case '\t': case '\n': break;
			default: 
				if (isDigit(c)) scanNumber();
				else if (isAlpha(c)) scanSymbol();
				else unknown.push_back(c);
			break;
		}
	}
	void Lexer::scanBraKet() {
		while (isAlphaNumeric(peek())) advance();
		if (!match('|')) {
			index = start + 1;
			addToken("<", Token::Type::minor);
			return;
		}
		SizeType save = index;
		if (!isAlpha(peek())) {
			addToken(
				source-> substr(start, index - start),
				Token::Type::braSymbol
			);
			return;
		}
		do advance(); while (isAlphaNumeric(peek()));
		if (!match('>')) {
			index = save;
			addToken(
				source-> substr(start, index - start),
				Token::Type::braSymbol
			);
			return;
		}
		addToken(
			source-> substr(start, index - start),
			Token::Type::braketSymbol
		);
	}
	void Lexer::scanKetBra() {
		while (isAlphaNumeric(peek())) advance();
		if (!match('>')) {
			index = start + 1;
			addToken("|", Token::Type::pipe);
			return;
		}
		SizeType save = index;
		if (!match('<')) {
			addToken(
				source-> substr(start, index - start),
				Token::Type::ketSymbol
			);
			return;
		}
		if (!isAlpha(peek())) {
			index = save;
			addToken(
				source-> substr(start, index - start),
				Token::Type::braSymbol
			);
			return;
		}
		do advance(); while (isAlphaNumeric(peek()));
		if (!match('|')) {
			index = save;
			addToken(
				source-> substr(start, index - start),
				Token::Type::braSymbol
			);
			return;
		}
		addToken(
			source-> substr(start, index - start),
			Token::Type::ketbraSymbol
		);
	}
	void Lexer::scanNumber() {
		Token::Type type = Token::Type::intLiteral;
		if (peekPrev() == '0') {
			// Try to parse base:
			if (match('x')) {
				Character x = peek();
				if (!((x >= '0' && x <= '9') ||
					  (x >= 'A' && x <= 'F'))) {
					// '0xSomething' so we return '0':
					index = start + 1;
					addToken("0", Token::Type::intLiteral);
					return;
				}
				while ((x >= '0' && x <= '9') ||
					   (x >= 'A' && x <= 'F')) {
					advance();
					x = peek();
				}
				addToken(
					source -> substr(start, index - start),
					Token::Type::intLiteral
				);
				return;
			} else if (match('b')) {
				Character b = peek();
				if (b != '0' && b != '1') {
					// '0bSomething' so we return '0':
					index = start + 1;
					addToken("0", Token::Type::intLiteral);
					return;
				}
				while (b == '0' || b == '1') {
					advance();
					b = peek();
				}
				addToken(
					source -> substr(start, index - start),
					Token::Type::intLiteral
				);
				return;
			} else if (match('o')) {
				Character o = peek();
				if (o < '0' || o > '7') {
					// '0oSomething' so we return '0':
					index = start + 1;
					addToken("0", Token::Type::intLiteral);
					return;
				}
				while (o >= '0' && o <= '7') {
					advance();
					o = peek();
				}
				addToken(
					source -> substr(start, index - start),
					Token::Type::intLiteral
				);
				return;
			} else if (match('d')) {
				Character d = peek();
				if (d < '0' || d > '9') {
					// '0dSomething' so we return '0':
					index = start + 1;
					addToken("0", Token::Type::intLiteral);
					return;
				}
				while (d >= '0' && d <= '9') {
					advance();
					d = peek();
				}
				addToken(
					source -> substr(start, index - start),
					Token::Type::intLiteral
				);
				return;
			}
		}
		while (isDigit(peek())) advance();
		if (peek() == '.' && isDigit(peekNext())) {
			type = Token::Type::realLiteral;
			advance();
			while (isDigit(peek())) advance();
			if (match('e')) {
				match('-');
				if (!isDigit(peek())) {
					// 123.456ea || 123.46e-a:
					unknown += source -> substr(start, index - start);
					return;
				}
				while (isDigit(peek())) advance();
			}
		}
		if (match('i')) type = Token::Type::imaginaryLiteral;
		addToken(source -> substr(start, index - start), type);
	}
	void Lexer::scanSymbol() {
		while (isAlphaNumeric(peek())) advance();
		String lexeme = source -> substr(start, index - start);
		auto search = reserved.find(lexeme);
		if (search != reserved.end()) {
			addToken(lexeme, search -> second);
		} else addToken(lexeme, Token::Type::symbol);
	}
	void Lexer::scanString() {
		while (peek() != '"' && !isAtEnd()) {
			if (match('\\')) match('"');
			else advance();
		}
		if (isAtEnd()) {
			addToken(
				source -> substr(start, index - start),
				Token::Type::invalid
			);
			return;
		}
		advance();
		addToken(
			source -> substr(start, index - start),
			Token::Type::stringLiteral
		);
	}
	void Lexer::scanSpecifier() {
		if (!isAlpha(peek())) {
			addToken("@", Token::Type::at);
			return;
		}
		while (isAlpha(peek())) advance();
		String lexeme = source -> substr(start, index - start);
		auto search = specifiers.find(lexeme);
		if (search != specifiers.end()) {
			addToken(lexeme, search -> second);
		} else unknown += lexeme;
	}
	void Lexer::scanBraLiteral() {
		advance();
		if (!match('|')) {
			index = start + 1;
			addToken("<", Token::Type::minor);
			return;
		}
		addToken(
			source -> substr(start, index - start),
			Token::Type::basisBraLiteral
		);
	}
	void Lexer::scanKetLiteral() {
		advance();
		if (!match('>')) {
			index = start + 1;
			addToken("|", Token::Type::pipe);
			return;
		}
		addToken(
			source -> substr(start, index - start),
			Token::Type::basisKetLiteral
		);
	}
	void Lexer::scanCharacter() {
		if (isAtEnd()) {
			addToken("'", Token::Type::transpose);
			return;
		}
		advance();
		if (isAtEnd()) {
			index = start + 1;
			addToken("'", Token::Type::transpose);
			return;
		}
		if (!match('\'')) {
			index = start + 1;
			addToken("'", Token::Type::transpose);
			return;
		}
		addToken(
			source -> substr(start, index - start),
			Token::Type::charLiteral
		);
	}
	void Lexer::addToken(Token t) {
		tokens -> push_back(t);
	}
	void Lexer::addToken(String l, Token::Type t) {
		tokens -> push_back({ l, t, start });
	}
	void Lexer::addInvalid(Token t) {
		t.type = Token::Type::invalid;
		if (tokens -> empty() ||
			tokens -> at(0).type == Token::Type::beginFile) {
			tokens -> push_back(t);
			return;
		}
		Token l = tokens -> at(tokens -> size() - 1);
		tokens -> pop_back();
		tokens -> push_back(t);
		tokens -> push_back(l);
	}
	void Lexer::addInvalid(String i) {
		Token t = { i, Token::Type::invalid, 0 };
		if (tokens -> empty() ||
			tokens -> at(0).type == Token::Type::beginFile) {
			tokens -> push_back(t);
			return;
		}
		Token l = tokens -> at(tokens -> size() - 1);
		tokens -> pop_back();
		t.position = start - i.length();
		tokens -> push_back(t);
		tokens -> push_back(l);
	}
	Boolean Lexer::match(Character c) {
		if (isAtEnd()) return false;
		if (source -> at(index) != c) return false;
		index += 1;
		return true;
	}
	Boolean Lexer::isAtEnd() {
		return index >= source -> length();
	}
	Character Lexer::peek() {
		if (isAtEnd()) return '\0';
		return source -> at(index);
	}
	Character Lexer::peekPrev() {
		if (index - 1 < 0) return '\0';
		return source -> at(index - 1);
	}
	Character Lexer::peekNext() {
		if (index + 1 >= source -> length()) return '\0';
		return source -> at(index + 1);
	}
	Character Lexer::advance() {
		index += 1;
		return source -> at(index - 1);
	}
	Boolean Lexer::isDigit(Character d) {
		return d >= '0' && d <= '9';
	}
	Boolean Lexer::isAlpha(Character a) {
		return (a >= 'a' && a <= 'z') ||      
			   (a >= 'A' && a <= 'Z') ||
				a == '_';
	}
	Boolean Lexer::isAlphaNumeric(Character c) {
		return isAlpha(c) || isDigit(c);
	}
	void Lexer::resetState() {
		tokens = nullptr;
		source = nullptr;
		start = 0;
		index = 0;
		unknown.clear();
	}
	Array<Token> * Lexer::tokenise(String * input) {
		if (!input || input -> empty()) {
			return new Array<Token>({
				{ "beginFile", Token::Type::beginFile, 0 },
				{ "endFile", Token::Type::endFile, 0 }
			});
		}
		source = new String(* input);
		tokens = new Array<Token>();
		addToken({ "beginFile", Token::Type::beginFile, 0 });
		while (!isAtEnd()) {
			start = index;
			SizeType l = unknown.length();
			scanToken();
			if (!unknown.empty() && unknown.length() == l) {
				// Found a good token, lex the unknown:
				addInvalid(unknown);
				unknown.clear();
			}
		}
		// Unknow token at the end:
		if (!unknown.empty()) {
			addToken({
				unknown,
				Token::Type::invalid,
				start - unknown.length() + 1
			});
		}
		addToken({ "endFile", Token::Type::endFile, 0 });
		Array<Token> * result = tokens;
		resetState();
		return result;
	}

}

#endif
