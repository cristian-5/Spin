
/*!
 *
 *    + --------------------------------------- +
 *    |  Parser.cpp                             |
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
!*/

#include "../Aliases/Prototypes/Parser.hpp"

#ifndef SPIN_PARSER
#define SPIN_PARSER

#include "../Aliases/Prototypes/Manager.hpp"
#include "../Aliases/Prototypes/Token.hpp"
#include "../Aliases/Prototypes/Lexer.hpp"
#include "../Aliases/Prototypes/Regex.hpp"
#include "../Aliases/Prototypes/SyntaxTree.hpp"

namespace Spin {

	SyntaxError::SyntaxError(String m, UInt64 l): message(m), line(l) { }
	const UInt64 & SyntaxError::getLine() const {
		return line;
	}
	const String & SyntaxError::getMessage() const {
		return message;
	}

	UnitError::UnitError(Array<SyntaxError> * e, String n): errors(e), name(n) { }
	const Array<SyntaxError> * const UnitError::getErrors() const {
		return errors;
	}
	const String & UnitError::getName() const {
		return name;
	}
	UnitError::~UnitError() {
		delete errors;
	}

	ParserErrorException::ParserErrorException(Array<UnitError *> * u): units(u) { }
	ParserErrorException::ParserErrorException(UnitError * u): units(new Array<UnitError *>({ u })) { }
	const Array<UnitError *> * const ParserErrorException::getUnitErrors() const {
		return units;
	}
	ParserErrorException::~ParserErrorException() {
		if (!units) return;
		for (UnitError * u : * units) delete u;
		delete units;
	}

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
				delete ex;
				return new Assignment(name, value, equals);
			} else if (ex -> isInstanceOf<DynamicGet>()) {
				DynamicGet * get = (DynamicGet *) ex;
				return new DynamicSet(
					get -> object, get -> name,
					value, equals, get -> selfReference
				);
			} else if (ex -> isInstanceOf<StaticGet>()) {
				StaticGet * get = (StaticGet *) ex;
				return new StaticSet(
					get -> object, get -> name,
					value, equals, get -> selfReference
				);
			}
			if (value) delete value;
			delete ex;
			SizeType position = equals -> position;
			delete equals;
			throw SyntaxError(
				"Expected identifier before assignment operator '='!",
				Manager::getLine(currentUnit -> contents, position)
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
						Manager::getLine(currentUnit -> contents, position)
					);
				}
			} else ex = new Binary(ex, op, rs);
		}
		return ex;
	}
	Expression * Parser::mediumPriorityOperator() {
		Expression * ex = nullptr;
		try { ex = postfixOperator(); }
		catch (SyntaxError & e) { throw; }
		while (matchRange(TokenType::starEqual,
						  TokenType::modulus)) {
			Token * op = new Token(previous());
			Expression * rs = nullptr;
			try { rs = postfixOperator(); }
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
						Manager::getLine(currentUnit -> contents, position)
					);
				}
			} else ex = new Binary(ex, op, rs);
		}
		return ex;
	}
	Expression * Parser::postfixOperator() {
		Expression * ex = nullptr;
		try { ex = prefixOperator(); }
		catch (SyntaxError & s) { throw; }
		while (true) {
			if (matchRange(TokenType::conjugate,
						   TokenType::dagger)) {
				ex = new Unary(new Token(previous()), ex);
			} else break;
		}
		return ex;
	}
	Expression * Parser::prefixOperator() {
		if (match(TokenType::minus) ||
			match(TokenType::plus)  ||
			match(TokenType::tilde) ||
			match(TokenType::exclamationMark)) {
			Token * op = new Token(previous());
			Expression * rs = nullptr;
			try { rs = prefixOperator(); }
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
		Bool isConstructor = false;
		try {
			if (check(TokenType::newKeyword)) {
				advance();
				if (peek().type == TokenType::customType) {
					tokens -> at(index).type = TokenType::symbol;
				} else {
					Token temp = peek();
					throw SyntaxError(
						"Expected constructor after 'new' keyword but found '" +
						temp.lexeme + "'!",
						Manager::getLine(currentUnit -> contents, temp.position)
					);
				}
				isConstructor = true;
			}
			ex = primary();
		} catch (SyntaxError & s) { throw; }
		while (true) {
			if (match(TokenType::openParenthesis)) {
				try {
					ex = completeCall(ex, isConstructor);
					isConstructor = false;
				} catch (SyntaxError & s) { throw; }
			} else if (match(TokenType::dot)) {
				try {
					Bool selfReference = false;
					if (index > 2 &&
						tokens -> at(index - 2).type ==
						TokenType::selfKeyword) {
						selfReference = true;
					}
					Token * name = new Token(
						consume(TokenType::symbol, "identifier")
					);
					ex = new DynamicGet(ex, name, selfReference);
				} catch (SyntaxError & s) { throw; }
			 } else if (match(TokenType::doublecolon)) {
				try {
					Bool selfReference = false;
					if (index > 2 &&
						tokens -> at(index - 2).type ==
						TokenType::selfKeyword) {
						selfReference = true;
					}
					Token * name = new Token(
						consume(TokenType::symbol, "identifier")
					);
					ex = new StaticGet(ex, name, selfReference);
				} catch (SyntaxError & s) { throw; }
			 } else if (isConstructor) {
				Token temp = peek();
				throw SyntaxError(
					"Expected constructor call after 'new' keyword but found '" +
					temp.lexeme + "'!",
					Manager::getLine(currentUnit -> contents, temp.position)
				);
			} else break;
		}
		return ex;
	}
	Expression * Parser::completeCall(Expression * callee, Bool isConstructor) {
		Token * parenthesis = new Token(previous());
		Array<Expression *> * arguments = new Array<Expression *>();
		if (!check(TokenType::closeParenthesis)) {
			do {
				Expression * ex = nullptr;
				try { ex = expression(); }
				catch (SyntaxError & s) { throw; }
				arguments -> push_back(ex);
			} while (match(TokenType::comma));
		}
		try { consume(TokenType::closeParenthesis, ")"); }
		catch (SyntaxError & s) {
			for (Expression * arg : * arguments) delete arg;
			delete arguments;
			throw;
		}
		arguments -> shrink_to_fit();
		return new Call(callee, parenthesis, arguments, isConstructor);
	}
	Expression * Parser::primary() {
		Token t = peek();
		if (t.isTypeLiteral()) {
			advance();
			return new Literal(new Token(t));
		} else if (t.type == TokenType::symbol ||
				   t.type == TokenType::customType ||
				   t.type == TokenType::basicType) {
			advance();
			return new Identifier(new Token(t));
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
						values -> push_back(ex);
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
			values -> shrink_to_fit();
			return new List(values);
		} else if (t.type == TokenType::selfKeyword) {
			advance();
			if (!isInClass) {
				throw SyntaxError(
					"Unexpected use of 'self' reference outside of class declaration! What am I supposed to reference to?",
					Manager::getLine(currentUnit -> contents, peek().position)
				);
			}
			return new Self(new Token(t));
		} else if (t.type == TokenType::ketSymbol) {
			advance();
			String ket = t.lexeme.substr(1, t.lexeme.length() - 2);
			return new Ket(new Token(t), ket);
		} else if (t.type == TokenType::braSymbol) {
			advance();
			String bra = t.lexeme.substr(1, t.lexeme.length() - 2);
			return new Bra(new Token(t), bra);
		} else if (t.type == TokenType::braketSymbol) {
			advance();
			Regex braket = Regex("<([A-Za-z_][A-Za-z0-9_]*)\\|([A-Za-z_][A-Za-z0-9_]*)>");
			Array<String> bk = RegexTools::findAllGroups(braket, t.lexeme);
			if (bk.size() != 2) {
				throw SyntaxError(
					"Invalid inner product expression '" + t.lexeme + "'!",
					Manager::getLine(currentUnit -> contents, t.position)
				);
			}
			return new Inner(new Token(t), bk.at(0), bk.at(1));
		} else if (t.type == TokenType::ketbraSymbol) {
			advance();
			Regex ketbra = Regex("\\|([A-Za-z_][A-Za-z0-9_]*)><([A-Za-z_][A-Za-z0-9_]*)\\|");
			Array<String> kb = RegexTools::findAllGroups(ketbra, t.lexeme);
			if (kb.size() != 2) {
				throw SyntaxError(
					"Invalid outer product expression '" + t.lexeme + "'!",
					Manager::getLine(currentUnit -> contents, t.position)
				);
			}
			return new Outer(new Token(t), kb.at(0), kb.at(1));
		}
		UInt64 line = 0;
		if (t.type == TokenType::endFile) {
			line = Manager::getLine(currentUnit -> contents, previous().position);
		} else line = Manager::getLine(currentUnit -> contents, t.position);
		throw SyntaxError(
			"Unexpected expression after '" + previous().lexeme + "'!", line
		);
	}

	/* Statements */

	String * Parser::typeString(Bool current) {
		if (current) return new String(peek().lexeme);
		return new String(previous().lexeme);
	}
	Statement * Parser::declaration() {
		Statement * st = nullptr;
		String * type = nullptr;
		try {
			if (match(TokenType::customType)) {
				if (peek().type == TokenType::doublecolon) {
					recede(); st = statement();
				} else {
					type = typeString();
					st = variableDeclaration(* type, true);
					delete type; type = nullptr;
				}
			} else if (match(TokenType::basicType)) {
				type = typeString();
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
	Statement * Parser::variableDeclaration(String stringType, Bool isClass) {
		if (stringType == "Vector") {
			try { return vectorDeclaration(); }
			catch (SyntaxError & s) { throw; }
		}
		Token * obj = nullptr;
		Token * name = nullptr;
		Token * equal = nullptr;
		Expression * initialiser = nullptr;
		try {
			if (isClass) obj = new Token(previous());
			name = new Token(consume(TokenType::symbol, "identifier"));
			if (match(TokenType::equal)) {
				equal = new Token(previous());
				initialiser = expression();
			}
			consume(TokenType::semicolon, ";");
		} catch (SyntaxError & s) {
			if (obj) delete obj;
			if (name) delete name;
			if (initialiser) delete initialiser;
			throw;
		}
		BasicType type = isClass ?
						 BasicType::InstanceType :
						 Object::typeFromString(stringType);
		return new VariableStatement(name, initialiser, type, equal, obj);
	}
	Statement * Parser::vectorDeclaration() {
		Token * vector = nullptr;
		Token * equal = nullptr;
		Expression * initialiser = nullptr;
		try {
			if (match(TokenType::braSymbol) ||
				match(TokenType::ketSymbol)) {
				vector = new Token(previous());
			} else {
				Token t = peek();
				throw SyntaxError(
					"Expected <identifier| or |identifier> in bra-ket notation but found '"
					+ t.lexeme + "'!",
					Manager::getLine(currentUnit -> contents, t.position)
				);
			}
			if (match(TokenType::equal)) {
				equal = new Token(previous());
				initialiser = expression();
			}
			consume(TokenType::semicolon, ";");
		} catch (SyntaxError & s) {
			if (vector) delete vector;
			if (initialiser) delete initialiser;
			throw;
		}
		if (vector -> lexeme.length() <= 2) {
			if (vector) delete vector;
			if (initialiser) delete initialiser;
			throw SyntaxError(
				"Expected <identifier| or |identifier> in bra-ket notation but found '"
				+ vector -> lexeme + "'!",
				Manager::getLine(currentUnit -> contents, vector -> position)
			);
		}
		String name = vector -> lexeme.substr(1, vector -> lexeme.length() - 2);
		return new VectorStatement(vector, name, initialiser, equal);
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
				case TokenType::semicolon: advance(); st = new RestStatement(); break;
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
				statements -> push_back(declaration());
			}
			consume(TokenType::closeBrace, "}");
		} catch (SyntaxError & s) {
			for (Statement * statement : * statements) {
				delete statement;
			}
			delete statements;
			throw;
		}
		statements -> shrink_to_fit();
		return new BlockStatement(statements);
	}
	Statement * Parser::breakStatement() {
		if (!isInControlFlow) {
			throw SyntaxError(
				"Unexpected break statement outside of control flow statements! What am I supposed to break?",
				Manager::getLine(currentUnit -> contents, peek().position)
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
				Manager::getLine(currentUnit -> contents, peek().position)
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
			name = new Token(consume(TokenType::symbol, "function identifier"));
			consume(TokenType::openParenthesis, "(");
			if (!check(TokenType::closeParenthesis)) {
				do {
					Parameter * p = new Parameter();
					p -> name = new Token(consume(TokenType::symbol, "parameter identifier"));
					consume(TokenType::colon, ":");
					stringType = typeString(true);
					p -> type = Object::typeFromString(* stringType);
					delete stringType; stringType = nullptr;
					p -> tokenType = new Token(peekAdvance());
					params -> push_back(p);
				} while (match(TokenType::comma));
			}
			consume(TokenType::closeParenthesis, ")");
			params -> shrink_to_fit();
			consume(TokenType::arrow, "arrow operator ->");
			stringType = typeString();
			advance();
			if (!stringType) {
				Token er = peek();
				throw SyntaxError(
					"Expected type but found '" + er.lexeme + "'!",
					Manager::getLine(currentUnit -> contents, er.position)
				);
			}
			returnType -> type = Object::typeFromString(* stringType);
			delete stringType; stringType = nullptr;
			returnType -> tokenType = new Token(previous());
			if (!check(TokenType::openBrace)) {
				Token er = peek();
				throw SyntaxError(
					"Expected function body but found '" + er.lexeme + "'!",
					Manager::getLine(currentUnit -> contents, er.position)
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
	Statement * Parser::procedureStatement(Bool allowType) {
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
			if (allowType && !isAtEnd() && peek().type == TokenType::customType) {
				tokens -> at(index).type = TokenType::symbol;
			}
			name = new Token(consume(TokenType::symbol, "procedure identifier"));
			consume(TokenType::openParenthesis, "(");
			if (!check(TokenType::closeParenthesis)) {
				do {
					Parameter * p = new Parameter();
					p -> name = new Token(consume(TokenType::symbol, "parameter identifier"));
					consume(TokenType::colon, ":");
					stringType = typeString(true);
					p -> type = Object::typeFromString(* stringType);
					delete stringType; stringType = nullptr;
					p -> tokenType = new Token(peekAdvance());
					params -> push_back(p);
				} while (match(TokenType::comma));
			}
			consume(TokenType::closeParenthesis, ")");
			params -> shrink_to_fit();
			if (!check(TokenType::openBrace)) {
				Token er = peek();
				throw SyntaxError(
					"Expected function body but found '" + er.lexeme + "'!",
					Manager::getLine(currentUnit -> contents, er.position)
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
		Token * name = nullptr;
		isInClass = true;
		Array<AttributeStatement *> * staticAttributes = new Array<AttributeStatement *>();
		Array<AttributeStatement *> * dynamicAttributes = new Array<AttributeStatement *>();
		ProcedureStatement * atCreate = nullptr;
		ProcedureStatement * atDelete = nullptr;
		try {
			name = new Token(consume(TokenType::customType, "identifier"));
			consume(TokenType::openBrace, "{");
			while (!check(TokenType::closeBrace) && !isAtEnd()) {
				// TODO: Parse the class body.
				Modifier access;
				Bool dynamic = true;
				if (match(TokenType::publicModifier)) access = Modifier::publicAccess;
				else if (match(TokenType::hiddenModifier)) access = Modifier::hiddenAccess;
				else if (match(TokenType::sharedModifier)) {
					access = Modifier::publicAccess; dynamic = false;
				} else if (match(TokenType::staticModifier)) {
					access = Modifier::hiddenAccess; dynamic = false;
				} else if (match(TokenType::secureModifier)) {
					access = Modifier::secureAccess;
					if (check(TokenType::funcKeyword) || check(TokenType::procKeyword)) {
						Token er = peek();
						throw SyntaxError(
							"Expected field declaration after @secure modifier but found '" + er.lexeme + "' instead!",
							Manager::getLine(currentUnit -> contents, er.position)
						);
					}
				} else if (match(TokenType::immuneModifier)) {
					access = Modifier::immuneAccess; dynamic = false;
					if (check(TokenType::funcKeyword) || check(TokenType::procKeyword)) {
						Token er = peek();
						throw SyntaxError(
							"Expected field declaration after @immune modifier but found '" + er.lexeme + "' instead!",
							Manager::getLine(currentUnit -> contents, er.position)
						);
					}
				} else if (match(TokenType::createSpecifier)) {
					if (atCreate) {
						Token er = previous();
						throw SyntaxError(
							"Redefinition of constructor with '" + er.lexeme + "' specifier!",
							Manager::getLine(currentUnit -> contents, er.position)
						);
					}
					if (!check(TokenType::procKeyword)) {
						Token er = peek();
						throw SyntaxError(
							"Expected 'proc' keyword after constructor specifier but found '" + er.lexeme + "' instead!",
							Manager::getLine(currentUnit -> contents, er.position)
						);
					}
					// Creation method:
					atCreate = (ProcedureStatement *) procedureStatement(true);
					if (atCreate -> name -> lexeme != name -> lexeme) {
						throw SyntaxError(
							"Constructor must be named with its class identifier '" + name -> lexeme + "'!",
							Manager::getLine(currentUnit -> contents, atCreate -> name -> position)
						);
					}
					staticAttributes -> push_back(
						new AttributeStatement(
							atCreate, Modifier::hiddenAccess
						)
					);
					continue;
				} else if (match(TokenType::deleteSpecifier)) {
					if (atDelete) {
						Token er = previous();
						throw SyntaxError(
							"Redefinition of constructor with '" + er.lexeme + "' specifier!",
							Manager::getLine(currentUnit -> contents, er.position)
						);
					}
					if (!check(TokenType::procKeyword)) {
						Token er = peek();
						throw SyntaxError(
							"Expected 'proc' keyword after destructor specifier but found '" + er.lexeme + "' instead!",
							Manager::getLine(currentUnit -> contents, er.position)
						);
					}
					// Deletion method:
					atDelete = (ProcedureStatement *) procedureStatement(true);
					if (atDelete -> name -> lexeme != name -> lexeme) {
						throw SyntaxError(
							"Destructor must be named with its class identifier '" + name -> lexeme + "'!",
							Manager::getLine(currentUnit -> contents, atDelete -> name -> position)
						);
					}
					if (!(atDelete -> params -> empty())) {
						throw SyntaxError(
							"The use of parameters in destructors is always forbidden due to automatic calls performed by the language when collecting the lost objects!",
							Manager::getLine(currentUnit -> contents, atDelete -> name -> position)
						);
					}
					staticAttributes -> push_back(
						new AttributeStatement(
							atDelete, Modifier::hiddenAccess
						)
					);
					continue;
				} else {
					// If I'm not a field, a method or
					// a class specifier, what am I?
					Token er = peek();
					throw SyntaxError(
						"Expected access modifier or specifier but found '" + er.lexeme + "' instead!",
						Manager::getLine(currentUnit -> contents, er.position)
					);
				}
				Token p = peek();
				if (p.type == TokenType::funcKeyword) {
					// Method declaration:
					AttributeStatement * method = new AttributeStatement(
						functionStatement(), access
					);
					if (dynamic) dynamicAttributes -> push_back(method);
					else staticAttributes -> push_back(method);
				} else if (p.type == TokenType::procKeyword) {
					// Method declaration:
					AttributeStatement * method = new AttributeStatement(
						procedureStatement(), access
					);
					if (dynamic) dynamicAttributes -> push_back(method);
					else staticAttributes -> push_back(method);
				} else {
					// Field declaration:
					AttributeStatement * field = new AttributeStatement(
						fieldStatement(), access
					);
					if (dynamic) dynamicAttributes -> push_back(field);
					else staticAttributes -> push_back(field);
				}
			}
			consume(TokenType::closeBrace, "}");
		} catch (SyntaxError & s) {
			if (name) delete name;
			if (atCreate) delete atCreate;
			if (atDelete) delete atDelete;
			for (AttributeStatement * a : * staticAttributes) delete a;
			for (AttributeStatement * a : * dynamicAttributes) delete a;
			delete staticAttributes; delete dynamicAttributes;
			throw;
		}
		isInClass = false;
		isInControlFlow = oldControlFlow;
		isInProcedure = oldProcedure;
		isInFunction = oldFunction;
		return new ClassStatement(name, staticAttributes, dynamicAttributes);
	}
	Statement * Parser::fieldStatement() {
		Statement * field = nullptr;
		try {
			String * type = nullptr;
			if (match(TokenType::customType)) {
				type = typeString();
				field = variableDeclaration(* type, true);
				delete type;
			} else if (match(TokenType::basicType)) {
				type = typeString();
				field = variableDeclaration(* type);
				delete type;
			} else {
				Token t = peek();
				throw SyntaxError(
					"Expected field declaration but found '" + t.lexeme + "'!",
					Manager::getLine(currentUnit -> contents, t.position)
				);
			}
		} catch (SyntaxError & s) { throw; }
		return field;
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
					Manager::getLine(currentUnit -> contents, position)
				);
			}
		} else if (!isInProcedure) {
			UInt32 position = returnToken -> position;
			if (returnToken) delete returnToken;
			throw SyntaxError(
				"Unexpected return statement outside of procedure! Where am I supposed to return to?",
				Manager::getLine(currentUnit -> contents, position)
			);
		}
		return new ReturnStatement(ex, returnToken);
	}
	Statement * Parser::deleteStatement() {
		advance();
		try {
			Token name = consume(TokenType::symbol, "variable identifier");
			consume(TokenType::semicolon, ";");
			return new DeleteStatement(new Token(name));
		} catch (SyntaxError & s) { throw; }
		throw SyntaxError(
			"Unexpected delete statement found outside of valid context!",
			Manager::getLine(currentUnit -> contents, previous().position)
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
		for (SizeType i = 0; i < tokenCount; i += 1) {
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
		String import;
		while (i < tokens -> size()) {
			if (tokens -> at(i).type == TokenType::symbol) {
				import += tokens -> at(i).lexeme;
				tokens -> at(i).type = TokenType::empty;
			} else throw SyntaxError(
				"Expected 'identifier' but found '" +
				tokens -> at(i).lexeme + "'!",
				Manager::getLine(currentUnit -> contents, tokens -> at(i).position)
			);
			i += 1;
			if (i < tokens -> size()) {
				switch (tokens -> at(i).type) {
					case TokenType::semicolon: {
						tokens -> at(i).type = TokenType::empty;
						return import;
					}
					case TokenType::doublecolon: {
						tokens -> at(i).type = TokenType::empty;
						import += "::";
						i += 1;
						break;
					}
					default: throw SyntaxError(
						"Expected ';' but found '" +
						tokens -> at(i).lexeme + "'!",
						Manager::getLine(currentUnit -> contents, tokens -> at(i).position)
					);
				}
			} else break;
		}
		throw SyntaxError(
			"Expected ';' but found '" +
			tokens -> at(i).lexeme + "'!",
			Manager::getLine(currentUnit -> contents, tokens -> at(i).position)
		);
	}
	SyntaxTree * Parser::runImportClassification(SyntaxTree * syntaxTree) {
		if (!syntaxTree) syntaxTree = new SyntaxTree();
		if (!tokens) return nullptr;
		SizeType tokenCount = tokens -> size();
		for (SizeType i = 0; i < tokenCount; i += 1) {
			if (tokens -> at(i).type == TokenType::importKeyword) {
				try {
					SizeType store = i;
					String s = parseImport(i);
					if (s == "Console") syntaxTree -> consoleLibrary = true;
					else if (s == "Maths") syntaxTree -> mathsLibrary = true;
					else if (s == "Kronos") syntaxTree -> kronosLibrary = true;
					else throw SyntaxError(
						"Invalid import statement asks for unknown library '" + s + "'!",
						Manager::getLine(currentUnit -> contents, tokens -> at(store).position)
					);
				} catch (SyntaxError & r) { throw; }
			}
		}
		return syntaxTree;
	}
	void Parser::runCastClassification() {
		if (!tokens) return;
		SizeType tokenCount = (tokens -> size()) - 1;
		for (SizeType i = 0; i < tokenCount; i += 1) {
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
		for (SizeType i = 0; i < tokenCount; i += 1) {
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
				newTokens -> push_back(token);
			}
		}
		delete tokens;
		newTokens -> shrink_to_fit();
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
	inline Token Parser::recede() {
		if (index > 0) index -= 1;
		return peek();
	}
	inline Token Parser::consume(TokenType type, String lexeme) {
		Token t = peek();
		if (check(type)) return advance();
		UInt64 line = 0;
		if (t.type == TokenType::endFile) {
			line = Manager::getLine(currentUnit -> contents, previous().position);
		} else line = Manager::getLine(currentUnit -> contents, t.position);
		throw SyntaxError(
			(lexeme.length() > 0 ? "Expected '" + lexeme +
			"' but found '" + t.lexeme + "'!" :
			"Expecting a different token than '" + t.lexeme + "'!"), line
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
				errors -> push_back(SyntaxError("The code unit is empty!", 0));
				errors -> shrink_to_fit();
				syntaxErrors -> push_back(new UnitError(errors, * unit -> name));
				errors = new Array<SyntaxError>(); resetState(); continue;
			}
			tokens = new Array<Token>(* unit -> tokens);
			currentUnit = unit;
			try { consume(TokenType::beginFile, "beginFile"); }
			catch (SyntaxError & s) {
				errors -> push_back(s);
				errors -> shrink_to_fit();
				syntaxErrors -> push_back(new UnitError(errors, * unit -> name));
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
				errors -> push_back(s);
				errors -> shrink_to_fit();
				syntaxErrors -> push_back(new UnitError(errors, * unit -> name));
				errors = new Array<SyntaxError>();
				delete tokens; resetState(); continue;
			}
			cleanEmptyTokens();
			if (!(syntaxTree -> statements)) {
				syntaxTree -> statements = new Array<Statement *>();
			}
			syntaxTree -> statements -> push_back(fileStatement());
			while (!isAtEnd()) {
				try {
					Statement * d = declaration();
					if (d -> isInstanceOf<ClassStatement>()    ||
						d -> isInstanceOf<FunctionStatement>() ||
						d -> isInstanceOf<ProcedureStatement>()) {
						syntaxTree -> statements -> insert(
							syntaxTree -> statements -> begin(), d
						);
					} else syntaxTree -> statements -> push_back(d);
				} catch (SyntaxError & s) {
					errors -> push_back(s);
					synchronise();
				}
			}
			if (errors -> size() > 0) {
				if (syntaxTree) {
					delete syntaxTree;
					syntaxTree = nullptr;
				}
				syntaxErrors -> push_back(new UnitError(errors, * unit -> name));
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
			errors -> push_back(SyntaxError("The code unit is empty!", 0));
			errors -> shrink_to_fit();
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
			errors -> push_back(s);
			errors -> shrink_to_fit();
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
			errors -> push_back(s);
			errors -> shrink_to_fit();
			UnitError * syntaxErrors = new UnitError(
				errors, * unit -> name
			);
			delete tokens; resetState();
			throw ParserErrorException(syntaxErrors);
		}
		cleanEmptyTokens();
		Array<Statement *> * statements = new Array<Statement *>();
		statements -> push_back(fileStatement());
		while (!isAtEnd()) {
			try {
				Statement * d = declaration();
				if (d -> isInstanceOf<ClassStatement>()     ||
					d -> isInstanceOf<FunctionStatement>()  ||
					d -> isInstanceOf<ProcedureStatement>()) {
					statements -> insert(statements -> begin(), d);
				} else statements -> push_back(d);
			} catch (SyntaxError & s) {
				errors -> push_back(s);
				synchronise();
			}
		}
		syntaxTree -> statements = statements;
		if (errors -> size() > 0) {
			if (syntaxTree) delete syntaxTree;
			errors -> shrink_to_fit();
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
