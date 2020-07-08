
#include "../Common/Header.hpp"

#ifndef SPIN_LEXER_HPP
#define SPIN_LEXER_HPP

#include <vector>
#include <unordered_map>

#include "../Token/Token.hpp"

namespace Spin {

	class Lexer {
		private:
		static const Dictionary<String, Token::Type> reserved;
		static const Dictionary<String, Token::Type> specifiers;
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
		inline void addToken(String l, Token::Type t);
		inline void addInvalid(Token t);
		inline void addInvalid(String i);
		inline Boolean match(Character c);
		inline Boolean isAtEnd();
		inline Character peek();
		inline Character peekPrev();
		inline Character peekNext();
		inline Character advance();
		inline Boolean isDigit(Character d);
		inline Boolean isAlpha(Character a);
		inline Boolean isAlphaNumeric(Character c);
		inline void resetState();
		Lexer() = default;
		~Lexer() = default;
		public:
		Lexer(const Lexer &) = delete;
		Lexer(Lexer &&) = delete;
		Lexer & operator = (const Lexer &) = delete;
		Lexer & operator = (Lexer &&) = delete;
		inline static Lexer * self() {
			static Lexer instance;
			return & instance;
		}
		Array<Token> * tokenise(String * input);
	};

}

#endif
