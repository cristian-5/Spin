
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

#include "../Aliases/Prototypes.hpp"

#ifndef STACKPARSER
#define STACKPARSER

namespace Stack {

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
	Expression * Parser::comparison() {
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
	Expression * Parser::lowPriorityOperator() {
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
	Expression * Parser::mediumPriorityOperator() {
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
	Expression * Parser::highPriorityOperator() {
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
		try { return call(); }
		catch (SyntaxError & s) { throw; }
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
		ArrayList<Expression *> arguments = ArrayList<Expression *>();
		if (!check(TokenType::closeParenthesis)) {
			do {
				Expression * ex = nullptr;
				try { ex = expression(); }
				catch (SyntaxError & s) { throw; }
				arguments.push(ex);
			} while (match(TokenType::comma));
		}
		try { consume(TokenType::closeParenthesis, ")"); }
		catch (SyntaxError & s) {
			for (Expression * arg : arguments) delete arg;
			throw;
		}
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
	Statement * Parser::statement() {
		Statement * st = nullptr;
		try {
			Token keyword = peek();
			switch (keyword.type) {
				case TokenType::funcKeyword: st = functionStatement(); break;
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
		ArrayList<Statement *> statements = ArrayList<Statement *>();
		try {
			while (!check(TokenType::closeBrace) && !isAtEnd()) {
				statements.push(declaration());
			}
			consume(TokenType::closeBrace, "}");
		} catch (SyntaxError & s) {
			for (Statement * statement : statements) {
				delete statement;
			}
			throw;
		}
		statements.shrinkToFit();
		return new BlockStatement(statements);
	}
	Statement * Parser::breakStatement() {
		if (!isInControlFlow) {
			throw SyntaxError(
				"Unexpected break statement outside of control flow statements! What am I supposed to break?",
				Linker::getPosition(input, peek().position)
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
				Linker::getPosition(input, peek().position)
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
		Bool oldFunction = isInFunction;
		Token * name = nullptr;
		String * stringType = nullptr;
		ArrayList<Parameter *> params = ArrayList<Parameter *>();
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
							Linker::getPosition(input, er.position)
						);
					}
					p -> type = Converter::typeFromString(* stringType);
					delete stringType; stringType = nullptr;
					p -> tokenType = new Token(previous());
					params.push(p);
				} while (match(TokenType::comma));
			}
			consume(TokenType::closeParenthesis, ")");
			params.shrinkToFit();
			consume(TokenType::arrow, "arrow operator ->");
			stringType = typeString();
			if (!stringType) {
				Token er = peek();
				throw SyntaxError(
					"Expected type but found '" + er.lexeme + "'!",
					Linker::getPosition(input, er.position)
				);
			}
			returnType -> type = Converter::typeFromString(* stringType);
			delete stringType; stringType = nullptr;
			returnType -> tokenType = new Token(previous());
			if (!check(TokenType::openBrace)) {
				Token er = peek();
				throw SyntaxError(
					"Expected function body but found '" + er.lexeme + "'!",
					Linker::getPosition(input, er.position)
				);
			}
			body = (BlockStatement *)blockStatement();
		} catch (SyntaxError & s) {
			if (name) delete name;
			if (returnType) delete returnType;
			if (stringType) delete stringType;
			for (Parameter * p : params) delete p;
			throw;
		}
		isInFunction = oldFunction;
		return new FunctionStatement(name, params, body, returnType);
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

	void Parser::runTypeClassification() {
		if (tokens -> size() == 0) return;
		SizeType tokenCount = (tokens -> size()) - 1;
		for (SizeType i = 0; i < tokenCount; i++) {
			if (tokens -> at(i).isTypeType()) {
				i += 1;
				if (tokens -> at(i).type == TokenType::symbol) {
					tokens -> at(i).type = TokenType::customType;
				}
			}
		}
	} 

	/* Core */

	Bool Parser::match(TokenType type) {
		if (check(type)) {
			advance();
			return true;
		} return false;
	}
	Bool Parser::match(ArrayList<TokenType> * types) {
		for (TokenType & type : * types) {
			if (check(type)) {
				advance();
				return true;
			}
		} return false;
	}
	Bool Parser::check(TokenType type) {
		if (isAtEnd()) return false;
		return peek().type == type;
	}
	Bool Parser::isOutOfRange() {
		if (tokens -> size() == 0) return true;
		if (index >= tokens -> size()) return true;
		return false;
	}
	Bool Parser::isAtEnd() {
		return peek().type == TokenType::endFile;
	}
	Token Parser::peek() { return tokens -> at(index); }
	Token Parser::peekAdvance() {
		Token peekToken = peek();
		advance();
		return peekToken;
	}
	Token Parser::previous() { return tokens -> at(index - 1); }
	Token Parser::advance() {
		if (!isAtEnd()) index += 1;
		return previous();
	}
	Token Parser::consume(TokenType type, String lexeme) {
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

	void Parser::synchronise() {
		advance();
		while (!isAtEnd()) {
			if (previous().type == TokenType::semicolon) {
				return;
			}
			Token t = peek();
			if (t.type >= TokenType::tryKeyword &&
				t.type <= TokenType::restKeyword) return;
			advance();
		}
	}

	ArrayList<Statement *> * Parser::parse(ArrayList<Token> * tokens, String * input, String fileName) {
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
		runTypeClassification();
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

}

#endif
