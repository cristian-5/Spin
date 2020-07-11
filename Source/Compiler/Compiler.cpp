
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

#include "../Utility/Converter.hpp"

namespace Spin {

	const Dictionary<Token::Type, Compiler::ParseRule> Compiler::rules = {
	
		{ Token::Type::openParenthesis, { & Compiler::grouping, nullptr, Precedence::none } },

		{ Token::Type::exclamationMark, { & Compiler::unary, nullptr, Precedence::none } },
		{ Token::Type::tilde, { & Compiler::unary, nullptr, Precedence::term } },
		{ Token::Type::minus, { & Compiler::unary, & Compiler::binary, Precedence::term } },
		{ Token::Type::plus, { & Compiler::unary, & Compiler::binary, Precedence::term } },
		{ Token::Type::slash, { nullptr, & Compiler::binary, Precedence::factor } },
		{ Token::Type::modulus, { nullptr, & Compiler::binary, Precedence::factor } },
		{ Token::Type::star, { nullptr, & Compiler::binary, Precedence::factor } },

		{ Token::Type::AND, { nullptr, & Compiler::binary, Precedence::logicAND } },
		{ Token::Type::OR, { nullptr, & Compiler::binary, Precedence::logicOR } },

		{ Token::Type::intLiteral, { & Compiler::integerLiteral, nullptr, Precedence::none } },
		{ Token::Type::realLiteral, { & Compiler::realLiteral, nullptr, Precedence::none } },
		{ Token::Type::imaginaryLiteral, { & Compiler::imaginaryLiteral, nullptr, Precedence::none } },
		{ Token::Type::stringLiteral, { & Compiler::stringLiteral, nullptr, Precedence::none } },
		{ Token::Type::charLiteral, { & Compiler::characterLiteral, nullptr, Precedence::none } },
		{ Token::Type::boolLiteral, { & Compiler::booleanLiteral, nullptr, Precedence::none } },

		{ Token::Type::inequality, { nullptr, & Compiler::binary, Precedence::equality } },
		{ Token::Type::equality, { nullptr, & Compiler::binary, Precedence::equality } },

		{ Token::Type::major, { nullptr, & Compiler::binary, Precedence::comparison } },
		{ Token::Type::majorEqual, { nullptr, & Compiler::binary, Precedence::comparison } },
		{ Token::Type::minor, { nullptr, & Compiler::binary, Precedence::comparison } },
		{ Token::Type::minorEqual, { nullptr, & Compiler::binary, Precedence::comparison } },

	};

