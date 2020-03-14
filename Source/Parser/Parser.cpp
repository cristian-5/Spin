
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

#include "../Aliases/Prototypes/Token.hpp"
#include "../Aliases/Prototypes/Regex.hpp"
#include "../Aliases/Prototypes/SyntaxTree.hpp"
#include "../Aliases/Prototypes/Program.hpp"

namespace Spin {

	Expression * Parser::expression() {
		try { return assignment(); }
		catch (Program::Error & e) { throw; }
	}
	Expression * Parser::assignment() {
		Expression * ex = nullptr;
		try { ex = shortOR(); }
		catch (Program::Error & e) { throw; }
		if (match(TokenType::equal)) {
			Token * equals = new Token(previous());
			Expression * value = nullptr;
			try { value = assignment(); }
			catch (Program::Error & e) { delete ex; delete equals; throw; }
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
			Token errorToken = * equals;
			delete equals;
			throw Program::Error(
				currentUnit,
				"Expected identifier before assignment operator '='!",
				errorToken, ErrorCode::syx
			);
		}
		return ex;
	}
	Expression * Parser::shortOR() {
		Expression * ex = nullptr;
		try { ex = shortAND(); }
		catch (Program::Error & e) { throw; }
		while (match(TokenType::OR)) {
			Token * op = new Token(previous());
			Expression * right = nullptr;
			try { right = shortAND(); }
			catch (Program::Error & e) { delete ex; delete op; throw; }
			ex = new Logical(ex, op, right);
		}
		return ex;
	}
	Expression * Parser::shortAND() {
		Expression * ex = nullptr;
		try { ex = equality(); }
		catch (Program::Error & e) { throw; }
		while (match(TokenType::AND)) {
			Token * op = new Token(previous());
			Expression * right = nullptr;
			try { right = equality(); }
			catch (Program::Error & e) { delete ex; delete op; throw; }
			ex = new Logical(ex, op, right);
		}
		return ex;
	}
	Expression * Parser::equality() {
		Expression * ex = nullptr;
		try { ex = comparison(); }
		catch (Program::Error & e) { throw; }
		while (match(TokenType::equality) ||
			   match(TokenType::inequality)) {
			Token * op = new Token(previous());
			Expression * rs = nullptr;
			try { rs = comparison(); }
			catch (Program::Error & e) {
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
		catch (Program::Error & e) { throw; }
		while (match(TokenType::major) ||
			   match(TokenType::minor) ||
			   match(TokenType::majorEqual) ||
			   match(TokenType::minorEqual)) {
			Token * op = new Token(previous());
			Expression * rs = nullptr;
			try { rs = lowPriorityOperator(); }
			catch (Program::Error & e) {
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
		catch (Program::Error & e) { throw; }
		while (matchRange(TokenType::plusEqual,
						  TokenType::pipe)) {
			Token * op = new Token(previous());
			Expression * rs = nullptr;
			try { rs = mediumPriorityOperator(); }
			catch (Program::Error & e) {
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
					Token errorToken = * op;
					delete op;
					throw Program::Error(
						currentUnit,
						"Expected identifier before mutable assignment operator '" + errorToken.lexeme + "'!",
						errorToken, ErrorCode::syx
					);
				}
			} else ex = new Binary(ex, op, rs);
		}
		return ex;
	}
	Expression * Parser::mediumPriorityOperator() {
		Expression * ex = nullptr;
		try { ex = postfixOperator(); }
		catch (Program::Error & e) { throw; }
		while (matchRange(TokenType::starEqual,
						  TokenType::modulus)) {
			Token * op = new Token(previous());
			Expression * rs = nullptr;
			try { rs = postfixOperator(); }
			catch (Program::Error & e) {
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
					Token errorToken = * op;
					delete op;
					throw Program::Error(
						currentUnit,
						"Expected identifier before mutable assignment operator '" + errorToken.lexeme + "'!",
						errorToken, ErrorCode::syx
					);
				}
			} else ex = new Binary(ex, op, rs);
		}
		return ex;
	}
	Expression * Parser::postfixOperator() {
		Expression * ex = nullptr;
		try { ex = prefixOperator(); }
		catch (Program::Error & e) { throw; }
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
			catch (Program::Error & e) {
				delete op;
				throw;
			}
			return new Unary(op, rs);
		}
		try { return subscription(); }
		catch (Program::Error & e) { throw; }
	}
	Expression * Parser::subscription() {
		Expression * ex = nullptr;
		try { ex = call(); }
		catch (Program::Error & e) { throw; }
		while (true) {
			if (match(TokenType::openBracket)) {
				try { ex = completeSubscript(ex); }
				catch (Program::Error & e) { throw; }
			} else break;
		}
		return ex;
	}
	Expression * Parser::completeSubscript(Expression * item) {
		Token * bracket = new Token(previous());
		Expression * ex = nullptr;
		if (!check(TokenType::closeBracket)) {
			try { ex = expression(); }
			catch (Program::Error & e) { throw; }
		}
		try { consume(TokenType::closeBracket, "]"); }
		catch (Program::Error & e) { if (ex) delete ex; throw; }
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
					Token errorToken = peek();
					throw Program::Error(
						currentUnit,
						"Expected constructor after 'new' keyword but found '" + errorToken.lexeme + "'!",
						errorToken, ErrorCode::syx
					);
				}
				isConstructor = true;
			}
			ex = primary();
		} catch (Program::Error & e) { throw; }
		while (true) {
			if (match(TokenType::openParenthesis)) {
				try {
					ex = completeCall(ex, isConstructor);
					isConstructor = false;
				} catch (Program::Error & e) { throw; }
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
				} catch (Program::Error & e) { throw; }
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
				} catch (Program::Error & e) { throw; }
			 } else if (isConstructor) {
				Token errorToken = peek();
				throw Program::Error(
					currentUnit,
					"Expected constructor call after 'new' keyword but found '" + errorToken.lexeme + "'!",
					errorToken, ErrorCode::syx
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
				catch (Program::Error & e) { throw; }
				arguments -> push_back(ex);
			} while (match(TokenType::comma));
		}
		try { consume(TokenType::closeParenthesis, ")"); }
		catch (Program::Error & e) {
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
			} catch (Program::Error & e) {
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
					} catch (Program::Error & e) {
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
			catch (Program::Error & e) {
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
				throw Program::Error(
					currentUnit,
					"Unexpected use of 'self' reference outside of class declaration! What am I supposed to reference to?",
					peek(), ErrorCode::syx
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
				throw Program::Error(
					currentUnit,
					"Invalid inner product expression '" + t.lexeme + "'!",
					t, ErrorCode::syx
				);
			}
			return new Inner(new Token(t), bk.at(0), bk.at(1));
		} else if (t.type == TokenType::ketbraSymbol) {
			advance();
			Regex ketbra = Regex("\\|([A-Za-z_][A-Za-z0-9_]*)><([A-Za-z_][A-Za-z0-9_]*)\\|");
			Array<String> kb = RegexTools::findAllGroups(ketbra, t.lexeme);
			if (kb.size() != 2) {
				throw Program::Error(
					currentUnit,
					"Invalid outer product expression '" + t.lexeme + "'!",
					t, ErrorCode::syx
				);
			}
			return new Outer(new Token(t), kb.at(0), kb.at(1));
		}
		Token errorToken = (t.type == TokenType::endFile ? previous() : t);
		throw Program::Error(
			currentUnit,
			"Unexpected expression after '" + previous().lexeme + "'!",
			errorToken, ErrorCode::syx
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
		} catch (Program::Error & e) {
			if (type) delete type;
			throw;
		}
		if (type) delete type;
		return st;
	}
	Statement * Parser::variableDeclaration(String stringType, Bool isClass) {
		if (stringType == "Vector") {
			try { return vectorDeclaration(); }
			catch (Program::Error & e) { throw; }
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
		} catch (Program::Error & e) {
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
				Token errorToken = peek();
				throw Program::Error(
					currentUnit,
					"Expected <identifier| or |identifier> in bra-ket notation but found '" + errorToken.lexeme + "'!",
					errorToken, ErrorCode::syx
				);
			}
			if (match(TokenType::equal)) {
				equal = new Token(previous());
				initialiser = expression();
			}
			consume(TokenType::semicolon, ";");
		} catch (Program::Error & e) {
			if (vector) delete vector;
			if (initialiser) delete initialiser;
			throw;
		}
		if (vector -> lexeme.length() <= 2) {
			if (initialiser) delete initialiser;
			Token errorToken = * vector;
			delete vector;
			throw Program::Error(
				currentUnit,
				"Expected <identifier| or |identifier> in bra-ket notation but found '" + errorToken.lexeme + "'!",
				errorToken, ErrorCode::syx
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
				case TokenType::importKeyword: throw Program::Error(
					currentUnit,
					"Found unexpected 'import' keyword! Imports are only supported at the top of the file!",
					keyword, ErrorCode::ppr
				); break;
				default: st = expressionStatement(); break;
			}
		} catch (Program::Error & e) { throw; }
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
		} catch (Program::Error & e) {
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
		} catch (Program::Error & e) {
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
			throw Program::Error(
				currentUnit,
				"Unexpected break statement outside of control flow statements!",
				peek(), ErrorCode::syx
			);
		}
		Token * breakToken = new Token(peekAdvance());
		try {
			consume(TokenType::semicolon, ";");
		} catch (Program::Error & e) {
			delete breakToken;
			throw;
		}
		return new BreakStatement(breakToken);
	}
	Statement * Parser::continueStatement() {
		if (!isInControlFlow) {
			throw Program::Error(
				currentUnit,
				"Unexpected continue statement outside of control flow statements!",
				peek(), ErrorCode::syx
			);
		}
		Token * continueToken = new Token(peekAdvance());
		try {
			consume(TokenType::semicolon, ";");
		} catch (Program::Error & e) {
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
		} catch (Program::Error & e) {
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
				Token errorToken = peek();
				throw Program::Error(
					currentUnit,
					"Expected type but found '" + errorToken.lexeme + "'!",
					errorToken, ErrorCode::syx
				);
			}
			returnType -> type = Object::typeFromString(* stringType);
			delete stringType; stringType = nullptr;
			returnType -> tokenType = new Token(previous());
			if (!check(TokenType::openBrace)) {
				Token errorToken = peek();
				throw Program::Error(
					currentUnit,
					"Expected function body but found '" + errorToken.lexeme + "'!",
					errorToken, ErrorCode::syx
				);
			}
			body = (BlockStatement *)blockStatement();
		} catch (Program::Error & e) {
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
				Token errorToken = peek();
				throw Program::Error(
					currentUnit,
					"Expected function body but found '" + errorToken.lexeme + "'!",
					errorToken, ErrorCode::syx
				);
			}
			body = (BlockStatement *)blockStatement();
		} catch (Program::Error & e) {
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
						Token errorToken = peek();
						throw Program::Error(
							currentUnit,
							"Expected field declaration after @secure modifier but found '" + errorToken.lexeme + "' instead!",
							errorToken, ErrorCode::syx
						);
					}
				} else if (match(TokenType::immuneModifier)) {
					access = Modifier::immuneAccess; dynamic = false;
					if (check(TokenType::funcKeyword) || check(TokenType::procKeyword)) {
						Token errorToken = peek();
						throw Program::Error(
							currentUnit,
							"Expected field declaration after @immune modifier but found '" + errorToken.lexeme + "' instead!",
							errorToken, ErrorCode::syx
						);
					}
				} else if (match(TokenType::createSpecifier)) {
					if (atCreate) {
						Token errorToken = previous();
						throw Program::Error(
							currentUnit,
							"Redefinition of constructor with '" + errorToken.lexeme + "' specifier!",
							errorToken, ErrorCode::syx
						);
					}
					if (!check(TokenType::procKeyword)) {
						Token errorToken = peek();
						throw Program::Error(
							currentUnit,
							"Expected 'proc' keyword after constructor specifier but found '" + errorToken.lexeme + "' instead!",
							errorToken, ErrorCode::syx
						);
					}
					// Creation method:
					atCreate = (ProcedureStatement *) procedureStatement(true);
					if (atCreate -> name -> lexeme != name -> lexeme) {
						Token errorToken = Token(
							name -> lexeme,
							name -> type,
							atCreate -> name -> position
						);
						throw Program::Error(
							currentUnit,
							"Constructor must be named with its class identifier '" + errorToken.lexeme + "'!",
							errorToken, ErrorCode::syx
						);
					}
					continue;
				} else if (match(TokenType::deleteSpecifier)) {
					if (atDelete) {
						Token errorToken = previous();
						throw Program::Error(
							currentUnit,
							"Redefinition of constructor with '" + errorToken.lexeme + "' specifier!",
							errorToken, ErrorCode::syx
						);
					}
					if (!check(TokenType::procKeyword)) {
						Token errorToken = peek();
						throw Program::Error(
							currentUnit,
							"Expected 'proc' keyword after destructor specifier but found '" + errorToken.lexeme + "' instead!",
							errorToken, ErrorCode::syx
						);
					}
					// Deletion method:
					atDelete = (ProcedureStatement *) procedureStatement(true);
					if (atDelete -> name -> lexeme != name -> lexeme) {
						Token errorToken = Token(
							name -> lexeme,
							name -> type,
							atDelete -> name -> position
						);
						throw Program::Error(
							currentUnit,
							"Destructor must be named with its class identifier '" + errorToken.lexeme + "'!",
							errorToken, ErrorCode::syx
						);
					}
					if (!(atDelete -> params -> empty())) {
						Token errorToken = * (atDelete -> name);
						throw Program::Error(
							currentUnit,
							"The use of parameters in destructors is always forbidden due to automatic calls performed by the language when collecting the lost objects!",
							errorToken, ErrorCode::syx
						);
					}
					continue;
				} else {
					// If I'm not a field, a method or
					// a class specifier, what am I?
					Token errorToken = peek();
					throw Program::Error(
						currentUnit,
						"Expected access modifier or specifier but found '" + errorToken.lexeme + "' instead!",
						errorToken, ErrorCode::syx
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
		} catch (Program::Error & e) {
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
		return new ClassStatement(name, staticAttributes, dynamicAttributes, atCreate);
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
				Token errorToken = peek();
				throw Program::Error(
					currentUnit,
					"Expected field declaration but found '" + errorToken.lexeme + "'!",
					errorToken, ErrorCode::syx
				);
			}
		} catch (Program::Error & e) { throw; }
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
		} catch (Program::Error & e) {
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
		} catch (Program::Error & e) {
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
		} catch (Program::Error & e) {
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
		} catch (Program::Error & e) {
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
		} catch (Program::Error & e) {
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
		} catch (Program::Error & e) {
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
		} catch (Program::Error & e) { throw; }
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
		} catch (Program::Error & e) {
			if (returnToken) delete returnToken;
			throw;
		}
		if (ex) {
			if (!isInFunction) {
				Token errorToken = * returnToken;
				delete returnToken;
				throw Program::Error(
					currentUnit,
					"Unexpected return statement outside of function!",
					errorToken, ErrorCode::syx
				);
			}
		} else if (!isInProcedure) {
			Token errorToken = * returnToken;
			delete returnToken;
			throw Program::Error(
				currentUnit,
				"Unexpected return statement outside of procedure!",
				errorToken, ErrorCode::syx
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
		} catch (Program::Error & e) { throw; }
		throw Program::Error(
			currentUnit,
			"Unexpected delete statement found outside of valid context!",
			previous(), ErrorCode::syx
		);
	}
	Statement * Parser::fileStatement() {
		return new FileStatement(currentUnit);
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
		Token errorToken = (t.type == TokenType::endFile ? previous() : t);
		throw Program::Error(
			currentUnit,
			(lexeme.length() > 0 ? "Expected '" + lexeme +
			"' but found '" + t.lexeme + "'!" :
			"Expecting a different token than '" + t.lexeme + "'!"),
			errorToken, ErrorCode::syx
		);
	}

	inline void Parser::resetState() {
		currentUnit = nullptr;
		tokens = nullptr;
		SizeType index = 0;
		isInControlFlow = false;
		isInFunction = false;
		isInProcedure = false;
		isInClass = false;
	}

	SyntaxTree * Parser::parse(Program * code) {
		if (!code || !(code -> main)) return nullptr;
		if (code -> wings) {
			// TODO: Parse Libraries.
		}
		if (!(code -> main -> contents)) return nullptr;
		if (!(code -> main -> tokens)) return nullptr;
		if (!(code -> main -> name)) return nullptr;
		currentUnit = code -> main;
		tokens = code -> main -> tokens;
		try { consume(TokenType::beginFile, "beginFile"); }
		catch (Program::Error & e) { return nullptr; }
		SyntaxTree * syntaxTree = new SyntaxTree();
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
			} catch (Program::Error & e) { throw; }
		}
		syntaxTree -> statements = statements;
		if (code -> libraries &&
			!(code -> libraries -> empty())) {
			syntaxTree -> libraries = code -> libraries;
		}
		resetState();
		return syntaxTree;
	}

}

#endif
