
/*!
 *
 *    + --------------------------------------- +
 *    |  Compiler.cpp                           |
 *    |                                         |
 *    |                Compiler                 |
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

#include "Compiler.hpp"

#ifndef SPIN_COMPILER_CPP
#define SPIN_COMPILER_CPP

#include "Converter.hpp"

namespace Spin {

	const Dictionary<Token::Type, Compiler::ParseRule> Compiler::rules = {
	
		{ Token::Type::openParenthesis, { grouping, nullptr, Precedence::none } },

		{ Token::Type::exclamationMark, { unary, nullptr, Precedence::none } },
		{ Token::Type::tilde, { unary, nullptr, Precedence::term } },
		{ Token::Type::minus, { unary, binary, Precedence::term } },
		{ Token::Type::plus, { nullptr, binary, Precedence::term } },
		{ Token::Type::slash, { nullptr, binary, Precedence::factor } },
		{ Token::Type::star, { nullptr, binary, Precedence::factor } },

		{ Token::Type::intLiteral, { integerLiteral, nullptr, Precedence::none } },
		{ Token::Type::realLiteral, { realLiteral, nullptr, Precedence::none } },
		{ Token::Type::imaginaryLiteral, { imaginaryLiteral, nullptr, Precedence::none } },
		{ Token::Type::stringLiteral, { stringLiteral, nullptr, Precedence::none } },
		{ Token::Type::charLiteral, { characterLiteral, nullptr, Precedence::none } },
		{ Token::Type::boolLiteral, { booleanLiteral, nullptr, Precedence::none } },

		{ Token::Type::inequality, { nullptr, binary, Precedence::equality } },
		{ Token::Type::equality, { nullptr, binary, Precedence::equality } },

		{ Token::Type::major, { nullptr, binary, Precedence::comparison } },
		{ Token::Type::majorEqual, { nullptr, binary, Precedence::comparison } },
		{ Token::Type::minor, { nullptr, binary, Precedence::comparison } },
		{ Token::Type::minorEqual, { nullptr, binary, Precedence::comparison } },

	};

	const Dictionary<Unary, Type> Compiler::prefix = {
		{ compose(Token::Type::exclamationMark, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::exclamationMark, Type::BooleanType), Type::BooleanType },
		{ compose(Token::Type::minus, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::RealType), Type::RealType },
		{ compose(Token::Type::minus, Type::ImaginaryType), Type::ImaginaryType },
		{ compose(Token::Type::tilde, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::tilde, Type::ByteType), Type::ByteType },
	};
	const Dictionary<Binary, Type> Compiler::infix = {
		// # + # ------------------------------------------------------------- # Composing Addition #
		{ compose(Token::Type::plus, Type::CharacterType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::plus, Type::CharacterType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::plus, Type::CharacterType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::plus, Type::ByteType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::plus, Type::ByteType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::plus, Type::ByteType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::plus, Type::IntegerType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::plus, Type::IntegerType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::plus, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::plus, Type::IntegerType, Type::RealType), Type::RealType },
		{ compose(Token::Type::plus, Type::RealType, Type::IntegerType), Type::RealType },
		{ compose(Token::Type::plus, Type::RealType, Type::RealType), Type::RealType },
		{ compose(Token::Type::plus, Type::ImaginaryType, Type::ImaginaryType), Type::ImaginaryType },
		// # - # ------------------------------------------------------------- # Composing Subtraction #
		{ compose(Token::Type::minus, Type::CharacterType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::CharacterType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::CharacterType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::ByteType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::ByteType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::ByteType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::IntegerType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::IntegerType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::IntegerType, Type::RealType), Type::RealType },
		{ compose(Token::Type::minus, Type::RealType, Type::IntegerType), Type::RealType },
		{ compose(Token::Type::minus, Type::RealType, Type::RealType), Type::RealType },
		{ compose(Token::Type::minus, Type::ImaginaryType, Type::ImaginaryType), Type::ImaginaryType },
		// # * # ------------------------------------------------------------- # Composing Multiplication #
		// # / # ------------------------------------------------------------- # Composing Division #
		// # % # ------------------------------------------------------------- # Composing Modulus #
	};

	void Compiler::booleanLiteral() {
		if (Converter::stringToBoolean(previous.lexeme)) {
			emitOperation(OPCode::PST);
		} else emitOperation(OPCode::PSF);
		typeStack.push(Type::BooleanType);
	}
	void Compiler::characterLiteral() {
		typeStack.push(Type::CharacterType);
	}
	void Compiler::stringLiteral() {
		typeStack.push(Type::StringType);
	}
	void Compiler::imaginaryLiteral() {
		Real literal = Converter::stringToImaginary(
			previous.lexeme
		);
		emitOperation(
			{ OPCode::CNS, { .index = program -> literals.size() } }
		);
		program -> literals.push_back(
			{ .real = literal }
		);
		typeStack.push(Type::ImaginaryType);
	}
	void Compiler::realLiteral() {
		Real literal = Converter::stringToReal(
			previous.lexeme
		);
		emitOperation(
			{ OPCode::CNS, { .index = program -> literals.size() } }
		);
		program -> literals.push_back(
			{ .real = literal }
		);
		typeStack.push(Type::RealType);
	}
	void Compiler::integerLiteral() {
		Integer literal = Converter::stringToInteger(
			previous.lexeme
		);
		emitOperation(
			{ OPCode::CNS, { .index = program -> literals.size() } }
		);
		program -> literals.push_back(
			{ .integer = literal }
		);
		typeStack.push(Type::IntegerType);
	}

	void Compiler::expression() {
		try { parsePrecedence(Precedence::assignment); }
		catch (Program::Error & e) { throw; }
	}
	void Compiler::grouping() {
		try {
			expression();
			consume(Token::Type::closeParenthesis, ")");
		} catch (Program::Error & e) { throw; }
	}
	void Compiler::binary() {
		const Token token = previous;
		ParseRule rule = getRule(token.type);
		Type typeA = typeStack.pop();
		try { parsePrecedence((Precedence)(rule.precedence + 1)); }
		catch (Program::Error & e) { throw; }
		Type typeB = typeStack.pop();
		const Types types = runtimeCompose(typeA, typeB);
		auto search = infix.find(
			runtimeCompose(token.type, typeA, typeB)
		);
		if (search == infix.end()) {
			throw Program::Error(
				currentUnit,
				"Binary operator '" + token.lexeme + "' doesn't support operands of type '" +
				Converter::typeToString(typeA) + "' and '" +
				Converter::typeToString(typeB) + "'!",
				token, ErrorCode::syx
			);
		}
		switch (token.type) {
			case  Token::Type::plus: emitOperation({ OPCode::ADD, { .types = types } }); break;
			case Token::Type::minus: emitOperation({ OPCode::SUB, { .types = types } }); break;
			case  Token::Type::star: emitOperation({ OPCode::MUL, { .types = types } }); break;
			case Token::Type::slash: emitOperation({ OPCode::DIV, { .types = types } }); break;
			case   Token::Type::equality: emitOperation({ OPCode::EQL, { .types = types } }); break;
			case Token::Type::inequality: emitOperation({ OPCode::NEQ, { .types = types } }); break;
			case      Token::Type::major: emitOperation({ OPCode::GRT, { .types = types } }); break;
			case Token::Type::majorEqual: emitOperation({ OPCode::GEQ, { .types = types } }); break;
			case      Token::Type::minor: emitOperation({ OPCode::LSS, { .types = types } }); break;
			case Token::Type::minorEqual: emitOperation({ OPCode::LEQ, { .types = types } }); break;
		}
		typeStack.push(search -> second);
	}
	void Compiler::unary() {
		const Token token = previous;
		try { parsePrecedence(Precedence::unary); }
		catch (Program::Error & e) { throw; }
		Type type = typeStack.pop();
		auto search = prefix.find(
			runtimeCompose(token.type, type)
		);
		if (search == prefix.end()) {
			throw Program::Error(
				currentUnit,
				"Unary operator '" + token.lexeme + "' doesn't support any operand of type '" +
				Converter::typeToString(type) + "'!",
				token, ErrorCode::syx
			);
		}
		switch (token.type) {
			case Token::Type::exclamationMark: emitOperation({ OPCode::NOT, { .type = type } }); break;
			case Token::Type::minus: emitOperation({ OPCode::NEG, { .type = type } }); break;
			case Token::Type::tilde: emitOperation({ OPCode::INV, { .type = type } }); break;
		}
		typeStack.push(search -> second);
	}

	void Compiler::parsePrecedence(Precedence precedence) {
		advance();
		ParseFunction prefixRule = getRule(previous.type).prefix;
		if (!prefixRule) {
			throw Program::Error(
				currentUnit,
				"Expected expression after '" + previous.lexeme + "'!",
				current, ErrorCode::syx
			);
		}
		try { (this ->* prefixRule)(); }
		catch (Program::Error & e) { throw; }
		while (precedence <= getRule(previous.type).precedence) {
			ParseFunction infixRule = getRule(previous.type).infix;
			try { (this ->* infixRule)(); }
			catch (Program::Error & e) { throw; }
		}
	}

	Compiler::ParseRule Compiler::getRule(Token::Type token) {
		return rules.at(token);
	}

	inline void Compiler::advance() {
		previous = current;
		current = tokens -> at(index);
		index += 1;
	}
	inline void Compiler::consume(Token::Type type, String lexeme) {
		if (current.type == type) {
			advance();
			return;
		}
		throw Program::Error(
			currentUnit,
			(lexeme.length() > 0 ? "Expected '" + lexeme +
			"' but found '" + current.lexeme + "'!" :
			"Expecting a different token than '" + current.lexeme + "'!"),
			(current.type == Token::Type::endFile ? previous : current),
			ErrorCode::syx
		);
	}
	inline void Compiler::emitOperation(ByteCode code) {
		program -> instructions.push_back(code);
	}
	inline void Compiler::emitOperation(OPCode code) {
		program -> instructions.push_back({ code, { .index = 0 } });
	}

	void Compiler::emitReturn() {
		emitOperation(
			{ OPCode::RET, { .index = 0 } }
		);
	}
	void Compiler::emitHalt() {
		emitOperation(
			{ OPCode::HLT, { .index = 0 } }
		);
	}

	Program * Compiler::compile(SourceCode * source) {
		if (!source) return nullptr;
		currentUnit = source -> main;
		if (!currentUnit) return nullptr;
		tokens = source -> main -> tokens;
		if (!tokens) return nullptr;
		program = new Program();

		advance();
		expression();

		emitReturn();
		emitHalt();
	}

}

#endif
