
/*!
 *
 *    + --------------------------------------- +
 *    |  Parser.hpp                             |
 *    |                                         |
 *    |                  Parser                 |
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

#ifndef STACKPARSER
#define STACKPARSER

#include "../Aliases/Aliases.hpp"
#include "../Collection/Collection.hpp"
#include "../Syntax/SRule.hpp"

#include "ASTree.hpp"
#include "Exceptions.hpp"

using namespace Collection;

namespace Stack {

	/*!
	 *   @brief Invalid Grammar Exception.
	 *   Raised when the grammar is not invaid.
	 */
	class InvalidGrammarException: public Exception {
		public: InvalidGrammarException(): Exception() { }
	};

	/*!
	 *   @brief Invalid Pointer Exception.
	 *   Raised when the pointer is not invaid.
	 */
	class InvalidPointerException: public Exception {
		public: InvalidPointerException(): Exception() { }
	};

	/*!
	 *   @brief Empty Grammar Exception.
	 *   Raised when the grammar is empty.
	 */
	class EmptyGrammarException: public Exception {
		public: EmptyGrammarException(): Exception() { }
	};

	/*!
	 *   @brief Empty Token Exception.
	 *   Raised when the grammar is empty.
	 */
	class EmptyTokenException: public Exception {
		public: EmptyTokenException(): Exception() { }
	};

	class Parser {

		private:

		StrongList<Token> * tokens = nullptr;

		Grammar * grammar = nullptr;

		UInt32 lastTerminal = 0;

		Boolean parse(SRule * r, UInt32 index) {
			if (index >= tokens -> count()) return false;
			Token t = tokens -> getNode(index);
			Boolean matches = r -> matches(& t);
			if (r -> isTerminal()) {
				if (matches) lastTerminal = index;
				return matches;
			}
			if (!matches) return false;
			index += 1;
			for (UInt32 j = 0; j < r -> nextRules.count(); j++) {
				if (parse(r -> nextRules[j], index)) return true;
			}
			return false;
		}

		public:

		Parser(StrongList<Token> * t, Grammar * g) {
			if (t == nullptr) throw InvalidPointerException();
			if (g == nullptr) throw InvalidPointerException();
			if (t -> isEmpty()) throw EmptyTokenException();
			if (g -> isEmpty()) throw EmptyGrammarException();
			tokens = t; grammar = g;
		}

		Boolean parseFrom(UInt32 start = 0) {
			return parse(grammar, start);
		}

		Boolean parseRange(UInt32 start, UInt32 & end) {
			Boolean result = parse(grammar, start);
			end = lastTerminal;
			return result;
		}

	};

	class TreeParser {

		private:
		
		StrongList<Token> * tokens = nullptr;

		UInt32 index = 0;

		Expression * equality() {
			Expression * ex = comparison();
			StrongList<String> * ops = new StrongList<String>();
			String s = "=="; ops -> link(s);
			s = "!="; ops -> link(s);
			while (match(TokenType::infixOperator)
				   && matchOperators(ops)) {
				Token op = previous();
				Expression * rs = comparison();
				//ex = new Binary(ex, op, rs); // TODO: Fix!
			}
			delete ops;
			return ex;
		}

		Expression * comparison() {
			return nullptr;
		}

		Boolean matchOperator(String op) {
			if (checkOperator(op)) {
				advance();
				return true;
			} return false;
		}

		Boolean matchOperators(StrongList<String> * o) {
			for (UInt32 i = 0; i < o -> count(); i++) {
				if (checkOperator(o -> getNode(i))) {
					advance();
					return true;
				}
			} return false;
		}

		Boolean match(TokenType t) {
			if (check(t)) {
				advance();
				return true;
			} return false;
		}

		Boolean match(StrongList<TokenType> * t) {
			for (UInt32 i = 0; i < t -> count(); i++) {
				if (check(t -> getNode(i))) {
					advance();
					return true;
				}
			} return false;
		}

		Boolean checkOperator(String op) {
			if (isAtEnd()) return false;
			return peek().lexeme == op;
		}

		Boolean check(TokenType type) {
			if (isAtEnd()) return false;
			return peek().type == type;
		}

		Boolean isAtEnd() {
			if (tokens -> count() <= index) return true;
			if (tokens -> isEmpty()) return true;
			return peek().type == TokenType::endFile;
		}

		Token peek() {
			return tokens -> getNode(index);
		}

		Token previous() {
			return tokens -> getNode(index - 1);
		}

		Token advance() {
			if (!isAtEnd()) index++;
			return previous();
		}

		public:

		TreeParser(StrongList<Token> * t) {
			tokens = t;
		}

	};

}

#endif
