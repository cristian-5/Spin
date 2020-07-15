
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

#define rethrow(A) try { A; } catch (Program::Error & e) { throw; }

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

		{ Token::Type::symbol, { & Compiler::identifier, nullptr, Precedence::none } },

		{ Token::Type::ampersand, { nullptr, & Compiler::binary, Precedence::bitwiseAND } },
		{ Token::Type::pipe, { nullptr, & Compiler::binary, Precedence::bitwiseOR } },
		{ Token::Type::dollar, { nullptr, & Compiler::binary, Precedence::bitwiseXOR } },

		{ Token::Type::AND, { nullptr, & Compiler::logicAND, Precedence::logicAND } },
		{ Token::Type::OR, { nullptr, & Compiler::logicOR, Precedence::logicOR } },

		{ Token::Type::intLiteral, { & Compiler::integerLiteral, nullptr, Precedence::none } },
		{ Token::Type::realLiteral, { & Compiler::realLiteral, nullptr, Precedence::none } },
		{ Token::Type::imaginaryLiteral, { & Compiler::imaginaryLiteral, nullptr, Precedence::none } },
		{ Token::Type::stringLiteral, { & Compiler::stringLiteral, nullptr, Precedence::none } },
		{ Token::Type::charLiteral, { & Compiler::characterLiteral, nullptr, Precedence::none } },
		{ Token::Type::boolLiteral, { & Compiler::booleanLiteral, nullptr, Precedence::none } },

		{ Token::Type::realIdiom, { & Compiler::realIdioms, nullptr, Precedence::none } },

		{ Token::Type::inequality, { nullptr, & Compiler::binary, Precedence::equality } },
		{ Token::Type::equality, { nullptr, & Compiler::binary, Precedence::equality } },

		{ Token::Type::major, { nullptr, & Compiler::binary, Precedence::comparison } },
		{ Token::Type::majorEqual, { nullptr, & Compiler::binary, Precedence::comparison } },
		{ Token::Type::minor, { nullptr, & Compiler::binary, Precedence::comparison } },
		{ Token::Type::minorEqual, { nullptr, & Compiler::binary, Precedence::comparison } },

	};

	const Dictionary<Unary, Type> Compiler::prefix = {
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
		// # == # ------------------------------------------------------------- # Composing Comparison #
		{ compose(Token::Type::equality, Type::BooleanType, Type::BooleanType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::CharacterType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::CharacterType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::CharacterType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::ByteType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::ByteType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::ByteType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::IntegerType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::IntegerType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::IntegerType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::IntegerType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::RealType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::RealType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::ImaginaryType, Type::ImaginaryType), Type::BooleanType },
		{ compose(Token::Type::equality, Type::StringType, Type::StringType), Type::BooleanType },
		// # != # ------------------------------------------------------------- # Composing Comparison #
		{ compose(Token::Type::inequality, Type::BooleanType, Type::BooleanType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::CharacterType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::CharacterType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::CharacterType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::ByteType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::ByteType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::ByteType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::IntegerType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::IntegerType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::IntegerType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::IntegerType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::RealType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::RealType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::ImaginaryType, Type::ImaginaryType), Type::BooleanType },
		{ compose(Token::Type::inequality, Type::StringType, Type::StringType), Type::BooleanType },
		// # < # ------------------------------------------------------------- # Composing Comparison #
		{ compose(Token::Type::minor, Type::BooleanType, Type::BooleanType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::CharacterType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::CharacterType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::CharacterType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::ByteType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::ByteType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::ByteType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::IntegerType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::IntegerType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::IntegerType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::IntegerType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::RealType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::RealType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::minor, Type::ImaginaryType, Type::ImaginaryType), Type::BooleanType },
		// # <= # ------------------------------------------------------------- # Composing Comparison #
		{ compose(Token::Type::minorEqual, Type::BooleanType, Type::BooleanType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::CharacterType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::CharacterType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::CharacterType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::ByteType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::ByteType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::ByteType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::IntegerType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::IntegerType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::IntegerType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::IntegerType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::RealType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::RealType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::minorEqual, Type::ImaginaryType, Type::ImaginaryType), Type::BooleanType },
		// # > # ------------------------------------------------------------- # Composing Comparison #
		{ compose(Token::Type::major, Type::BooleanType, Type::BooleanType), Type::BooleanType },
		{ compose(Token::Type::major, Type::CharacterType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::major, Type::CharacterType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::major, Type::CharacterType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::major, Type::ByteType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::major, Type::ByteType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::major, Type::ByteType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::major, Type::IntegerType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::major, Type::IntegerType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::major, Type::IntegerType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::major, Type::IntegerType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::major, Type::RealType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::major, Type::RealType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::major, Type::ImaginaryType, Type::ImaginaryType), Type::BooleanType },
		// # >= # ------------------------------------------------------------- # Composing Comparison #
		{ compose(Token::Type::majorEqual, Type::BooleanType, Type::BooleanType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::CharacterType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::CharacterType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::CharacterType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::ByteType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::ByteType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::ByteType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::IntegerType, Type::CharacterType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::IntegerType, Type::ByteType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::IntegerType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::IntegerType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::RealType, Type::IntegerType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::RealType, Type::RealType), Type::BooleanType },
		{ compose(Token::Type::majorEqual, Type::ImaginaryType, Type::ImaginaryType), Type::BooleanType },
	};

	const Dictionary<Types, Boolean> Compiler::implicitCast = {
		// Basic Types:
		{ compose(Type::CharacterType, Type::ByteType), false },
		{ compose(Type::CharacterType, Type::IntegerType), true },
		{ compose(Type::ByteType, Type::CharacterType), false },
		{ compose(Type::ByteType, Type::IntegerType), true },
		{ compose(Type::IntegerType, Type::CharacterType), true },
		{ compose(Type::IntegerType, Type::ByteType), true },
		{ compose(Type::IntegerType, Type::RealType), true },
		{ compose(Type::RealType, Type::IntegerType), true },
		// Basic Objects:
		{ compose(Type::CharacterType, Type::StringType), true },
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
		rethrow(parsePrecedence(Precedence::assignment));
	}
	void Compiler::statement() {
		if (match(Token::ifKeyword)) {
			rethrow(ifStatement());
		} else if (match(Token::Type::printKeywork)) {
			rethrow(printStatement());
		} else if (match(Token::Type::whileKeyword)) {
			rethrow(whileStatement());
		} else if (match(Token::Type::untilKeyword)) {
			rethrow(untilStatement());
		} else if (match(Token::Type::openBrace)) {
			beginScope();
			rethrow(block());
			endScope();
		} else rethrow(expressionStatement());
	}
	void Compiler::declaration() {
		if (match(Token::Type::basicType)) {
			rethrow(variable());
		} else rethrow(statement());
	}
	void Compiler::variable() {

		Type typeA = Converter::stringToType(previous.lexeme);
		rethrow(consume(Token::Type::symbol, "identifier"));
		const String id = previous.lexeme;

		typeStack.push(typeA);

		if (scopeDepth) local();
		else global();

		typeStack.decrease();

		if (match(Token::Type::equal)) {
			const Token token = previous;
			rethrow(expression());
			Type typeB = typeStack.pop();
			if (typeA != typeB) {
				// Since we're working with B -> A:
				auto casting = implicitCast.find(
					runtimeCompose(typeB, typeA)
				);
				if (casting == implicitCast.end()) {
					throw Program::Error(
						currentUnit,
						"Assignment operator '=' doesn't support implicit cast of '" +
						Converter::typeToString(typeB) + "' in '" +
						Converter::typeToString(typeA) + "'!",
						token, ErrorCode::lgc
					);
				}
				// If needed produce a CAST:
				if (casting -> second) {
					emitOperation({
						OPCode::CST,
						{ .types = runtimeCompose(typeB, typeA) }
					});
				}
			}
		} else {
			switch (typeA) {
				case   Type::BooleanType: emitOperation(OPCode::PSF); break;
				case Type::CharacterType: 
				case      Type::ByteType: emitOperation({
					OPCode::CNS, { .value = { .byte = 0 } }
				}); break;
				case   Type::IntegerType: emitOperation({
					OPCode::CNS, { .value = { .integer = 0 } }
				}); break;
				case      Type::RealType:
				case Type::ImaginaryType: emitOperation({
					OPCode::CNS, { .value = { .real = 0.0 } }
				}); break;
				case    Type::StringType: {
					const Pointer ptr = new String();
					emitObject(ptr, Type::StringType);
					emitOperation({
						OPCode::CNS, { .value = { .pointer = ptr } }
					});
				} break;
				default: emitOperation({
					OPCode::CNS, { .value = { .integer = 0 } }
				}); break;
			}
		}
	
		// If the scope is 0, we need to emitGlobal
		// but if it's a local it will already be
		// on the stack frame.
		if (!scopeDepth) {
			emitGlobal();
			emitOperation({
				OPCode::SGB,
				{ .index = (globals.size() - 1) }
			});
			emitOperation(OPCode::POP);
			// Globals sentinel:
			globals[id].ready = true;
		} else {
			// Locals sentinel:
			locals[locals.size() - 1].ready = true;
		}

		rethrow(consume(Token::Type::semicolon, ";"));
	}
	void Compiler::local() {
		const String name = previous.lexeme;
		for (Local local : locals) {
			if (local.ready && local.depth < scopeDepth) {
				break;
			}
			if (local.name == name) {
				throw Program::Error(
					currentUnit,
					"Variable redefinition! The identifier '" +
					name + "' was already declared in the current scope!",
					previous, ErrorCode::lgc
				);
			}
		}
		locals.push_back({ name, scopeDepth, typeStack.top(), false });
	}
	void Compiler::global() {
		const String id = previous.lexeme;
		if (globals.find(id) != globals.end()) {
			throw Program::Error(
				currentUnit,
				"Variable redefinition! The identifier '" +
				id + "' was already declared with type '" +
				Converter::typeToString(globals.at(id).type) +
				"' in the current global scope!",
				previous, ErrorCode::lgc
			);
		}
		globals.insert({ id, { globalIndex++, typeStack.top(), false } });
	}

	void Compiler::identifier() {
		OPCode GET, SET;
		Local local;
		SizeType argument;
		rethrow(argument = resolve(previous.lexeme, local));
		Type typeA = local.type;
		if (argument != -1) {
			GET = OPCode::GLC;
			SET = OPCode::SLC;
		} else {
			GET = OPCode::GGB;
			SET = OPCode::SGB;
			auto search = globals.find(previous.lexeme);
			if (search == globals.end()) {
				throw Program::Error(
					currentUnit,
					"Unexpected identifier '" +
					previous.lexeme + "'!",
					previous, ErrorCode::lgc
				);
			}
			if (!(search -> second.ready)) {
				throw Program::Error(
					currentUnit,
					"Cannot access local variable '" + previous.lexeme +
					"' in it's own definition statement!",
					previous, ErrorCode::lgc
				);
			}
			typeA = search -> second.type;
			argument = search -> second.index;
		}
		const Boolean canAssign = assignmentStack.top();
		if (canAssign && match(Token::Type::equal)) {
			const Token token = previous;
			rethrow(expression());
			Type typeB = typeStack.pop();
			if (typeA != typeB) {
				// Since we're working with B -> A:
				auto casting = implicitCast.find(
					runtimeCompose(typeB, typeA)
				);
				if (casting == implicitCast.end()) {
					throw Program::Error(
						currentUnit,
						"Assignment operator '=' doesn't support implicit cast of '" +
						Converter::typeToString(typeB) + "' in '" +
						Converter::typeToString(typeA) + "'!",
						token, ErrorCode::lgc
					);
				}
				// If needed produce a CAST:
				if (casting -> second) {
					emitOperation({
						OPCode::CST,
						{ .types = runtimeCompose(typeB, typeA) }
					});
				}
			}
			emitOperation({ SET, { .index = (UInt64)argument } });
		} else {
			emitOperation({ GET, { .index = (UInt64)argument } });
		}
		typeStack.push(typeA);
	}
	void Compiler::block() {
		while (!check(Token::Type::closeBrace) &&
			   !check(Token::Type::endFile)) {
			declaration();
		}
		rethrow(consume(Token::Type::closeBrace, "}"));
	}

	void Compiler::logicAND() {
		const Token token = previous;
		Type typeA = typeStack.pop();
		const SizeType endJMP = emitJMP(OPCode::JAF);
		emitOperation(OPCode::POP);
		rethrow(parsePrecedence(Precedence::logicAND));
		Type typeB = typeStack.pop();
		if (typeA != Type::BooleanType || typeB != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Binary operator '&&' doesn't support operands of type '" +
				Converter::typeToString(typeA) + "' and '" +
				Converter::typeToString(typeB) + "'!",
				token, ErrorCode::typ
			);
		}
		patchJMP(endJMP);
	}
	void Compiler::logicOR() {
		const Token token = previous;
		Type typeA = typeStack.pop();
		const SizeType elseJMP = emitJMP(OPCode::JAF);
		const SizeType endJMP = emitJMP(OPCode::JMP);
		patchJMP(elseJMP);
		emitOperation(OPCode::POP);
		rethrow(parsePrecedence(Precedence::logicOR));
		Type typeB = typeStack.pop();
		if (typeA != Type::BooleanType || typeB != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Binary operator '||' doesn't support operands of type '" +
				Converter::typeToString(typeA) + "' and '" +
				Converter::typeToString(typeB) + "'!",
				token, ErrorCode::typ
			);
		}
		patchJMP(endJMP);
	}

	void Compiler::grouping() {
		rethrow(
			expression();
			consume(Token::Type::closeParenthesis, ")");
		);
	}
	void Compiler::binary() {
		const Token token = previous;
		ParseRule rule = getRule(token.type);
		Type typeA = typeStack.pop();
		rethrow(parsePrecedence((Precedence)(rule.precedence + 1)));
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
			case Token::Type::slash:
				emitOperation({ OPCode::DIV, { .types = types } });
				emitException(Program::Error(
					currentUnit,
					"Binary operator '/' threw division by zero!",
					token, ErrorCode::evl
				));
			break;
			case Token::Type::modulus:
				emitOperation({ OPCode::DIV, { .types = types } });
				emitException(Program::Error(
					currentUnit,
					"Binary operator '%' threw division by zero!",
					token, ErrorCode::evl
				));
			break;
			case   Token::Type::equality: emitOperation({ OPCode::EQL, { .types = types } }); break;
			case Token::Type::inequality: emitOperation({ OPCode::NEQ, { .types = types } }); break;
			case      Token::Type::major: emitOperation({ OPCode::GRT, { .types = types } }); break;
			case Token::Type::majorEqual: emitOperation({ OPCode::GEQ, { .types = types } }); break;
			case      Token::Type::minor: emitOperation({ OPCode::LSS, { .types = types } }); break;
			case Token::Type::minorEqual: emitOperation({ OPCode::LEQ, { .types = types } }); break;
			case  Token::Type::ampersand: emitOperation({ OPCode::BWA, { .types = types } }); break;
			case       Token::Type::pipe: emitOperation({ OPCode::BWO, { .types = types } }); break;
			default: break;
		}
		typeStack.push(search -> second);
	}
	void Compiler::unary() {
		const Token token = previous;
		rethrow(parsePrecedence(Precedence::unary));
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
		rethrow(
			expression();
			consume(Token::Type::semicolon, ";");
		);
		emitOperation(OPCode::POP);
	}
	void Compiler::printStatement() {
		const Token token = previous;
		rethrow(
			expression();
			consume(Token::Type::semicolon, ";");
		);
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
	void Compiler::ifStatement() {
		const Token token = previous;
		rethrow(
			consume(Token::Type::openParenthesis, "(");
			expression();
			consume(Token::Type::closeParenthesis, ")");
		);
		if (typeStack.pop() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'if' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType thenJMP = emitJMP(OPCode::JIF);
		rethrow(statement());
		const SizeType elseJMP = emitJMP(OPCode::JMP);
		patchJMP(thenJMP);
		if (match(Token::Type::elseKeyword)) rethrow(statement());
		patchJMP(elseJMP);
	}
	void Compiler::whileStatement() {
		const SizeType loopStart = program -> instructions.size();
		const Token token = previous;
		rethrow(
			consume(Token::Type::openParenthesis, "(");
			expression();
			consume(Token::Type::closeParenthesis, ")");
		);
		if (typeStack.pop() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'while' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType exitJMP = emitJMP(OPCode::JIF);
		rethrow(statement());
		emitJMB(loopStart);
		patchJMP(exitJMP);
	}
	void Compiler::untilStatement() {
		const SizeType loopStart = program -> instructions.size();
		const Token token = previous;
		rethrow(
			consume(Token::Type::openParenthesis, "(");
			expression();
			consume(Token::Type::closeParenthesis, ")");
		);
		if (typeStack.pop() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'while' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType exitJMP = emitJMP(OPCode::JIT);
		rethrow(statement());
		emitJMB(loopStart);
		patchJMP(exitJMP);
	}

	SizeType Compiler::resolve(String & name, Local & local) {
		// Resolve local variable:
		for (Int64 i = locals.size() - 1; i >= 0; i -= 1) {
			if (locals[i].name == name) {
				if (!locals[i].ready) {
					throw Program::Error(
						currentUnit,
						"Cannot access local variable '" + locals[i].name +
						"' in it's own definition statement!",
						previous, ErrorCode::lgc
					);
				}
				local = locals[i];
				return i;
			}
		}
		return -1;
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
		Boolean canAssign = (precedence <= Precedence::assignment);
		assignmentStack.push(canAssign);
		rethrow((this ->* prefixRule)());
		while (precedence <= getRule(current.type).precedence) {
			advance();
			ParseFunction infixRule = getRule(previous.type).infix;
			rethrow((this ->* infixRule)());
		}
		if (canAssign && match(Token::Type::equal)) {
			throw Program::Error(
				currentUnit,
				"Found invalid assignment target '" +
				previous.lexeme + "'!",
				previous, ErrorCode::lgc
			);
		}
		assignmentStack.decrease();
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
	inline void Compiler::emitException(Program::Error error) {
		program -> errors.insert({
			program -> instructions.size() - 1,
			error
		});
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
	inline void Compiler::emitGlobal(Value value) {
		program -> instructions.push_back(
			{ OPCode::GLB, { .value = value } }
		);
	}
	inline void Compiler::emitJMB(SizeType jmb) {
		jmb = program -> instructions.size() - jmb + 1;
		emitOperation({ OPCode::JMB, { .index = jmb } });
	}
	inline SizeType Compiler::emitJMP(OPCode code) {
		const SizeType count = program -> instructions.size();
		emitOperation(code);
		return count;
	}
	inline void Compiler::patchJMP(SizeType jmp) {
		const SizeType jump = (program -> instructions.size() - jmp);
		program -> instructions[jmp].as.index = jump - 1;
	}
	inline void Compiler::beginScope() {
		scopeDepth += 1;
	}
	inline void Compiler::endScope() {
		scopeDepth -= 1;
		// TODO: Add a POP with operand to pop n values.
		while (locals.size() > 0 &&
			   locals[locals.size() - 1].depth > scopeDepth) {
			emitOperation(OPCode::POP);
			locals.pop_back();
		}
	}

	void Compiler::emitReturn() {
		emitOperation(OPCode::RET);
	}
	void Compiler::emitHalt() {
		emitOperation(OPCode::HLT);
	}

	void Compiler::reset() {
		// Reset Globals:
		globals.clear();
		globalIndex = 0;
		// Reset Assignment Stack:
		assignmentStack.clear();
		// Reset Locals:
		locals.clear();
		scopeDepth = 0;
	}

	Program * Compiler::compile(SourceCode * source) {
		if (!source) return nullptr;
		currentUnit = source -> main;
		if (!currentUnit) return nullptr;
		tokens = source -> main -> tokens;
		if (!tokens) return nullptr;
		program = new Program();

		reset();

		rethrow(
			advance();
			consume(Token::Type::beginFile, "Begin File");
			while (!match(Token::Type::endFile)) declaration();
			emitHalt();
		);

		reset();

		return program;
	}

}

#undef rethrow

#endif
