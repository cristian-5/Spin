
/*!
 *
 *    + --------------------------------------- +
 *    |  Parser.hpp                             |
 *    |                                         |
 *    |             Language Parser             |
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

#include "../Aliases/Prototypes.hpp"

#ifndef SPINPARSER
#define SPINPARSER

namespace Spin {

	Expression * Parser::expression() {
		try { return assignment(); }
		catch (SyntaxError & s) { throw; }
	}
	Expression * Parser::assignment() {
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
				return new Assignment(name, value, equals);
			}
			if (value) delete value;
			delete ex;
			SizeType position = equals -> position;
			delete equals;
			throw SyntaxError(
				"Expected identifier before assignment operator '='!",
				Linker::getPosition(currentUnit -> contents, position)
			);
		}
		return ex;
	}
	Expression * Parser::shortOR() {
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
	Expression * Parser::shortAND() {
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
	Expression * Parser::equality() {
		Expression * ex = nullptr;
		try { ex = comparison(); }
		catch (SyntaxError & s) { throw; }
		while (match(TokenType::equality) ||
			   match(TokenType::inequality)) {
			Token * op = new Token(previous());
			Expression * rs = nullptr;
			try { rs = comparison(); }
			catch (SyntaxError & s) {
				delete ex;
				delete op;
				throw;
			}
			ex = new Comparison(ex, op, rs);
		}
		return ex;
	}
	Expression * Parser::comparison() {
		Expression * ex = nullptr;
		try { ex = lowPriorityOperator(); }
		catch (SyntaxError & s) { throw; }
		while (match(TokenType::major) ||
			   match(TokenType::minor) ||
			   match(TokenType::majorEqual) ||
			   match(TokenType::minorEqual)) {
			Token * op = new Token(previous());
			Expression * rs = nullptr;
			try { rs = lowPriorityOperator(); }
			catch (SyntaxError & s) {
				delete ex;
				delete op;
				throw;
			}
			ex = new Comparison(ex, op, rs);
		}
		return ex;
	}
	Expression * Parser::lowPriorityOperator() {
		Expression * ex = nullptr;
		try { ex = mediumPriorityOperator(); }
		catch (SyntaxError & e) { throw; }
		while (matchRange(TokenType::plusEqual,
						  TokenType::pipe)) {
			Token * op = new Token(previous());
			Expression * rs = nullptr;
			try { rs = mediumPriorityOperator(); }
			catch (SyntaxError & e) {
				delete ex;
				delete op;
				throw;
			}
			if (op -> type >= TokenType::plusEqual &&
				op -> type <= TokenType::pipeEqual) {
				if (ex -> isInstanceOf<Identifier>()) {
					Token * name = new Token(* (((Identifier *)(ex)) -> name));
					ex = new Mutable(name, rs, op);
				} else {
					if (rs) delete rs;
					delete ex;
					String sign = op -> lexeme;
					SizeType position = op -> position;
					delete op;
					throw SyntaxError(
						"Expected identifier before mutable assignment operator '" + sign + "'!",
						Linker::getPosition(currentUnit -> contents, position)
					);
				}
			} else ex = new Binary(ex, op, rs);
		}
		return ex;
	}
	Expression * Parser::mediumPriorityOperator() {
		Expression * ex = nullptr;
		try { ex = highPriorityOperator(); }
		catch (SyntaxError & e) { throw; }
		while (matchRange(TokenType::starEqual,
						  TokenType::modulus)) {
			Token * op = new Token(previous());
			Expression * rs = nullptr;
			try { rs = highPriorityOperator(); }
			catch (SyntaxError & e) {
				delete ex;
				delete op;
				throw;
			}
			if (op -> type >= TokenType::starEqual &&
				op -> type <= TokenType::modulusEqual) {
				if (ex -> isInstanceOf<Identifier>()) {
					Token * name = new Token(* (((Identifier *)(ex)) -> name));
					ex = new Mutable(name, rs, op);
				} else {
					if (rs) delete rs;
					delete ex;
					String sign = op -> lexeme;
					SizeType position = op -> position;
					delete op;
					throw SyntaxError(
						"Expected identifier before mutable assignment operator '" + sign + "'!",
						Linker::getPosition(currentUnit -> contents, position)
					);
				}
			} else ex = new Binary(ex, op, rs);
		}
		return ex;
	}
	Expression * Parser::highPriorityOperator() {
		if (match(TokenType::minus) ||
			match(TokenType::plus)  ||
			match(TokenType::tilde) ||
			match(TokenType::exclamationMark)) {
			Token * op = new Token(previous());
			Expression * rs = nullptr;
			try { rs = highPriorityOperator(); }
			catch (SyntaxError & e) {
				delete op;
				throw;
			}
			return new Unary(op, rs);
		}
		try { return subscription(); }
		catch (SyntaxError & s) { throw; }
	}
	Expression * Parser::subscription() {
		Expression * ex = nullptr;
		try { ex = call(); }
		catch (SyntaxError & s) { throw; }
		while (true) {
			if (match(TokenType::openBracket)) {
				try { ex = completeSubscript(ex); }
				catch (SyntaxError & s) { throw; }
			} else break;
		}
		return ex;
	}
	Expression * Parser::completeSubscript(Expression * item) {
		Token * bracket = new Token(previous());
		Expression * ex = nullptr;
		if (!check(TokenType::closeBracket)) {
			try { ex = expression(); }
			catch (SyntaxError & s) { throw; }
		}
		try { consume(TokenType::closeBracket, "]"); }
		catch (SyntaxError & s) { if (ex) delete ex; throw; }
		return new Subscript(item, bracket, ex);
	}
	Expression * Parser::call() {
		Expression * ex = nullptr;
		try { ex = primary(); }
		catch (SyntaxError & s) { throw; }
		while (true) {
			if (match(TokenType::openParenthesis)) {
				try { ex = completeCall(ex); }
				catch (SyntaxError & s) { throw; }
			} else break;
		}
		return ex;
	}
	Expression * Parser::completeCall(Expression * callee) {
		Token * parenthesis = new Token(previous());
		Array<Expression *> * arguments = new Array<Expression *>();
		if (!check(TokenType::closeParenthesis)) {
			do {
				Expression * ex = nullptr;
				try { ex = expression(); }
				catch (SyntaxError & s) { throw; }
				arguments -> push(ex);
			} while (match(TokenType::comma));
		}
		try { consume(TokenType::closeParenthesis, ")"); }
		catch (SyntaxError & s) {
			for (Expression * arg : * arguments) delete arg;
			delete arguments;
			throw;
		}
		arguments -> shrinkToFit();
		return new Call(callee, parenthesis, arguments);
	}
	Expression * Parser::primary() {
		Token t = peek();
		if (t.isTypeLiteral()) {
			advance();
			Token * literal = new Token(t);
			return new Literal(literal);
		} else if (t.type == TokenType::symbol) {
			advance();
			Token * name = new Token(t);
			return new Identifier(name);
		} else if (t.type == TokenType::openParenthesis) {
			advance();
			Expression * ex = nullptr;
			try {
				ex = expression();
				consume(TokenType::closeParenthesis, ")");
			} catch (SyntaxError & e) {
				if (ex) delete ex;
				throw;
			}
			return new Grouping(ex);
		} else if (t.type == TokenType::openBracket) {
			advance();
			Array<Expression *> * values = new Array<Expression *>();
			Expression * ex = nullptr;
			if (!check(TokenType::closeBracket)) {
				do {
					try {
						ex = expression();
						values -> push(ex);
					} catch (SyntaxError & e) {
						if (ex) delete ex;
						for (Expression * expression : * values) {
							delete expression;
						}
						delete values;
						throw;
					}
				} while (match(TokenType::comma));
			}
			try { consume(TokenType::closeBracket, "]"); }
			catch (SyntaxError & e) {
				if (ex) delete ex;
				for (Expression * expression : * values) {
					delete expression;
				}
				delete values;
				throw;
			}
			values -> shrinkToFit();
			return new List(values);
		}
		FilePosition fp = { 0, 0 };
		if (t.type == TokenType::endFile) {
			fp = Linker::getPosition(currentUnit -> contents, previous().position);
		} else fp = Linker::getPosition(currentUnit -> contents, t.position);
		throw SyntaxError(
			"Expected expression after '" + previous().lexeme + "'!", fp
		);
	}

	/* Statements */

	String * Parser::typeString() {
		if (match(TokenType::basicType) ||
			match(TokenType::customType)) {
			return new String(previous().lexeme);
		}
		return nullptr;
	}
	Statement * Parser::declaration() {
		Statement * st = nullptr;
		String * type = nullptr;
		try {
			type = typeString();
			if (type) {
				st = variableDeclaration(* type);
				delete type; type = nullptr;
			} else st = statement();
		} catch (SyntaxError & s) {
			if (type) delete type;
			throw;
		}
		if (type) delete type;
		return st;
	}
	Statement * Parser::variableDeclaration(String stringType) {
		if (stringType == "Vector") {
			try { return vectorDeclaration(); }
			catch (SyntaxError & s) { throw; }
		}
		Token * name = nullptr;
		Expression * initializer = nullptr;
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
	Statement * Parser::vectorDeclaration() {
		Token * name = nullptr;
		Expression * initializer = nullptr;
		try {
			if (match(TokenType::braSymbol) ||
				match(TokenType::ketSymbol)) {
				name = new Token(previous());
			} else {
				Token t = previous();
				FilePosition fp = Linker::getPosition(currentUnit -> contents, t.position);
				throw SyntaxError(
					"Expected <identifier| or |identifier> in bra-ket notation but found '"
					+ t.lexeme + "'!", fp
				);
			}
			if (match(TokenType::equal)) initializer = expression();
			consume(TokenType::semicolon, ";");
		} catch (SyntaxError & s) {
			if (name) delete name;
			if (initializer) delete initializer;
			throw;
		}
		return new VariableStatement(name, initializer, BasicType::VectorType);
	}
	Statement * Parser::statement() {
		Statement * st = nullptr;
		try {
			Token keyword = peek();
			switch (keyword.type) {
				case TokenType::classKeyword: st = classDeclaration(); break;
				case TokenType::funcKeyword: st = functionStatement(); break;
				case TokenType::procKeyword: st = procedureStatement(); break;
				case TokenType::ifKeyword: st = ifStatement(); break;
				case TokenType::printKeyword: st = printStatement(); break;
				case TokenType::whileKeyword: st = whileStatement(); break;
				case TokenType::doKeyword: st = doWhileStatement(); break;
				case TokenType::untilKeyword: st = untilStatement(); break;
				case TokenType::repeatKeyword: st = repeatUntilStatement(); break;
				case TokenType::loopKeyword: st = loopStatement(); break;
				case TokenType::forKeyword: st = forStatement(); break;
				case TokenType::openBrace: st = blockStatement(); break;
				case TokenType::breakKeyword: st = breakStatement(); break;
				case TokenType::continueKeyword: st = continueStatement(); break;
				case TokenType::restKeyword: st = restStatement(); break;
				case TokenType::returnKeyword: st = returnStatement(); break;
				case TokenType::deleteKeyword: st = deleteStatement(); break;
				default: st = expressionStatement(); break;
			}
		} catch (SyntaxError & s) { throw; }
		return st;
	}
	Statement * Parser::ifStatement() {
		Token * ifToken = new Token(peekAdvance());
		Expression * condition = nullptr;
		Statement * thenBranch = nullptr;
		Statement * elseBranch = nullptr;
		try {
			consume(TokenType::openParenthesis, "(");
			condition = expression();
			consume(TokenType::closeParenthesis, ")");
			thenBranch = statement();
			elseBranch = nullptr;
			if (match(TokenType::elseKeyword)) {
				elseBranch = statement();
			}
		} catch (SyntaxError & s) {
			if (ifToken) delete ifToken;
			if (condition) delete condition;
			if (thenBranch) delete thenBranch;
			if (elseBranch) delete elseBranch;
			throw;
		}
		return new IfStatement(condition, thenBranch, elseBranch, ifToken);
	}
	Statement * Parser::blockStatement() {
		advance();
		Array<Statement *> * statements = new Array<Statement *>();
		try {
			while (!check(TokenType::closeBrace) && !isAtEnd()) {
				statements -> push(declaration());
			}
			consume(TokenType::closeBrace, "}");
		} catch (SyntaxError & s) {
			for (Statement * statement : * statements) {
				delete statement;
			}
			delete statements;
			throw;
		}
		statements -> shrinkToFit();
		return new BlockStatement(statements);
	}
	Statement * Parser::breakStatement() {
		if (!isInControlFlow) {
			throw SyntaxError(
				"Unexpected break statement outside of control flow statements! What am I supposed to break?",
				Linker::getPosition(currentUnit -> contents, peek().position)
			);
		}
		Token * breakToken = new Token(peekAdvance());
		try {
			consume(TokenType::semicolon, ";");
		} catch (SyntaxError & s) {
			delete breakToken;
			throw;
		}
		return new BreakStatement(breakToken);
	}
	Statement * Parser::continueStatement() {
		if (!isInControlFlow) {
			throw SyntaxError(
				"Unexpected continue statement outside of control flow statements! Where am I supposed to continue?",
				Linker::getPosition(currentUnit -> contents, peek().position)
			);
		}
		Token * continueToken = new Token(peekAdvance());
		try {
			consume(TokenType::semicolon, ";");
		} catch (SyntaxError & s) {
			delete continueToken;
			throw;
		}
		return new ContinueStatement(continueToken);
	}
	Statement * Parser::expressionStatement() {
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
	Statement * Parser::functionStatement() {
		advance();
		Bool oldControlFlow = isInControlFlow;
		isInControlFlow = false;
		Bool oldProcedure = isInProcedure;
		isInProcedure = false;
		Bool oldFunction = isInFunction;
		isInFunction = true;
		Token * name = nullptr;
		String * stringType = nullptr;
		Array<Parameter *> * params = new Array<Parameter *>();
		Parameter * returnType = new Parameter();
		BlockStatement * body = nullptr;
		try {
			name = new Token(consume(TokenType::symbol, "identifier"));
			consume(TokenType::openParenthesis, "(");
			if (!check(TokenType::closeParenthesis)) {
				do {
					Parameter * p = new Parameter();
					p -> name = new Token(consume(TokenType::symbol, "identifier"));
					consume(TokenType::colon, ":");
					if (match(TokenType::refKeyword)) p -> reference = true;
					else if (match(TokenType::cpyKeyword)) p -> reference = false;
					stringType = typeString();
					if (!stringType) {
						Token er = peek();
						throw SyntaxError(
							"Expected type but found '" + er.lexeme + "'!",
							Linker::getPosition(currentUnit -> contents, er.position)
						);
					}
					p -> type = Converter::typeFromString(* stringType);
					delete stringType; stringType = nullptr;
					p -> tokenType = new Token(previous());
					params -> push(p);
				} while (match(TokenType::comma));
			}
			consume(TokenType::closeParenthesis, ")");
			params -> shrinkToFit();
			consume(TokenType::arrow, "arrow operator ->");
			stringType = typeString();
			if (!stringType) {
				Token er = peek();
				throw SyntaxError(
					"Expected type but found '" + er.lexeme + "'!",
					Linker::getPosition(currentUnit -> contents, er.position)
				);
			}
			returnType -> type = Converter::typeFromString(* stringType);
			delete stringType; stringType = nullptr;
			returnType -> tokenType = new Token(previous());
			if (!check(TokenType::openBrace)) {
				Token er = peek();
				throw SyntaxError(
					"Expected function body but found '" + er.lexeme + "'!",
					Linker::getPosition(currentUnit -> contents, er.position)
				);
			}
			body = (BlockStatement *)blockStatement();
		} catch (SyntaxError & s) {
			if (name) delete name;
			if (returnType) delete returnType;
			if (stringType) delete stringType;
			for (Parameter * p : * params) delete p;
			delete params;
			throw;
		}
		isInControlFlow = oldControlFlow;
		isInProcedure = oldProcedure;
		isInFunction = oldFunction;
		return new FunctionStatement(name, params, body, returnType);
	}
	Statement * Parser::procedureStatement() {
		advance();
		Bool oldControlFlow = isInControlFlow;
		isInControlFlow = false;
		Bool oldFunction = isInFunction;
		isInFunction = false;
		Bool oldProcedure = isInProcedure;
		isInProcedure = true;
		Token * name = nullptr;
		String * stringType = nullptr;
		Array<Parameter *> * params = new Array<Parameter *>();
		BlockStatement * body = nullptr;
		try {
			name = new Token(consume(TokenType::symbol, "identifier"));
			consume(TokenType::openParenthesis, "(");
			if (!check(TokenType::closeParenthesis)) {
				do {
					Parameter * p = new Parameter();
					p -> name = new Token(consume(TokenType::symbol, "identifier"));
					consume(TokenType::colon, ":");
					if (match(TokenType::refKeyword)) p -> reference = true;
					else if (match(TokenType::cpyKeyword)) p -> reference = false;
					stringType = typeString();
					if (!stringType) {
						Token er = peek();
						throw SyntaxError(
							"Expected type but found '" + er.lexeme + "'!",
							Linker::getPosition(currentUnit -> contents, er.position)
						);
					}
					p -> type = Converter::typeFromString(* stringType);
					delete stringType; stringType = nullptr;
					p -> tokenType = new Token(previous());
					params -> push(p);
				} while (match(TokenType::comma));
			}
			consume(TokenType::closeParenthesis, ")");
			params -> shrinkToFit();
			if (!check(TokenType::openBrace)) {
				Token er = peek();
				throw SyntaxError(
					"Expected function body but found '" + er.lexeme + "'!",
					Linker::getPosition(currentUnit -> contents, er.position)
				);
			}
			body = (BlockStatement *)blockStatement();
		} catch (SyntaxError & s) {
			if (name) delete name;
			if (stringType) delete stringType;
			for (Parameter * p : * params) delete p;
			delete params;
			throw;
		}
		isInControlFlow = oldControlFlow;
		isInFunction = oldFunction;
		isInProcedure = oldProcedure;
		return new ProcedureStatement(name, params, body);
	}
	Statement * Parser::classDeclaration() {
		advance();
		Bool oldControlFlow = isInControlFlow;
		isInControlFlow = false;
		Bool oldProcedure = isInProcedure;
		isInProcedure = false;
		Bool oldFunction = isInFunction;
		isInFunction = false;

		

		


		isInControlFlow = oldControlFlow;
		isInProcedure = oldProcedure;
		isInFunction = oldFunction;
		return nullptr;
	}
	Statement * Parser::forStatement() {
		Bool oldControlFlow = isInControlFlow;
		isInControlFlow = true;
		Token * forToken = new Token(peekAdvance());
		Statement * declaration = nullptr;
		Expression * condition = nullptr;
		Expression * stepper = nullptr;
		Statement * body = nullptr;
		String * type = nullptr;
		try {
			consume(TokenType::openParenthesis, "(");
			type = typeString();
			if (type) {
				declaration = variableDeclaration(* type);
				delete type; type = nullptr;
			} else declaration = expressionStatement();
			condition = expression();
			consume(TokenType::semicolon, ";");
			stepper = expression();
			consume(TokenType::closeParenthesis, ")");
			body = statement();
		} catch (SyntaxError & s) {
			if (type) delete type;
			if (forToken) delete forToken;
			if (declaration) delete declaration;
			if (condition) delete condition;
			if (stepper) delete stepper;
			if (body) delete body;
			throw;
		}
		isInControlFlow = oldControlFlow;
		return new BlockStatement(
			new ForStatement(declaration, condition,
								stepper, body, forToken)
		);
	}
	Statement * Parser::printStatement() {
		advance();
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
	Statement * Parser::whileStatement() {
		Bool oldControlFlow = isInControlFlow;
		isInControlFlow = true;
		Token * whileToken = new Token(peekAdvance());
		Expression * condition = nullptr;
		Statement * body = nullptr;
		try {
			consume(TokenType::openParenthesis, "(");
			condition = expression();
			consume(TokenType::closeParenthesis, ")");
			body = statement();
		} catch (SyntaxError & s) {
			if (whileToken) delete whileToken;
			if (condition) delete condition;
			if (body) delete body;
			throw;
		}
		isInControlFlow = oldControlFlow;
		return new WhileStatement(condition, body, whileToken);
	}
	Statement * Parser::doWhileStatement() {
		Bool oldControlFlow = isInControlFlow;
		isInControlFlow = true;
		advance();
		Statement * body = nullptr;
		Expression * condition = nullptr;
		Token * whileToken = nullptr;
		try {
			body = statement();
			whileToken = new Token(
				consume(TokenType::whileKeyword, "while")
			);
			consume(TokenType::openParenthesis, "(");
			condition = expression();
			consume(TokenType::closeParenthesis, ")");
			consume(TokenType::semicolon, ";");
		} catch (SyntaxError & s) {
			if (whileToken) delete whileToken;
			if (condition) delete condition;
			if (body) delete body;
			throw;
		}
		isInControlFlow = oldControlFlow;
		return new DoWhileStatement(body, condition, whileToken);
	}
	Statement * Parser::untilStatement() {
		Bool oldControlFlow = isInControlFlow;
		isInControlFlow = true;
		Token * untilToken = new Token(peekAdvance());
		Expression * condition = nullptr;
		Statement * body = nullptr;
		try {
			consume(TokenType::openParenthesis, "(");
			condition = expression();
			consume(TokenType::closeParenthesis, ")");
			body = statement();
		} catch (SyntaxError & s) {
			if (untilToken) delete untilToken;
			if (condition) delete condition;
			if (body) delete body;
			throw;
		}
		isInControlFlow = oldControlFlow;
		return new UntilStatement(condition, body, untilToken);
	}
	Statement * Parser::repeatUntilStatement() {
		Bool oldControlFlow = isInControlFlow;
		isInControlFlow = true;
		advance();
		Statement * body = nullptr;
		Expression * condition = nullptr;
		Token * untilToken = nullptr;
		try {
			body = statement();
			untilToken = new Token(
				consume(TokenType::untilKeyword, "until")
			);
			consume(TokenType::openParenthesis, "(");
			condition = expression();
			consume(TokenType::closeParenthesis, ")");
			consume(TokenType::semicolon, ";");
		} catch (SyntaxError & s) {
			if (untilToken) delete untilToken;
			if (condition) delete condition;
			if (body) delete body;
			throw;
		}
		isInControlFlow = oldControlFlow;
		return new RepeatUntilStatement(body, condition, untilToken);
	}
	Statement * Parser::loopStatement() {
		Bool oldControlFlow = isInControlFlow;
		isInControlFlow = true;
		Token * loopToken = new Token(peekAdvance());
		Statement * body = nullptr;
		try {
			body = statement();
		} catch (SyntaxError & s) {
			if (loopToken) delete loopToken;
			if (body) delete body;
			throw;
		}
		isInControlFlow = oldControlFlow;
		return new LoopStatement(body, loopToken);
	}
	Statement * Parser::restStatement() {
		advance();
		try {
			consume(TokenType::semicolon, ";");
		} catch (SyntaxError & s) { throw; }
		return new RestStatement();
	}
	Statement * Parser::returnStatement() {
		Token * returnToken = new Token(peekAdvance());
		Expression * ex = nullptr;
		try {
			if (!match(TokenType::semicolon)) {
				ex = expression();
				consume(TokenType::semicolon, ";");
			}
		} catch (SyntaxError & s) {
			if (returnToken) delete returnToken;
			throw;
		}
		if (ex) {
			if (!isInFunction) {
				UInt32 position = returnToken -> position;
				if (returnToken) delete returnToken;
				throw SyntaxError(
					"Unexpected return statement outside of function! Where am I supposed to return to?",
					Linker::getPosition(currentUnit -> contents, position)
				);
			}
		} else if (!isInProcedure) {
			UInt32 position = returnToken -> position;
			if (returnToken) delete returnToken;
			throw SyntaxError(
				"Unexpected return statement outside of procedure! Where am I supposed to return to?",
				Linker::getPosition(currentUnit -> contents, position)
			);
		}
		return new ReturnStatement(ex, returnToken);
	}
	Statement * Parser::deleteStatement() {
		advance();
		try {
			Token name = consume(TokenType::symbol, "identifier");
			consume(TokenType::semicolon, ";");
			return new DeleteStatement(new Token(name));
		} catch (SyntaxError & s) { throw; }
		throw SyntaxError(
			"Unexpected delete statement found outside of valid context!",
			Linker::getPosition(currentUnit -> contents, previous().position)
		);
	}
	Statement * Parser::fileStatement() {
		return new FileStatement(
			currentUnit -> name,
			currentUnit -> contents
		);
	}

	void Parser::replace(TokenType type, String lexeme, TokenType newType) {
		for (Token & t : * tokens) {
			if (t.type == type && t.lexeme == lexeme) t.type = newType;
		}
	}
	void Parser::runTypeClassification() {
		if (!tokens) return;
		SizeType tokenCount = (tokens -> size()) - 1;
		for (SizeType i = 0; i < tokenCount; i++) {
			if (tokens -> at(i).isTypeType()) {
				i += 1;
				Token token = tokens -> at(i);
				if (token.type == TokenType::symbol) {
					replace(TokenType::symbol, token.lexeme, TokenType::customType);
				}
			}
		}
	}
	String Parser::parseImport(SizeType & i) {
		tokens -> at(i).type = TokenType::empty;
		i += 1;
		String import = "";
		while (i < tokens -> size()) {
			if (tokens -> at(i).type == TokenType::symbol) {
				import += tokens -> at(i).lexeme;
				tokens -> at(i).type = TokenType::empty;
			} else throw SyntaxError(
				"Expected 'identifier' but found '" +
				tokens -> at(i).lexeme + "'!",
				Linker::getPosition(currentUnit -> contents, tokens -> at(i).position)
			);
			i += 1;
			if (i < tokens -> size()) {
				switch (tokens -> at(i).type) {
					case TokenType::semicolon: {
						tokens -> at(i).type = TokenType::empty;
						return import;
					}
					case TokenType::dot: {
						tokens -> at(i).type = TokenType::empty;
						import += ".";
						i += 1;
						break;
					}
					default: throw SyntaxError(
						"Expected ';' but found '" +
						tokens -> at(i).lexeme + "'!",
						Linker::getPosition(currentUnit -> contents, tokens -> at(i).position)
					);
				}
			} else break;
		}
		throw SyntaxError(
			"Expected ';' but found '" +
			tokens -> at(i).lexeme + "'!",
			Linker::getPosition(currentUnit -> contents, tokens -> at(i).position)
		);
	}
	SyntaxTree * Parser::runImportClassification(SyntaxTree * syntaxTree) {
		if (!syntaxTree) syntaxTree = new SyntaxTree();
		if (!tokens) return nullptr;
		SizeType tokenCount = tokens -> size();
		for (SizeType i = 0; i < tokenCount; i++) {
			if (tokens -> at(i).type == TokenType::importKeyword) {
				try {
					SizeType store = i;
					String s = parseImport(i);
					if (s == "Standard") syntaxTree -> standardLibrary = true;
					else if (s == "Maths") syntaxTree -> mathsLibrary = true;
					else if (s == "Kronos") syntaxTree -> kronosLibrary = true;
					else throw SyntaxError(
						"Invalid import statement asks for unknown library '" + s + "'!",
						Linker::getPosition(currentUnit -> contents, tokens -> at(store).position)
					);
				} catch (SyntaxError & r) { throw; }
			}
		}
		return syntaxTree;
	}
	void Parser::runCastClassification() {
		if (!tokens) return;
		SizeType tokenCount = (tokens -> size()) - 1;
		for (SizeType i = 0; i < tokenCount; i++) {
			if (tokens -> at(i).isTypeBasicType()) {
				i += 1;
				Token token = tokens -> at(i);
				if (token.type == TokenType::openParenthesis) {
					tokens -> at(i - 1).type = TokenType::symbol;
				}
			}
		}
	}
	void Parser::runVectorClassification() {
		if (!tokens) return;
		SizeType tokenCount = tokens -> size() - 1;
		for (SizeType i = 0; i < tokenCount; i++) {
			Token token = tokens -> at(i);
			switch (token.type) {
				case TokenType::closeParenthesis: {
					Token next = tokens -> at(i + 1);
					if (next.type == TokenType::openParenthesis ||
						next.type == TokenType::ketSymbol ||
						next.isTypeNumeral()) {
						i = i + 1;
						SizeType pos = token.position + token.lexeme.length() - 1;
						//tokens -> insert(i, Token("*", TokenType::virtualOperator, pos));
					}

				} break;
				case TokenType::braSymbol: {

				} break;
				case TokenType::ketSymbol: {

				} break;
				case TokenType::braketSymbol: {

				} break;
				default: break;
			}
		}

	} 
	void Parser::cleanEmptyTokens() {
		Array<Token> * newTokens = new Array<Token>();
		for (Token token : * tokens) {
			if (token.type != TokenType::empty) {
				newTokens -> push (token);
			}
		}
		delete tokens;
		newTokens -> shrinkToFit();
		tokens = newTokens;
	}

	/* Core */

	inline Bool Parser::match(TokenType type) {
		if (check(type)) {
			advance();
			return true;
		} return false;
	}
	inline Bool Parser::matchRange(TokenType from, TokenType to) {
		if (isAtEnd()) return false;
		TokenType current = peek().type;
		if (current >= from && current <= to) {
			advance();
			return true;
		} return false;
	}
	inline Bool Parser::check(TokenType type) {
		if (isAtEnd()) return false;
		return peek().type == type;
	}
	inline Bool Parser::isOutOfRange() {
		if (tokens -> size() == 0) return true;
		if (index >= tokens -> size()) return true;
		return false;
	}
	inline Bool Parser::isAtEnd() {
		return peek().type == TokenType::endFile;
	}
	inline Token Parser::peek() { return tokens -> at(index); }
	inline Token Parser::peekAdvance() {
		Token peekToken = peek();
		advance();
		return peekToken;
	}
	inline Token Parser::previous() { return tokens -> at(index - 1); }
	inline Token Parser::advance() {
		if (!isAtEnd()) index += 1;
		return previous();
	}
	inline Token Parser::consume(TokenType type, String lexeme) {
		Token t = peek();
		if (check(type)) return advance();
		FilePosition fp = { 0, 0 };
		if (t.type == TokenType::endFile) {
			fp = Linker::getPosition(currentUnit -> contents, previous().position);
		} else fp = Linker::getPosition(currentUnit -> contents, t.position);
		throw SyntaxError(
			(lexeme.length() > 0 ? "Expected '" + lexeme +
			"' but found '" + t.lexeme + "'!" :
			"Expecting a different token than '" + t.lexeme + "'!"), fp
		);
	}

	inline void Parser::resetState() {
		index = 0;
		isInControlFlow = false;
		isInFunction = false;
		isInProcedure = false;
	}

	void Parser::synchronise() {
		advance();
		while (!isAtEnd()) {
			if (previous().type == TokenType::semicolon) {
				return;
			}
			Token t = peek();
			if (t.type >= TokenType::ifKeyword &&
				t.type <= TokenType::deleteKeyword) return;
			advance();
		}
	}

	SyntaxTree * Parser::parse(Array<CodeUnit *> * units) {
		if (!units) return nullptr;
		SyntaxTree * syntaxTree = nullptr;
		if (!errors) errors = new Array<SyntaxError>();
		Array<UnitError *> * syntaxErrors = new Array<UnitError *>();
		for (CodeUnit * unit : * units) {
			if (!(unit -> contents)) return nullptr;
			if (!(unit -> tokens)) return nullptr;
			if (!(unit -> name)) return nullptr;
			if (unit -> tokens -> size() <= 2) {
				errors -> push(SyntaxError("The code unit is empty!", { 0, 0 }));
				errors -> shrinkToFit();
				syntaxErrors -> push(new UnitError(errors, * unit -> name));
				errors = new Array<SyntaxError>(); resetState(); continue;
			}
			tokens = new Array<Token>(* unit -> tokens);
			currentUnit = unit;
			try { consume(TokenType::beginFile, "beginFile"); }
			catch (SyntaxError & s) {
				errors -> push(s);
				errors -> shrinkToFit();
				syntaxErrors -> push(new UnitError(errors, * unit -> name));
				errors = new Array<SyntaxError>();
				delete tokens; resetState(); continue;
			}
			runCastClassification();
			runTypeClassification();
			try { syntaxTree = runImportClassification(syntaxTree); }
			catch (SyntaxError & s) {
				if (syntaxTree) {
					delete syntaxTree;
					syntaxTree = nullptr;
				}
				errors -> push(s);
				errors -> shrinkToFit();
				syntaxErrors -> push(new UnitError(errors, * unit -> name));
				errors = new Array<SyntaxError>();
				delete tokens; resetState(); continue;
			}
			cleanEmptyTokens();
			if (!(syntaxTree -> statements)) {
				syntaxTree -> statements = new Array<Statement *>();
			}
			syntaxTree -> statements -> push(fileStatement());
			while (!isAtEnd()) {
				try {
					syntaxTree -> statements -> push(declaration());
				} catch (SyntaxError & s) {
					errors -> push(s);
					synchronise();
				}
			}
			if (errors -> size() > 0) {
				if (syntaxTree) {
					delete syntaxTree;
					syntaxTree = nullptr;
				}
				syntaxErrors -> push(new UnitError(errors, * unit -> name));
				errors = new Array<SyntaxError>();
				delete tokens; resetState(); continue;
			}
			delete tokens; resetState();
		}
		delete errors; errors = nullptr;
		if (syntaxErrors -> size() > 0) {
			if (syntaxTree) delete syntaxTree;
			resetState();
			throw ParserErrorException(syntaxErrors);
		}
		delete syntaxErrors;
		resetState();
		return syntaxTree;
	}
	SyntaxTree * Parser::parse(CodeUnit * unit) {
		if (!unit) return nullptr;
		if (!(unit -> contents)) return nullptr;
		if (!(unit -> tokens)) return nullptr;
		if (!(unit -> name)) return nullptr;
		SyntaxTree * syntaxTree = nullptr;
		if (!errors) errors = new Array<SyntaxError>();
		if (unit -> tokens -> size() <= 2) {
			errors -> push(SyntaxError("The code unit is empty!", { 0, 0 }));
			errors -> shrinkToFit();
			UnitError * syntaxErrors = new UnitError(
				errors, * unit -> name
			);
			resetState();
			throw ParserErrorException(syntaxErrors);
		}
		tokens = new Array<Token>(* unit -> tokens);
		currentUnit = unit;
		try { consume(TokenType::beginFile, "beginFile"); }
		catch (SyntaxError & s) {
			errors -> push(s);
			errors -> shrinkToFit();
			UnitError * syntaxErrors = new UnitError(
				errors, * unit -> name
			);
			delete tokens; resetState();
			throw ParserErrorException(syntaxErrors);
		}
		runCastClassification();
		runTypeClassification();
		try { syntaxTree = runImportClassification(syntaxTree); }
		catch (SyntaxError & s) {
			errors -> push(s);
			errors -> shrinkToFit();
			UnitError * syntaxErrors = new UnitError(
				errors, * unit -> name
			);
			delete tokens; resetState();
			throw ParserErrorException(syntaxErrors);
		}
		cleanEmptyTokens();
		Array<Statement *> * statements = new Array<Statement *>();
		statements -> push(fileStatement());
		while (!isAtEnd()) {
			try {
				statements -> push(declaration());
			} catch (SyntaxError & s) {
				errors -> push(s);
				synchronise();
			}
		}
		syntaxTree -> statements = statements;
		if (errors -> size() > 0) {
			if (syntaxTree) delete syntaxTree;
			errors -> shrinkToFit();
			UnitError * syntaxErrors = new UnitError(
				errors, * unit -> name
			);
			delete tokens; resetState();
			throw ParserErrorException(syntaxErrors);
		}
		delete tokens; delete errors;
		errors = nullptr; resetState();
		return syntaxTree;
	}

}

#endif
