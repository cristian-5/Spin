
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
		const String _message;
		const FilePosition _position;
		public:
		const FilePosition & getPosition() const { return _position; }
		const String& getMessage() const { return _message; }
		SyntaxError(String message, FilePosition position):
		Exception(), _message(message), _position(position) { }
	};

	class ParseErrorException: public Exception {
		private:
		const ArrayList<SyntaxError> * const _errors;
		const String _fileName;
		public:
		const ArrayList<SyntaxError> * const getErrors() const { return _errors; }
		const String & getFileName() const { return _fileName; }
		ParseErrorException(ArrayList<SyntaxError> * errors, String name):
		Exception(), _errors(errors), _fileName(name) { }
		~ParseErrorException() { delete _errors; }
	};

	class Parser {

		private:

		String fileName = "";
		String * input = nullptr;

		ArrayList<Token> * tokens = nullptr;

		ArrayList<SyntaxError> * errors = new ArrayList<SyntaxError>();

		SizeType index = 0;

		Expression * expression() {
			try { return equality(); }
			catch (SyntaxError & s) { throw; }
		}

		Expression * equality() {
			Expression * ex = nullptr;
			try { ex = comparison(); }
			catch (SyntaxError & s) { throw; }
			ArrayList<String> * ops = new ArrayList<String>();
			ops -> push("=="); ops -> push("!=");
			while (match(TokenType::infixOperator) &&
				   matchOperators(ops)) {
				Token * op = new Token(previous());
				Expression * rs = nullptr;
				try { rs = comparison(); }
				catch (SyntaxError & s) {
					delete ops;
					delete ex;
					delete op;
					throw;
				}
				ex = new Binary(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		Expression * comparison() {
			Expression * ex = nullptr;
			try { ex = lowPriorityOperator(); }
			catch (SyntaxError & s) { throw; }
			ArrayList<String> * ops = new ArrayList<String>();
			ops -> push(">"); ops -> push("<");
			ops -> push(">="); ops -> push(">=");
			while (match(TokenType::infixOperator) &&
				   matchOperators(ops)) {
				Token * op = new Token(previous());
				Expression * rs = nullptr;
				try { rs = lowPriorityOperator(); }
				catch (SyntaxError & s) {
					delete ops;
					delete ex;
					delete op;
					throw;
				}
				ex = new Binary(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		/*  Parses Low Priority infix Operators [N + M]. */
		Expression * lowPriorityOperator() {
			Expression * ex = nullptr;
			try { ex = mediumPriorityOperator(); }
			catch (SyntaxError & e) { throw; }
			ArrayList<TokenType> * ops = new ArrayList<TokenType>();
			ops -> push(TokenType::plus);
			ops -> push(TokenType::minus);
			while (match(ops)) {
				Token * op = new Token(previous());
				Expression * rs = nullptr;
				try { rs = mediumPriorityOperator(); }
				catch (SyntaxError & e) {
					delete ops;
					delete ex;
					delete op;
					throw;
				}
				ex = new Binary(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		/*  Parses Medium Priority infix Operators [N * M]. */
		Expression * mediumPriorityOperator() {
			Expression * ex = nullptr;
			try { ex = highPriorityOperator(); }
			catch (SyntaxError & e) { throw; }
			ArrayList<TokenType> * ops = new ArrayList<TokenType>();
			ops -> push(TokenType::star);
			ops -> push(TokenType::slash);
			ops -> push(TokenType::modulus);
			while (match(ops)) {
				Token * op = new Token(previous());
				Expression * rs = nullptr;
				try { rs = highPriorityOperator(); }
				catch (SyntaxError & e) {
					delete ops;
					delete ex;
					delete op;
					throw;
				}
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
				Expression * rs = nullptr;
				try { rs = highPriorityOperator(); }
				catch (SyntaxError & e) {
					delete ops;
					delete op;
					throw;
				}
				return new Unary(op, rs);
			}
			delete ops;
			try { return primary(); }
			catch (SyntaxError & s) { throw; }
		}

		/*  Parses Nested Expressions and Literals. */
		Expression * primary() {
			Token t = peek();
			if (t.isTypeLiteral()) {
				advance();
				Token * literal = new Token(t);
				return new Literal(literal);
			} else if (match(TokenType::openRoundBracket)) {
				Expression * ex = nullptr;
				try {
					ex = expression();
					consume(TokenType::closeRoundBracket, ")");
				} catch (SyntaxError & e) {
					if (ex != nullptr) delete ex;
					throw;
				}
				return new Grouping(ex);
			}
			FilePosition fp = { 0, 0 };
			if (t.type == TokenType::endFile) {
				fp = Linker::getPosition(input, previous().position);
			} else fp = Linker::getPosition(input, t.position);
			throw SyntaxError(
				"Expected expression after '" + previous().lexeme + "'!", fp
			);
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
			FilePosition fp = { 0, 0 };
			if (t.type == TokenType::endFile) {
				fp = Linker::getPosition(input, previous().position);
			} else fp = Linker::getPosition(input, t.position);
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

		Expression * parse(ArrayList<Token> * tokens,
						   String * input = nullptr,
						   String fileName = "Unknown File") {
			if (tokens == nullptr) return nullptr;
			if (tokens -> size() < 2) return nullptr;
			this -> tokens = tokens;
			this -> input = input;
			this -> fileName = fileName;
			try {
				consume(TokenType::beginFile, "beginFile");
			} catch (SyntaxError & s) {
				errors -> push(s);
				errors -> shrinkToFit();
				throw ParseErrorException(errors, fileName);
			}
			Expression * ex = nullptr;
			try {
				ex = expression();
			} catch (SyntaxError & s) {
				errors -> push(s);
				errors -> shrinkToFit();
				throw ParseErrorException(errors, fileName);
			}
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
