
#include "../Aliases.hpp"

#ifndef SPIN_LEXER_HPP
#define SPIN_LEXER_HPP

#include <vector>

#include "Token.hpp"

namespace Spin {

	class CodeUnit {
		public:
		Array<Token> * tokens;
		String * name;
		String * contents;
		CodeUnit(Array<Token> * tokens,
				 String * name,
				 String * contents);
		~CodeUnit();
	};

	class Lexer {
		private:
		Array<TokenRule> grammar;
		static void removeComments(String & input);
		Lexer();
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
		Array<Token> * tokenise(String * input) const;
	};

}

#endif
