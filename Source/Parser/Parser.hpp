
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
#include "../Linker/Linker.hpp"

#include "ASTree.hpp"

namespace Stack {

	class SyntaxError: public Exception {
		private:
		String message = "";
		Linker::FilePosition pos = { 0, 0 };
		public:
		Linker::FilePosition getPosition() { return pos; }
		String getMessage() { return message; }
		SyntaxError(String m, Linker::FilePosition position):
		Exception(), pos(position) { }
	};

	class ParseErrorException: public Exception {
		private:
		ArrayList<SyntaxError> * errors = nullptr;
		String fileName = "";
		public:
		ArrayList<SyntaxError> * getErrors() { return errors; }
		String getFileName() { return fileName; }
		ParseErrorException(ArrayList<SyntaxError> * e, String f):
		Exception(), errors(e), fileName(f) { }
		~ParseErrorException() { delete errors; }
	};

	class Parser {

		private:

		String fileName = "";

		String * inputFile = nullptr;

		ArrayList<Token> * tokens = nullptr;

		ArrayList<SyntaxError> * errors = new ArrayList<SyntaxError>();

		SizeType index = 0;

		inline Expression * expression() {
			return equality();
		}

		Expression * equality() {
			Expression * ex = comparison();
			ArrayList<String> * ops = new ArrayList<String>();
			ops -> push("=="); ops -> push("!=");
			while (match(TokenType::infixOperator) &&
				   matchOperators(ops)) {
				Token * op = new Token(previous());
				Expression * rs = comparison();
				ex = new Binary(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		/*  Parses Low Priority infix Operators [N + M]. */
		Expression * lowPriorityOperator() {
			Expression * ex = mediumPriorityOperator();
			ArrayList<TokenType> * ops = new ArrayList<TokenType>();
			ops -> push(TokenType::plus);
			ops -> push(TokenType::minus);
			while (match(ops)) {
				Token * op = new Token(previous());
				Expression * rs = mediumPriorityOperator();
				ex = new Binary(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		/*  Parses Medium Priority infix Operators [N * M]. */
		Expression * mediumPriorityOperator() {
			Expression * ex = highPriorityOperator();
			ArrayList<TokenType> * ops = new ArrayList<TokenType>();
			ops -> push(TokenType::star);
			ops -> push(TokenType::slash);
			ops -> push(TokenType::modulus);
			while (match(ops)) {
				Token * op = new Token(previous());
				Expression * rs = highPriorityOperator();
				ex = new Binary(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		/*  Parses High Priority Unary Operators [-N]. */
		Expression * highPriorityOperator() {
			ArrayList<TokenType> * ops = new ArrayList<TokenType>();
			ops -> push(TokenType::minus);
			ops -> push(TokenType::plus);
			ops -> push(TokenType::exclamationMark);
			ops -> push(TokenType::tilde);
			if (match(ops)) {
				Token * op = new Token(previous());
				Expression * rs = highPriorityOperator();
				delete ops;
				return new Unary(op, rs);
			}
			delete ops;
			return primary();
		}

		/*  Parses Nested Expressions and Literals. */
		Expression * primary() {
			Token t = peek();
			if (t.isTypeLiteral()) {
				advance();
				Token * literal = new Token(t);
				return new Literal(literal);
			} else if (match(TokenType::openRoundBracket)) {
				Expression * ex = expression();
				try {
					consume(TokenType::closeRoundBracket, ")");
				} catch (SyntaxError & s) {
					errors -> push(s);
					if (ex != nullptr) delete ex;
					synchronise();
				}
				return new Grouping(ex);
			}
			return nullptr;
		}

		Expression * comparison() {
			Expression * ex = lowPriorityOperator();
			ArrayList<String> * ops = new ArrayList<String>();
			ops -> push(">"); ops -> push("<");
			ops -> push(">="); ops -> push(">=");
			while (match(TokenType::infixOperator) &&
				   matchOperators(ops)) {
				Token * op = new Token(previous());
				Expression * rs = lowPriorityOperator();
				ex = new Binary(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		Boolean matchOperator(String & op) {
			if (checkOperator(op)) {
				advance();
				return true;
			} return false;
		}

		Boolean matchOperators(ArrayList<String> * operators) {
			for (String & op : * operators) {
				if (checkOperator(op)) {
					advance();
					return true;
				}
			} return false;
		}

		Boolean match(TokenType type) {
			if (check(type)) {
				advance();
				return true;
			} return false;
		}

		Boolean match(ArrayList<TokenType> * types) {
			for (TokenType & type : * types) {
				if (check(type)) {
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

		Boolean isOutOfRange() {
			if (tokens -> size() == 0) return true;
			if (index >= tokens -> size()) return true;
			return false;
		}

		Boolean isAtEnd() {
			return peek().type == TokenType::endFile;
		}

		Token peek() { return tokens -> at(index); }

		Token previous() { return tokens -> at(index - 1); }

		Token advance() {
			if (!isAtEnd()) index += 1;
			return previous();
		}

		Token consume(TokenType type, String lexeme = "") {
			Token t = peek();
			if (check(type)) return advance();
			Linker::FilePosition fp = Linker::getPosition(inputFile, t.position);
			throw SyntaxError(
				(lexeme.length() > 0 ? "Expected '" + lexeme +
				"' but found '" + t.lexeme + "'!" :
				"Expecting a different token than '" + t.lexeme + "'!"), fp
			);
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

		Parser() = default;
		~Parser() = default;

		public:

		Parser(const Parser &) = delete;
		Parser(Parser &&) = delete;
		Parser & operator = (const Parser &) = delete;
		Parser & operator = (Parser &&) = delete;

		static Parser * self() {
			static Parser instance;
			return & instance;
		}

		Expression * parse(ArrayList<Token> * t,
						   String * i = nullptr,
						   String f = "Unknown File") {
			if (t == nullptr) return nullptr;
			if (t -> size() < 2) return nullptr;
			try {
				consume(TokenType::beginFile, "beginFile");
			} catch (SyntaxError & s) {
				errors -> push(s);
				errors -> shrinkToFit();
				throw ParseErrorException(errors, fileName);
			}
			tokens = t; inputFile = i; fileName = f;
			Expression * ex = expression();
			if (errors -> size() > 0) {
				errors -> shrinkToFit();
				throw ParseErrorException(errors, fileName);
			}
			delete errors;
			return ex;
		}

	};

}

#endif
