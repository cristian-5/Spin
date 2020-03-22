
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

namespace Spin {

	const Dictionary<String, TokenType> Lexer::reserved = {

		{ "if", TokenType::ifKeyword },
		{ "else", TokenType::elseKeyword },
		{ "swap", TokenType::swapKeyword },
		{ "while", TokenType::whileKeyword },
		{ "do", TokenType::doKeyword },
		{ "loop", TokenType::loopKeyword },
		{ "for", TokenType::forKeyword },
		{ "repeat", TokenType::repeatKeyword },
		{ "until", TokenType::untilKeyword },
		{ "break", TokenType::breakKeyword },
		{ "continue", TokenType::continueKeyword },
		{ "self", TokenType::selfKeyword },
		{ "new", TokenType::newKeyword },
		{ "delete", TokenType::deleteKeyword },
		{ "import", TokenType::importKeyword },
		{ "func", TokenType::funcKeyword },
		{ "proc", TokenType::procKeyword },
		{ "class", TokenType::classKeyword },
		{ "rest", TokenType::restKeyword },
		{ "return", TokenType::returnKeyword },

		{ "Boolean", TokenType::basicType },
		{ "Byte", TokenType::basicType },
		{ "Character", TokenType::basicType },
		{ "Complex", TokenType::basicType },
		{ "Imaginary", TokenType::basicType },
		{ "Integer", TokenType::basicType },
		{ "Real", TokenType::basicType },
		{ "String", TokenType::basicType },
		{ "Vector", TokenType::basicType },

		{ "false", TokenType::boolLiteral },
		{ "true", TokenType::boolLiteral },

	};

	const Dictionary<String, TokenType> Lexer::specifiers = {

		{ "@public", TokenType::publicModifier },
		{ "@hidden", TokenType::hiddenModifier },
		{ "@secure", TokenType::secureModifier },
		{ "@immune", TokenType::immuneModifier },
		{ "@static", TokenType::staticModifier },
		{ "@shared", TokenType::sharedModifier },

		{ "@create", TokenType::createSpecifier },
		{ "@delete", TokenType::deleteSpecifier },

	};

