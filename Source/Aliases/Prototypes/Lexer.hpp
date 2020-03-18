
#include "../Aliases.hpp"

#ifndef SPIN_LEXER_HPP
#define SPIN_LEXER_HPP

#include <vector>
#include <unordered_map>

#include "Token.hpp"

namespace Spin {

	class Lexer {
		private:
		static const Dictionary<String, TokenType> keywords;
		static const Dictionary<String, TokenType> specifiers;
		String unknown;
		Array<Token> * tokens = nullptr;
		String * source = nullptr;
		SizeType start = 0;
		SizeType index = 0;
		void scanToken();
		void scanBraKet();
		void scanKetBra();
		void scanNumber();
		void scanSymbol();
		void scanString();
		void scanSpecifier();
		void scanCharacter();
		void scanBraLiteral();
		void scanKetLiteral();
		inline void addToken(Token t);
		inline void addToken(String l, TokenType t);
		inline void addInvalid(Token t);
		inline void addInvalid(String i);
		inline Bool match(Character c);
		inline Bool isAtEnd();
		inline Character peek();
		inline Character peekPrev();
		inline Character peekNext();
		inline Character advance();
		inline Bool isDigit(Character d);
		inline Bool isAlpha(Character a);
		inline Bool isAlphaNumeric(Character c);
		inline void resetState();
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
		Array<Token> * tokenise(String * input);
	};

}

#endif
