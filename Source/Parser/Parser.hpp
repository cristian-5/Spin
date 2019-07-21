
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

#include "ASTree.hpp"
#include "Exceptions.hpp"

using namespace Collection;

namespace Stack {

	class Parser {

		private:

		String fileName = "";
		String * inputFile = nullptr;
		
		StrongList<Token> * tokens = nullptr;

		UInt32 index = 0;

		inline Expression * expression() {
			try {
				return equality();
			} catch (Exception & e) { throw; }
		}

		Expression * equality() {
			Expression * ex = nullptr;
			try {
				ex = comparison();
			} catch (Exception & e) { throw; }
			StrongList<String> * ops = new StrongList<String>();
			String s = "=="; ops -> link(s);
			s = "!="; ops -> link(s);
			while (match(TokenType::infixOperator)
				   && matchOperators(ops)) {
				Token * op = new Token();
				* op = previous();
				Expression * rs = nullptr;
				try {
					rs = comparison();
				} catch (Exception & e) { throw; }
				ex = new Binary(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		/*  Parses Low Priority infix Operators [N + M]. */
		Expression * lowPriorityOperator() {
			Expression * ex = nullptr;
			try {
				ex = mediumPriorityOperator();
			} catch (Exception & e) { throw; }
			StrongList<TokenType> * ops = new StrongList<TokenType>();
			TokenType t = TokenType::minus; ops -> link(t);
			t = TokenType::plus; ops -> link(t);
			while (match(ops)) {
				Token * op = new Token();
				* op = previous();
				Expression * rs = nullptr;
				try {
					rs = mediumPriorityOperator();
				} catch (Exception & e) { throw; }
				ex = new Binary(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		/*  Parses Medium Priority infix Operators [N * M]. */
		Expression * mediumPriorityOperator() {
			Expression * ex = nullptr;
			try {
				ex = highPriorityOperator();
			} catch (Exception & e) { throw; }
			StrongList<TokenType> * ops = new StrongList<TokenType>();
			TokenType t = TokenType::star; ops -> link(t);
			t = TokenType::slash; ops -> link(t);
			t = TokenType::modulus; ops -> link(t);
			while (match(ops)) {
				Token * op = new Token();
				* op = previous();
				Expression * rs = nullptr;
				try {
					rs = highPriorityOperator();
				} catch (Exception & e) { throw; }
				ex = new Binary(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		/*  Parses High Priority Unary Operators [-N]. */
		Expression * highPriorityOperator() {
			StrongList<TokenType> * ops = new StrongList<TokenType>();
			TokenType t = TokenType::minus; ops -> link(t);
			t = TokenType::exclamationMark; ops -> link(t);
			if (match(ops)) {
				Token * op = new Token();
				* op = previous();
				Expression * rs = highPriorityOperator();
				delete ops;
				return new Unary(op, rs);
			}
			delete ops;
			try {
				return primary();
			} catch(Exception & e) { throw; }
		}

		/*  Parses Nested Expressions and Literals. */
		Expression * primary() {
			Token * t = new Token();
			* t = peek();
			if (t -> isTypeLiteral()) {
				advance();
				return new Literal(t);
			} else if (match(TokenType::openRoundBracket)) {
				Expression * ex = expression();
				try {
					consume(TokenType::closeRoundBracket);
				} catch (Exception & e) { throw; }
				return new Grouping(ex);
			}
			FilePosition fp = getPosition(inputFile, t -> position);
			throw UnexpectedEndException(t -> lexeme, fp, fileName);
		}

		Expression * comparison() {
			Expression * ex = nullptr;
			try {
				ex = lowPriorityOperator();
			} catch (Exception & e) { throw; }
			StrongList<String> * ops = new StrongList<String>();
			String s = ">"; ops -> link(s);
			s = ">="; ops -> link(s);
			s = "<"; ops -> link(s);
			s = "<="; ops -> link(s);
			while (matchOperators(ops)) {
				Token * op = new Token();
				* op = previous();
				Expression * rs = nullptr;
				try {
					rs = lowPriorityOperator();
				} catch (Exception & e) { throw; }
				ex = new Binary(ex, op, rs);
			}
			delete ops;
			return ex;
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

		Token peek() { return tokens -> getNode(index); }

		Token previous() { return tokens -> getNode(index - 1); }

		Token advance() {
			if (!isAtEnd()) index++;
			return previous();
		}

		Token consume(TokenType type) {
			if (check(type)) return advance();
			Token t = peek();
			FilePosition fp = getPosition(inputFile, t.position);
			throw SyntaxErrorException(t.lexeme, ")", fp, fileName);
		}

		void synchronise() {
			advance();
			while (!isAtEnd()) {
				if (previous().type == TokenType::semicolon) {
					return;
				}
				switch (peek().type) {
					case TokenType::tryKeyword:
					case TokenType::catchKeyword:
					case TokenType::throwKeyword:
					case TokenType::throwsKeyword:
					case TokenType::avoidKeyword:
					case TokenType::ifKeyword:
					case TokenType::elseKeyword:
					case TokenType::switchKeyword:
					case TokenType::caseKeyword:
					case TokenType::defaultKeyword:
					case TokenType::whileKeyword:
					case TokenType::doKeyword:
					case TokenType::loopKeyword:
					case TokenType::forKeyword:
					case TokenType::repeatKeyword:
					case TokenType::untilKeyword:
					case TokenType::breakKeyword:
					case TokenType::continueKeyword:
					case TokenType::importKeyword:
					case TokenType::funcKeyword:
					case TokenType::procKeyword:
					case TokenType::staticKeyword:
					case TokenType::classKeyword:
					case TokenType::enumKeyword:
					case TokenType::structKeyword:
					case TokenType::exceptKeyword:
					case TokenType::privateKeyword:
					case TokenType::publicKeyword:
					case TokenType::refKeyword:
					case TokenType::cpyKeyword:
					case TokenType::constKeyword:
					case TokenType::returnKeyword: return;
					default: break;
				}
				advance();
			}
		}

		public:

		Parser() { }

		Expression * parse(StrongList<Token> * t,
						   String * i = nullptr,
						   String f = "Unknown File") {
			if (t == nullptr) return nullptr;
			if (t -> isEmpty()) throw EmptyUnitException(f);
			tokens = t; inputFile = i; fileName = f;
			try {
				return expression();
			} catch (Exception & e) { throw; }
		}

	};

}

#endif
