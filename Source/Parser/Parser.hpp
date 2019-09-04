
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
#include "../Interpreter/Converter.hpp"

#include "ASTree.hpp"

namespace Stack {

	class SyntaxError: public Exception {
		private:
		const String _message;
		const FilePosition _position;
		public:
		const FilePosition & getPosition() const { return _position; }
		const String & getMessage() const { return _message; }
		SyntaxError(String message, FilePosition position):
		Exception(), _message(message), _position(position) { }
	};

	class ParserErrorException: public Exception {
		private:
		const ArrayList<SyntaxError> * const _errors;
		const String _fileName;
		public:
		const ArrayList<SyntaxError> * const getErrors() const { return _errors; }
		const String & getFileName() const { return _fileName; }
		ParserErrorException(ArrayList<SyntaxError> * errors, String name):
		Exception(), _errors(errors), _fileName(name) { }
		~ParserErrorException() { delete _errors; }
	};

	class Parser {

		private:

		String * input = nullptr;

		ArrayList<Token> * tokens = nullptr;

		ArrayList<SyntaxError> * errors = new ArrayList<SyntaxError>();

		SizeType index = 0;

		/* Expressions */

		Expression * expression() {
			try { return assignment(); }
			catch (SyntaxError & s) { throw; }
		}

		Expression * assignment() {
			Expression * ex = nullptr;
			try { ex = shortOR(); }
			catch (SyntaxError & s) { throw; }
			if (match(TokenType::equal)) {
				Token * equals = new Token(previous());
				Expression * value = nullptr;
				try { value = assignment(); }
				catch (SyntaxError & s) { delete ex; delete equals; throw; }
				if (ex -> isInstanceOf<Identifier>()) {
					Token * name = new Token(* (((Identifier *)(ex)) -> name));
					delete equals;
					return new Assignment(name, value);
				}
				if (value) delete value;
				delete ex; delete equals;
				throw SyntaxError(
					"Expected identifier before assignment operator '='!",
					Linker::getPosition(input, equals -> position)
				);
			}
			return ex;
		}

		Expression * shortOR() {
			Expression * ex = nullptr;
			try { ex = shortAND(); }
			catch (SyntaxError & s) { throw; }
			while (match(TokenType::OR)) {
				Token * op = new Token(previous());
				Expression * right = nullptr;
				try { right = shortAND(); }
				catch (SyntaxError & s) { delete ex; delete op; throw; }
				ex = new Logical(ex, op, right);
			}
			return ex;
		}

		Expression * shortAND() {
			Expression * ex = nullptr;
			try { ex = equality(); }
			catch (SyntaxError & s) { throw; }
			while (match(TokenType::AND)) {
				Token * op = new Token(previous());
				Expression * right = nullptr;
				try { right = equality(); }
				catch (SyntaxError & s) { delete ex; delete op; throw; }
				ex = new Logical(ex, op, right);
			}
			return ex;
		}