	const Dictionary<Unary, Type> Compiler::prefix = {
		{ compose(Token::Type::exclamationMark, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::exclamationMark, Type::BooleanType), Type::BooleanType },
		{ compose(Token::Type::minus, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::minus, Type::RealType), Type::RealType },
		{ compose(Token::Type::minus, Type::ImaginaryType), Type::ImaginaryType },
		{ compose(Token::Type::plus, Type::CharacterType), Type::CharacterType },
		{ compose(Token::Type::plus, Type::ByteType), Type::ByteType },
		{ compose(Token::Type::plus, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::plus, Type::RealType), Type::RealType },
		{ compose(Token::Type::plus, Type::ImaginaryType), Type::ImaginaryType },
		{ compose(Token::Type::tilde, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::tilde, Type::ByteType), Type::ByteType },
	};
	const Dictionary<Binary, Type> Compiler::infix = {
		// # B # ------------------------------------------------------------- # Composing Short Circuit #
		{ compose(Token::Type::AND, Type::BooleanType, Type::BooleanType), Type::BooleanType },
		{ compose(Token::Type::OR, Type::BooleanType, Type::BooleanType), Type::BooleanType },
		// # & # ------------------------------------------------------------- # Composing Bitwise AND #
		{ compose(Token::Type::ampersand, Type::CharacterType, Type::CharacterType), Type::CharacterType },
		{ compose(Token::Type::ampersand, Type::ByteType, Type::ByteType), Type::ByteType },
		{ compose(Token::Type::ampersand, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		// # | # ------------------------------------------------------------- # Composing Bitwise OR #
		{ compose(Token::Type::pipe, Type::CharacterType, Type::CharacterType), Type::CharacterType },
		{ compose(Token::Type::pipe, Type::ByteType, Type::ByteType), Type::ByteType },
		{ compose(Token::Type::pipe, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		// # $ # ------------------------------------------------------------- # Composing Bitwise XOR #
		{ compose(Token::Type::dollar, Type::CharacterType, Type::CharacterType), Type::CharacterType },
		{ compose(Token::Type::dollarEqual, Type::ByteType, Type::ByteType), Type::ByteType },
		{ compose(Token::Type::dollarEqual, Type::IntegerType, Type::IntegerType), Type::IntegerType },
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
		{ compose(Token::Type::plus, Type::StringType, Type::StringType), Type::StringType },
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
		{ compose(Token::Type::star, Type::CharacterType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::star, Type::CharacterType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::star, Type::CharacterType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::star, Type::ByteType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::star, Type::ByteType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::star, Type::ByteType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::star, Type::IntegerType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::star, Type::IntegerType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::star, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::star, Type::IntegerType, Type::RealType), Type::RealType },
		{ compose(Token::Type::star, Type::IntegerType, Type::ImaginaryType), Type::ImaginaryType },
		{ compose(Token::Type::star, Type::RealType, Type::IntegerType), Type::RealType },
		{ compose(Token::Type::star, Type::RealType, Type::RealType), Type::RealType },
		{ compose(Token::Type::star, Type::RealType, Type::ImaginaryType), Type::ImaginaryType },
		{ compose(Token::Type::star, Type::ImaginaryType, Type::IntegerType), Type::ImaginaryType },
		{ compose(Token::Type::star, Type::ImaginaryType, Type::RealType), Type::ImaginaryType },
		{ compose(Token::Type::star, Type::ImaginaryType, Type::ImaginaryType), Type::RealType },
		// # / # ------------------------------------------------------------- # Composing Division #
		{ compose(Token::Type::slash, Type::CharacterType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::slash, Type::CharacterType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::slash, Type::CharacterType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::slash, Type::ByteType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::slash, Type::ByteType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::slash, Type::ByteType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::slash, Type::IntegerType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::slash, Type::IntegerType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::slash, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::slash, Type::IntegerType, Type::RealType), Type::RealType },
		{ compose(Token::Type::slash, Type::IntegerType, Type::ImaginaryType), Type::ImaginaryType },
		{ compose(Token::Type::slash, Type::RealType, Type::IntegerType), Type::RealType },
		{ compose(Token::Type::slash, Type::RealType, Type::RealType), Type::RealType },
		{ compose(Token::Type::slash, Type::RealType, Type::ImaginaryType), Type::ImaginaryType },
		{ compose(Token::Type::slash, Type::ImaginaryType, Type::IntegerType), Type::ImaginaryType },
		{ compose(Token::Type::slash, Type::ImaginaryType, Type::RealType), Type::ImaginaryType },
		{ compose(Token::Type::slash, Type::ImaginaryType, Type::ImaginaryType), Type::RealType },
		// # % # ------------------------------------------------------------- # Composing Modulus #
		{ compose(Token::Type::modulus, Type::CharacterType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::modulus, Type::CharacterType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::modulus, Type::CharacterType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::modulus, Type::ByteType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::modulus, Type::ByteType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::modulus, Type::ByteType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::modulus, Type::IntegerType, Type::CharacterType), Type::IntegerType },
		{ compose(Token::Type::modulus, Type::IntegerType, Type::ByteType), Type::IntegerType },
		{ compose(Token::Type::modulus, Type::IntegerType, Type::IntegerType), Type::IntegerType },
	};

	void Compiler::booleanLiteral() {
		if (previous.lexeme[0] == 't') {
			emitOperation(OPCode::PST);
		} else emitOperation(OPCode::PSF);
		typeStack.push(Type::BooleanType);
	}
	void Compiler::characterLiteral() {
		String lexeme = previous.lexeme.substr(
			1, previous.lexeme.length() - 2
		);
		Character literal = Converter::escapeChar(lexeme);
		emitOperation(
			{ OPCode::CNS, { .value = { .byte = ((Byte)(literal)) } } }
		);
		typeStack.push(Type::CharacterType);
	}
	void Compiler::stringLiteral() {
		String literal = previous.lexeme.substr(
			1, previous.lexeme.length() - 2
		);
		literal = Converter::escapeString(literal);
		const Pointer ptr = new String(literal);
		emitOperation(
			{ OPCode::CNS, { .value = { .pointer = ptr } } }
		);
		emitObject(ptr, Type::StringType);
		typeStack.push(Type::StringType);
	}
	void Compiler::imaginaryLiteral() {
		Real literal = Converter::stringToImaginary(
			previous.lexeme
		);
		emitOperation(
			{ OPCode::CNS, { .value = { .real = literal } } }
		);
		typeStack.push(Type::ImaginaryType);
	}
	void Compiler::realLiteral() {
		Real literal = Converter::stringToReal(
			previous.lexeme
		);
		emitOperation(
			{ OPCode::CNS, { .value = { .real = literal } } }
		);
		typeStack.push(Type::RealType);
	}
	void Compiler::realIdioms() {
		if (previous.lexeme[0] == 'i') {
			emitOperation(OPCode::PSI);
		} else emitOperation(OPCode::PSU);
		typeStack.push(Type::RealType);
	}
	void Compiler::integerLiteral() {
		Int64 literal = Converter::stringToInteger(
			previous.lexeme
		);
		emitOperation(
			{ OPCode::CNS, { .value = { .integer = literal } } }
		);
		typeStack.push(Type::IntegerType);
	}

	void Compiler::expression() {
		try { parsePrecedence(Precedence::assignment); }
		catch (Program::Error & e) { throw; }
	}
	void Compiler::statement() {
		if (match(Token::Type::printKeywork)) {
			try { printStatement(); }
			catch (Program::Error & e) { throw; }
		} else {
			try { expressionStatement(); }
			catch (Program::Error & e) { throw; }
		}
	}
	void Compiler::declaration() {
		try { statement(); }
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
				token, ErrorCode::typ
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
			case  Token::Type::ampersand: emitOperation({ OPCode::BWA, { .types = types } }); break;
			case       Token::Type::pipe: emitOperation({ OPCode::BWO, { .types = types } }); break;
			case        Token::Type::AND: emitOperation(OPCode::AND); break;
			case               Token::OR: emitOperation(OPCode::ORR); break;
			default: break;
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
				token, ErrorCode::typ
			);
		}
		switch (token.type) {
			case Token::Type::exclamationMark: emitOperation({ OPCode::NOT, { .type = type } }); break;
			case Token::Type::minus: emitOperation({ OPCode::NEG, { .type = type } }); break;
			case Token::Type::tilde: emitOperation({ OPCode::INV, { .type = type } }); break;
			default: break;
		}
		typeStack.push(search -> second);
	}

	void Compiler::expressionStatement() {
		try {
			expression();
			consume(Token::Type::semicolon, ";");
		} catch (Program::Error & e) { throw; }
		emitOperation(OPCode::POP);
	}
	void Compiler::printStatement() {
		const Token token = previous;
		try {
			expression();
			consume(Token::Type::semicolon, ";");
		} catch (Program::Error & e) { throw; }
		Type type = typeStack.pop();
		if (type > Type::StringType) {
			throw Program::Error(
				currentUnit,
				"Print statement doesn't support any operand of type '" +
				Converter::typeToString(type) + "'!",
				token, ErrorCode::typ
			);
		}
		emitOperation({ OPCode::PRN, { .type = type } });
		emitOperation(OPCode::NLN);
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
		while (precedence <= getRule(current.type).precedence) {
			advance();
			ParseFunction infixRule = getRule(previous.type).infix;
			try { (this ->* infixRule)(); }
			catch (Program::Error & e) { throw; }
		}
	}

	Compiler::ParseRule Compiler::getRule(Token::Type token) {
		auto search = rules.find(token);
		if (search == rules.end()) {
			return { nullptr, nullptr, Precedence::none };
		}
		return search -> second;
	}

	inline Boolean Compiler::match(Token::Type type) {
		if (check(Token::Type::endFile)) return check(type);
		if (!check(type)) return false;
		advance(); return true;
	}
	inline Boolean Compiler::check(Token::Type type) {
		return current.type == type;
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
		program -> instructions.push_back({ code, {
			.value = { .integer = 0 } }
		});
	}
	inline void Compiler::emitObject(Pointer ptr, Type type) {
		program -> objects.push_back({ ptr, type });
	}

	void Compiler::emitReturn() {
		emitOperation(OPCode::RET);
	}
	void Compiler::emitHalt() {
		emitOperation(OPCode::HLT);
	}

	Program * Compiler::compile(SourceCode * source) {
		if (!source) return nullptr;
		currentUnit = source -> main;
		if (!currentUnit) return nullptr;
		tokens = source -> main -> tokens;
		if (!tokens) return nullptr;
		program = new Program();

		try {
			advance();
			consume(Token::Type::beginFile, "Begin File");
			while (!match(Token::Type::endFile)) declaration();
			emitHalt();
		} catch (Program::Error & e) { throw; }

		return program;
	}

}

#endif