	void Lexer::scanToken() {
		Character c = advance();
		switch (c) {
			case ';': addToken(";", TokenType::semicolon); break;
			case '(': addToken("(", TokenType::openParenthesis); break;
			case ')': addToken(")", TokenType::closeParenthesis); break;
			case '{': addToken("{", TokenType::openBrace); break;
			case '}': addToken("}", TokenType::closeBrace); break;
			case '=':
				if (match('=')) addToken("==", TokenType::equality);
				else addToken("=", TokenType::equal); break;
			case '+':
				if (match('=')) addToken("+=", TokenType::plusEqual);
				else addToken("+", TokenType::plus); break;
			case '-':
				if (match('=')) addToken("-=", TokenType::minusEqual);
				else if (match('>')) addToken("->", TokenType::arrow);
				else addToken("-", TokenType::minus); break;
			case '*':
				if (match('=')) addToken("*=", TokenType::starEqual);
				else addToken("*", TokenType::star); break;
			case '/':
				if (match('=')) {
					addToken("/=", TokenType::slashEqual);
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
							TokenType::invalid
						);
					}
				} else addToken("/", TokenType::slash); break;
			case '<':
				if (match('=')) addToken("<=", TokenType::minorEqual);
				else if (peek() == '0' || peek() == '1') {
					scanBraLiteral();
				} else if (isAlpha(peek())) {
					scanBraKet();
				} else addToken("<", TokenType::minor); break;
			case '>':
				if (match('=')) addToken(">=", TokenType::majorEqual);
				else addToken(">", TokenType::major); break;
			case '[': addToken("[", TokenType::openBracket); break;
			case ']': addToken("]", TokenType::closeBracket); break;
			case ',': addToken(",", TokenType::comma); break;
			case '.': addToken(".", TokenType::dot); break;
			case '@': scanSpecifier(); break;
			case '"': scanString(); break;
			case '\'': scanCharacter(); break;
			case '\\':
				addToken("\\", TokenType::backslash); break;
			case '!':
				if (match('=')) addToken("!=", TokenType::inequality);
				else addToken("!", TokenType::exclamationMark); break;
			case '|':
				if (match('=')) addToken("|=", TokenType::pipeEqual);
				else if (match('|')) addToken("||", TokenType::OR);
				else if (peek() == '0' || peek() == '1') {
					scanKetLiteral();
				} else if (isAlpha(peek())) {
					scanKetBra();
				} else addToken("|", TokenType::pipe); break;
			case '&':
				if (match('=')) addToken("&=", TokenType::ampersandEqual);
				else if (match('&')) addToken("&&", TokenType::AND);
				else addToken("&", TokenType::ampersand); break;
			case '%':
				if (match('=')) addToken("%=", TokenType::modulusEqual);
				else addToken("%", TokenType::modulus); break;
			case '$':
				if (match('=')) addToken("$=", TokenType::dollarEqual);
				else addToken("$", TokenType::dollar); break;
			case ':':
				if (match(':')) addToken("::", TokenType::doublecolon);
				else addToken(":", TokenType::colon); break;
			case '~': addToken("~", TokenType::tilde); break;
			case '\xC2': // ° = C2 B0 : Character 16 { shift + (à°#) }
				if (match('\xB0')) addToken("°", TokenType::conjugate);
				else unknown.push_back(c); break;
			case '\xE2': // † = E2 80 A0 : Character 24 { option + X }
				if (match('\x80')) {
					if (match('\xA0')) addToken("†", TokenType::dagger);
					else {
						unknown.push_back(c);
						unknown.push_back('\x80');
					}
				} else unknown.push_back(c); break;
			case '^': addToken("^", TokenType::transpose); break;
			case '?': addToken("?", TokenType::questionMark); break;
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
			addToken("<", TokenType::minor);
			return;
		}
		SizeType save = index;
		if (!isAlpha(peek())) {
			addToken(
				source-> substr(start, index - start),
				TokenType::braSymbol
			);
			return;
		}
		do advance(); while (isAlphaNumeric(peek()));
		if (!match('>')) {
			index = save;
			addToken(
				source-> substr(start, index - start),
				TokenType::braSymbol
			);
			return;
		}
		addToken(
			source-> substr(start, index - start),
			TokenType::braketSymbol
		);
	}

	void Lexer::scanKetBra() {
		while (isAlphaNumeric(peek())) advance();
		if (!match('>')) {
			index = start + 1;
			addToken("|", TokenType::pipe);
			return;
		}
		SizeType save = index;
		if (!match('<')) {
			addToken(
				source-> substr(start, index - start),
				TokenType::ketSymbol
			);
			return;
		}
		if (!isAlpha(peek())) {
			index = save;
			addToken(
				source-> substr(start, index - start),
				TokenType::braSymbol
			);
			return;
		}
		do advance(); while (isAlphaNumeric(peek()));
		if (!match('|')) {
			index = save;
			addToken(
				source-> substr(start, index - start),
				TokenType::braSymbol
			);
			return;
		}
		addToken(
			source-> substr(start, index - start),
			TokenType::ketbraSymbol
		);
	}

	void Lexer::scanNumber() {
		TokenType type = TokenType::intLiteral;
		if (peekPrev() == '0') {
			// Try to parse base:
			if (match('x')) {
				Character x = peek();
				if (!((x >= '0' && x <= '9') ||
					  (x >= 'A' && x <= 'F'))) {
					// '0xSomething' so we return '0':
					index = start + 1;
					addToken("0", TokenType::intLiteral);
					return;
				}
				while ((x >= '0' && x <= '9') ||
					   (x >= 'A' && x <= 'F')) {
					advance();
					x = peek();
				}
				addToken(
					source -> substr(start, index - start),
					TokenType::intLiteral
				);
				return;
			} else if (match('b')) {
				Character b = peek();
				if (b != '0' && b != '1') {
					// '0bSomething' so we return '0':
					index = start + 1;
					addToken("0", TokenType::intLiteral);
					return;
				}
				while (b == '0' || b == '1') {
					advance();
					b = peek();
				}
				addToken(
					source -> substr(start, index - start),
					TokenType::intLiteral
				);
				return;
			} else if (match('o')) {
				Character o = peek();
				if (o < '0' || o > '7') {
					// '0oSomething' so we return '0':
					index = start + 1;
					addToken("0", TokenType::intLiteral);
					return;
				}
				while (o >= '0' && o <= '7') {
					advance();
					o = peek();
				}
				addToken(
					source -> substr(start, index - start),
					TokenType::intLiteral
				);
				return;
			} else if (match('d')) {
				Character d = peek();
				if (d < '0' || d > '9') {
					// '0dSomething' so we return '0':
					index = start + 1;
					addToken("0", TokenType::intLiteral);
					return;
				}
				while (d >= '0' && d <= '9') {
					advance();
					d = peek();
				}
				addToken(
					source -> substr(start, index - start),
					TokenType::intLiteral
				);
				return;
			}
		}
		while (isDigit(peek())) advance();
		if (peek() == '.' && isDigit(peekNext())) {
			type = TokenType::realLiteral;
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
		if (match('i')) type = TokenType::imaginaryLiteral;
		addToken(source -> substr(start, index - start), type);
	}

	void Lexer::scanSymbol() {
		while (isAlphaNumeric(peek())) advance();
		String lexeme = source -> substr(start, index - start);
		auto search = reserved.find(lexeme);
		if (search != reserved.end()) {
			addToken(lexeme, search -> second);
		} else addToken(lexeme, TokenType::symbol);
	}

	void Lexer::scanString() {
		while (peek() != '"' && !isAtEnd()) {
			if (match('\\')) match('"');
			else advance();
		}
		if (isAtEnd()) {
			addToken(
				source -> substr(start, index - start),
				TokenType::invalid
			);
			return;
		}
		advance();
		addToken(
			source -> substr(start, index - start),
			TokenType::stringLiteral
		);
	}

	void Lexer::scanSpecifier() {
		while (isAlphaNumeric(peek())) advance();
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
			addToken("<", TokenType::minor);
			return;
		}
		addToken(
			source -> substr(start, index - start),
			TokenType::basisBraLiteral
		);
	}

	void Lexer::scanKetLiteral() {
		advance();
		if (!match('>')) {
			index = start + 1;
			addToken("|", TokenType::pipe);
			return;
		}
		addToken(
			source -> substr(start, index - start),
			TokenType::basisKetLiteral
		);
	}

	void Lexer::scanCharacter() {
		if (match('\\')) {
			if (isAtEnd()) {
				unknown.push_back('\'');
				unknown.push_back('\\');
				return;
			}
			advance();
		} else if (match('\'')) {
			unknown.push_back('\'');
			unknown.push_back('\'');
			return;
		} else if (isAtEnd()) {
			unknown.push_back('\'');
			return;
		} else advance();
		if (match('\'')) {
			addToken(
				source -> substr(start, index - start),
				TokenType::charLiteral
			);
			return;
		}
		// Ending quote not found:
		unknown += source -> substr(start, index - start);
	}

	void Lexer::addToken(Token t) {
		tokens -> push_back(t);
	}
	void Lexer::addToken(String l, TokenType t) {
		tokens -> push_back({ l, t, start });
	}
	void Lexer::addInvalid(Token t) {
		t.type = TokenType::invalid;
		if (tokens -> empty() ||
			tokens -> at(0).type == TokenType::beginFile) {
			tokens -> push_back(t);
			return;
		}
		Token l = tokens -> at(tokens -> size() - 1);
		tokens -> pop_back();
		tokens -> push_back(t);
		tokens -> push_back(l);
	}
	void Lexer::addInvalid(String i) {
		Token t = { i, TokenType::invalid, 0 };
		if (tokens -> empty() ||
			tokens -> at(0).type == TokenType::beginFile) {
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
				{ "beginFile", TokenType::beginFile, 0 },
				{ "endFile", TokenType::endFile, 0 }
			});
		}
		source = new String(* input);
		tokens = new Array<Token>();
		addToken({ "beginFile", TokenType::beginFile, 0 });
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
				TokenType::invalid,
				start - unknown.length() + 1
			});
		}
		addToken({ "endFile", TokenType::endFile, 0 });
		Array<Token> * result = tokens;
		resetState();
		return result;
	}

}

#endif