		Expression * equality() {
			Expression * ex = nullptr;
			try { ex = comparison(); }
			catch (SyntaxError & s) { throw; }
			ArrayList<TokenType> * ops = new ArrayList<TokenType>();
			ops -> push(TokenType::equality);
			ops -> push(TokenType::inequality);
			while (match(ops)) {
				Token * op = new Token(previous());
				Expression * rs = nullptr;
				try { rs = comparison(); }
				catch (SyntaxError & s) {
					delete ops;
					delete ex;
					delete op;
					throw;
				}
				ex = new Comparison(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		Expression * comparison() {
			Expression * ex = nullptr;
			try { ex = lowPriorityOperator(); }
			catch (SyntaxError & s) { throw; }
			ArrayList<TokenType> * ops = new ArrayList<TokenType>();
			ops -> push(TokenType::major);
			ops -> push(TokenType::minor);
			ops -> push(TokenType::majorEqual);
			ops -> push(TokenType::minorEqual);
			while (match(ops)) {
				Token * op = new Token(previous());
				Expression * rs = nullptr;
				try { rs = lowPriorityOperator(); }
				catch (SyntaxError & s) {
					delete ops;
					delete ex;
					delete op;
					throw;
				}
				ex = new Comparison(ex, op, rs);
			}
			delete ops;
			return ex;
		}

		Expression * lowPriorityOperator() {
			Expression * ex = nullptr;
			try { ex = mediumPriorityOperator(); }
			catch (SyntaxError & e) { throw; }
			ArrayList<TokenType> * ops = new ArrayList<TokenType>();
			ops -> push(TokenType::plus);
			ops -> push(TokenType::minus);
			ops -> push(TokenType::pipe);
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

		Expression * mediumPriorityOperator() {
			Expression * ex = nullptr;
			try { ex = highPriorityOperator(); }
			catch (SyntaxError & e) { throw; }
			ArrayList<TokenType> * ops = new ArrayList<TokenType>();
			ops -> push(TokenType::star);
			ops -> push(TokenType::slash);
			ops -> push(TokenType::ampersand);
			ops -> push(TokenType::hat);
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

		Expression * primary() {
			Token t = peek();
			if (t.isTypeLiteral()) {
				advance();
				Token * literal = new Token(t);
				return new Literal(literal);
			} else if (match(TokenType::symbol)) {
				Token * name = new Token(previous());
				return new Identifier(name);
			} else if (match(TokenType::openRoundBracket)) {
				Expression * ex = nullptr;
				try {
					ex = expression();
					consume(TokenType::closeRoundBracket, ")");
				} catch (SyntaxError & e) {
					if (ex) delete ex;
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

		/* Statements */

		Statement * declaration() {
			Statement * st = nullptr;
			try {
				if (match(TokenType::basicType)) {
					st = variableDeclaration();
				} else st = statement();
			} catch (SyntaxError & s) { throw; }
			return st;
		}

		Statement * variableDeclaration() {
			Token * name = nullptr;
			Expression * initializer = nullptr;
			String stringType = previous().lexeme;
			try {
				name = new Token(consume(TokenType::symbol, "identifier"));
				if (match(TokenType::equal)) initializer = expression();
				consume(TokenType::semicolon, ";");
			} catch (SyntaxError & s) {
				if (name) delete name;
				if (initializer) delete initializer;
				throw;
			}
			BasicType type = Converter::typeFromString(stringType);
			return new VariableStatement(name, initializer, type);
		}

		Statement * statement() {
			Statement * st = nullptr;
			try {
				if (match(TokenType::ifKeyword)) st = ifStatement();
				else if (match(TokenType::printKeyword)) st = printStatement();
				else if (match(TokenType::whileKeyword)) st = whileStatement();
				else if (match(TokenType::openCurlyBracket)) st = blockStatement();
				else st = expressionStatement();
			} catch (SyntaxError & s) { throw; }
			return st;
		}

		Statement * ifStatement() {
			Token * t = nullptr;
			Expression * condition = nullptr;
			Statement * thenBranch = nullptr;
			Statement * elseBranch = nullptr;
			try {
				consume(TokenType::openRoundBracket, "(");
				t = new Token(peek());
				condition = expression();
				consume(TokenType::closeRoundBracket, ")");
				thenBranch = statement();
				elseBranch = nullptr;
				if (match(TokenType::elseKeyword)) {
					elseBranch = statement();
				}
			} catch (SyntaxError & s) {
				if (t) delete t;
				if (condition) delete condition;
				if (thenBranch) delete thenBranch;
				if (elseBranch) delete elseBranch;
				throw;
			}
			return new IfStatement(condition, thenBranch, elseBranch, t);
		}

		Statement * blockStatement() {
			ArrayList<Statement *> statements = ArrayList<Statement *>();
			try {
				while (!check(TokenType::closeCurlyBracket) && !isAtEnd()) {
					statements.push(declaration());
				}
				consume(TokenType::closeCurlyBracket, "}");
			} catch (SyntaxError & s) {
				for (Statement * statement : statements) {
					delete statement;
				}
				throw;
			}
			statements.shrinkToFit();
			return new BlockStatement(statements);
		}

		Statement * expressionStatement() {
			Expression * ex = nullptr;
			try {
				ex = expression();
				consume(TokenType::semicolon, ";");
			} catch (SyntaxError & s) {
				if (ex) delete ex;
				throw;
			}
			return new ExpressionStatement(ex);
		}

		Statement * printStatement() {
			Expression * ex = nullptr;
			try {
				ex = expression();
				consume(TokenType::semicolon, ";");
			} catch (SyntaxError & s) {
				if (ex) delete ex;
				throw;
			}
			return new PrintStatement(ex);
		}

		Statement * whileStatement() {
			Expression * condition = nullptr;
			Token * t = nullptr;
			Statement * body = nullptr;
			try {
				consume(TokenType::openRoundBracket, "(");
				t = new Token(peek());
				condition = expression();
				consume(TokenType::closeRoundBracket, ")");
				body = statement();
			} catch (SyntaxError & s) {
				if (t) delete t;
				if (condition) delete condition;
				if (body) delete body;
				throw;
			}
			return new WhileStatement(condition, body, t);
		}

		/* Core */

		Bool match(TokenType type) {
			if (check(type)) {
				advance();
				return true;
			} return false;
		}

		Bool match(ArrayList<TokenType> * types) {
			for (TokenType & type : * types) {
				if (check(type)) {
					advance();
					return true;
				}
			} return false;
		}

		Bool check(TokenType type) {
			if (isAtEnd()) return false;
			return peek().type == type;
		}

		Bool isOutOfRange() {
			if (tokens -> size() == 0) return true;
			if (index >= tokens -> size()) return true;
			return false;
		}

		Bool isAtEnd() {
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
				Token t = peek();
				if (t.type >= TokenType::tryKeyword &&
					t.type <= TokenType::nevermind) return;
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

		ArrayList<Statement *> * parse(ArrayList<Token> * tokens,
											   String * input = nullptr,
											   String fileName = "Unknown File") {
			if (!tokens || tokens -> size() <= 2) {
				errors -> push(SyntaxError("The code unit is empty!", { 0, 0 }));
				errors -> shrinkToFit();
				throw ParserErrorException(errors, fileName);
			}
			this -> tokens = tokens;
			this -> input = input;
			try {
				consume(TokenType::beginFile, "beginFile");
			} catch (SyntaxError & s) {
				errors -> push(s);
				errors -> shrinkToFit();
				throw ParserErrorException(errors, fileName);
			}
			ArrayList<Statement *> * statements = new ArrayList<Statement *>();
			while (!isAtEnd()) {
				try {
					statements -> push(declaration());
				} catch (SyntaxError & s) {
					errors -> push(s);
					synchronise();
				}
			}
			if (errors -> size() > 0) {
				errors -> shrinkToFit();
				throw ParserErrorException(errors, fileName);
			}
			delete errors;
			return statements;
		}

	};

}

#endif
