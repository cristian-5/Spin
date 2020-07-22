
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
#include "../Types/Complex.hpp"

#define rethrow(A) try { A; } catch (Program::Error & e) { throw; }

namespace Spin {

	const Dictionary<Token::Type, Compiler::ParseRule> Compiler::rules = {
	
		{ Token::Type::openParenthesis, { & Compiler::grouping, nullptr, Precedence::none } },

		{ Token::Type::questionMark, { nullptr, & Compiler::ternary, Precedence::assignment } },

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
		{ compose(Token::Type::minus, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::plus, Type::CharacterType), Type::CharacterType },
		{ compose(Token::Type::plus, Type::ByteType), Type::ByteType },
		{ compose(Token::Type::plus, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::plus, Type::RealType), Type::RealType },
		{ compose(Token::Type::plus, Type::ImaginaryType), Type::ImaginaryType },
		{ compose(Token::Type::plus, Type::ComplexType), Type::ComplexType },
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

		{ compose(Token::Type::plus, Type::IntegerType, Type::ImaginaryType), Type::ComplexType },
		{ compose(Token::Type::plus, Type::IntegerType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::plus, Type::RealType, Type::ImaginaryType), Type::ComplexType },
		{ compose(Token::Type::plus, Type::RealType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::plus, Type::ImaginaryType, Type::IntegerType), Type::ComplexType },
		{ compose(Token::Type::plus, Type::ImaginaryType, Type::RealType), Type::ComplexType },
		{ compose(Token::Type::plus, Type::ImaginaryType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::plus, Type::ComplexType, Type::IntegerType), Type::ComplexType },
		{ compose(Token::Type::plus, Type::ComplexType, Type::RealType), Type::ComplexType },
		{ compose(Token::Type::plus, Type::ComplexType, Type::ImaginaryType), Type::ComplexType },
		{ compose(Token::Type::plus, Type::ComplexType, Type::ComplexType), Type::ComplexType },

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

		{ compose(Token::Type::minus, Type::IntegerType, Type::ImaginaryType), Type::ComplexType },
		{ compose(Token::Type::minus, Type::IntegerType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::minus, Type::RealType, Type::ImaginaryType), Type::ComplexType },
		{ compose(Token::Type::minus, Type::RealType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::minus, Type::ImaginaryType, Type::IntegerType), Type::ComplexType },
		{ compose(Token::Type::minus, Type::ImaginaryType, Type::RealType), Type::ComplexType },
		{ compose(Token::Type::minus, Type::ImaginaryType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::minus, Type::ComplexType, Type::IntegerType), Type::ComplexType },
		{ compose(Token::Type::minus, Type::ComplexType, Type::RealType), Type::ComplexType },
		{ compose(Token::Type::minus, Type::ComplexType, Type::ImaginaryType), Type::ComplexType },
		{ compose(Token::Type::minus, Type::ComplexType, Type::ComplexType), Type::ComplexType },
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

		{ compose(Token::Type::star, Type::IntegerType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::star, Type::RealType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::star, Type::ImaginaryType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::star, Type::ComplexType, Type::IntegerType), Type::ComplexType },
		{ compose(Token::Type::star, Type::ComplexType, Type::RealType), Type::ComplexType },
		{ compose(Token::Type::star, Type::ComplexType, Type::ImaginaryType), Type::ComplexType },
		{ compose(Token::Type::star, Type::ComplexType, Type::ComplexType), Type::ComplexType },
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

		{ compose(Token::Type::slash, Type::IntegerType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::slash, Type::RealType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::slash, Type::ImaginaryType, Type::ComplexType), Type::ComplexType },
		{ compose(Token::Type::slash, Type::ComplexType, Type::IntegerType), Type::ComplexType },
		{ compose(Token::Type::slash, Type::ComplexType, Type::RealType), Type::ComplexType },
		{ compose(Token::Type::slash, Type::ComplexType, Type::ImaginaryType), Type::ComplexType },
		{ compose(Token::Type::slash, Type::ComplexType, Type::ComplexType), Type::ComplexType },
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
		{ compose(Type::IntegerType, Type::ComplexType), true },
		{ compose(Type::RealType, Type::ComplexType), true },
		{ compose(Type::ImaginaryType, Type::ComplexType), true },
		{ compose(Type::CharacterType, Type::StringType), true },
		{ compose(Type::ComplexType, Type::IntegerType), true },
		{ compose(Type::ComplexType, Type::RealType), true },
		{ compose(Type::ComplexType, Type::ImaginaryType), true },
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
		switch (current.type) {
			case       Token::ifKeyword: advance(); ifStatement(); break;
			case    Token::printKeywork: advance(); printStatement(); break;
			case     Token::procKeyword: advance(); procStatement(); break;
			case     Token::funcKeyword: advance(); funcStatement(); break;
			case      Token::forKeyword: advance(); forStatement(); break;
			case    Token::whileKeyword: advance(); whileStatement(); break;
			case    Token::untilKeyword: advance(); untilStatement(); break;
			case       Token::doKeyword: advance(); doWhileStatement(); break;
			case   Token::repeatKeyword: advance(); repeatUntilStatement(); break;
			case     Token::loopKeyword: advance(); loopStatement(); break;
			case    Token::breakKeyword: advance(); breakStatement(); break;
			case Token::continueKeyword: advance(); continueStatement(); break;
			case     Token::swapKeyword: advance(); swapStatement(); break;
			case       Token::openBrace: advance();
				beginScope();
				rethrow(block());
				endScope();
			break;
			default: rethrow(expressionStatement());
		}
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
				case   Type::ComplexType: {
					const Pointer ptr = new Complex();
					emitObject(ptr, Type::ComplexType);
					emitOperation({
						OPCode::CNS, { .value = { .pointer = ptr } }
					});
				} break;
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
		if (canAssign && matchAssignment()) {
			// Takes care of operation assignments:
			const Token token = previous;
			OPCode o = OPCode::RST;
			Token::Type t;
			switch (token.type) {
				case      Token::Type::plusEqual: o = OPCode::ADD; t = Token::Type::plus; break;
				case     Token::Type::minusEqual: o = OPCode::SUB; t = Token::Type::minus;break;
				case      Token::Type::pipeEqual: o = OPCode::BWO; t = Token::Type::pipe;break;
				case      Token::Type::starEqual: o = OPCode::MUL; t = Token::Type::star; break;
				case     Token::Type::slashEqual: o = OPCode::DIV; t = Token::Type::slash; break;
				case Token::Type::ampersandEqual: o = OPCode::BWA; t = Token::Type::ampersand; break;
				case   Token::Type::modulusEqual: o = OPCode::MOD; t = Token::Type::modulus; break;
				case    Token::Type::dollarEqual: o = OPCode::BWX; t = Token::Type::dollar; break;
				default: break;
			}
			// If we have an operation assignment:
			if (o != OPCode::RST) {
				// Get the item before mutation:
				emitOperation({ GET, { .index = (UInt64)argument } });
			}
			rethrow(expression());
			Type typeB = typeStack.pop();
			// If we have an operation assignment:
			if (o != OPCode::RST) {
				const Types types = runtimeCompose(typeA, typeB);
				auto search = infix.find(
					runtimeCompose(t, typeA, typeB)
				);
				if (search == infix.end()) {
					throw Program::Error(
						currentUnit,
						"Mutation assignment operator '" + token.lexeme +
						"' doesn't support operands of type '" +
						Converter::typeToString(typeA) + "' and '" +
						Converter::typeToString(typeB) + "'!",
						token, ErrorCode::typ
					);
				}
				emitOperation({ o, { .types = types } });
				// Emitting eventual exceptions for throwing operators.
				if (o == OPCode::DIV || o == OPCode::MOD) {
					emitException(Program::Error(
						currentUnit,
						"Mutation assignment operator '" + token.lexeme +
						"' threw division by zero!",
						token, ErrorCode::evl
					));
				}
				typeB = search -> second;
			}
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
		} else emitOperation({ GET, { .index = (UInt64)argument } });
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
		const SizeType endJMP = emitJMP(OPCode::JAT);
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
	void Compiler::ternary() {
		Token token = previous;
		ParseRule rule = getRule(token.type);
		if (typeStack.pop() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside ternary condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType thenJMP = emitJMP(OPCode::JIF);
		// Then Condition:
		rethrow(parsePrecedence((Precedence)(rule.precedence + 1)));
		Type typeA = typeStack.pop();
		// End then.
		const SizeType elseJMP = emitJMP(OPCode::JMP);
		patchJMP(thenJMP);

		rethrow(consume(Token::Type::colon, ":"));
		token = previous;

		// Else Condition:
		rethrow(parsePrecedence((Precedence)(rule.precedence + 1)));
		Type typeB = typeStack.pop();
		// End else.
		patchJMP(elseJMP);

		if (typeA != typeB) {
			throw Program::Error(
				currentUnit,
				"Ternary operator ' ? : ' doesn't support operands of non matching types '" +
				Converter::typeToString(typeA) + "' and '" +
				Converter::typeToString(typeB) + "'!",
				token, ErrorCode::typ
			);
		}

		typeStack.push(typeA);
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
		const SizeType tempJMP = emitRST();
		const SizeType cutPosition = sourcePosition();
		rethrow(statement());
		if (!match(Token::Type::elseKeyword)) {
			// If statement with only then:
			patchOP(tempJMP, OPCode::JIF);
			patchJMP(tempJMP);
		} else {
			// If statement with then and else:
			patchOP(tempJMP, OPCode::JIT);
			Array<ByteCode> thenCodes = cutCodes(cutPosition);
			rethrow(statement());
			const SizeType endJMP = emitJMP(OPCode::JMP);
			patchJMP(tempJMP);
			pasteCodes(thenCodes);
			patchJMP(endJMP);
		}
	}
	void Compiler::whileStatement() {
		const SizeType loopStart = sourcePosition();
		cycleScopes.push(scopeDepth);
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
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJMP(breakStack.pop().line);
		}
		while (!continueStack.isEmpty()) {
			if (continueStack.top().scope <= scopeDepth) break;
			const SizeType pos = continueStack.pop().line;
			patchOP(pos, OPCode::JMB);
			patchJMB(pos, loopStart);
		}
	}
	void Compiler::untilStatement() {
		const SizeType loopStart = sourcePosition();
		cycleScopes.push(scopeDepth);
		const Token token = previous;
		rethrow(
			consume(Token::Type::openParenthesis, "(");
			expression();
			consume(Token::Type::closeParenthesis, ")");
		);
		if (typeStack.pop() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'until' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType exitJMP = emitJMP(OPCode::JIT);
		rethrow(statement());
		emitJMB(loopStart);
		patchJMP(exitJMP);
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJMP(breakStack.pop().line);
		}
		while (!continueStack.isEmpty()) {
			if (continueStack.top().scope <= scopeDepth) break;
			const SizeType pos = continueStack.pop().line;
			patchOP(pos, OPCode::JMB);
			patchJMB(pos, loopStart);
		}
	}
	void Compiler::doWhileStatement() {
		const SizeType loopStart = sourcePosition();
		cycleScopes.push(scopeDepth);
		const Token token = previous;
		rethrow(
			statement();
			consume(Token::Type::whileKeyword, "while");
			consume(Token::Type::openParenthesis, "(");
		);
		while (!continueStack.isEmpty()) {
			if (continueStack.top().scope <= scopeDepth) break;
			const SizeType pos = continueStack.pop().line;
			patchOP(pos, OPCode::JMP);
			patchJMP(pos);
		}
		rethrow(
			expression();
			consume(Token::Type::closeParenthesis, ")");
			consume(Token::Type::semicolon, ";");
		);
		if (typeStack.pop() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'do while' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType exitJMP = emitJMP(OPCode::JIF);
		emitJMB(loopStart);
		patchJMP(exitJMP);
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJMP(breakStack.pop().line);
		}
	}
	void Compiler::repeatUntilStatement() {
		const SizeType loopStart = sourcePosition();
		cycleScopes.push(scopeDepth);
		const Token token = previous;
		rethrow(
			statement();
			consume(Token::Type::untilKeyword, "until");
			consume(Token::Type::openParenthesis, "(");
		);
		while (!continueStack.isEmpty()) {
			if (continueStack.top().scope <= scopeDepth) break;
			const SizeType pos = continueStack.pop().line;
			patchOP(pos, OPCode::JMP);
			patchJMP(pos);
		}
		rethrow(
			expression();
			consume(Token::Type::closeParenthesis, ")");
			consume(Token::Type::semicolon, ";");
		);
		if (typeStack.pop() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'repeat until' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType exitJMP = emitJMP(OPCode::JIT);
		emitJMB(loopStart);
		patchJMP(exitJMP);
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJMP(breakStack.pop().line);
		}
	}
	void Compiler::loopStatement() {
		const SizeType loopStart = sourcePosition();
		cycleScopes.push(scopeDepth);
		rethrow(statement());
		emitJMB(loopStart);
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJMP(breakStack.pop().line);
		}
		while (!continueStack.isEmpty()) {
			if (continueStack.top().scope <= scopeDepth) break;
			const SizeType pos = continueStack.pop().line;
			patchOP(pos, OPCode::JMB);
			patchJMB(pos, loopStart);
		}
	}
	void Compiler::forStatement() {
		beginScope();
		cycleScopes.push(scopeDepth);
		const Token token = previous;
		rethrow(consume(Token::Type::openParenthesis, "("));
		if (match(Token::Type::basicType)) {
			rethrow(variable());
			// Variable already consumes ';'.
		} else {
			rethrow(
				expression();
				emitOperation(OPCode::POP);
				consume(Token::Type::semicolon, ";");
			);
			typeStack.decrease();
		}
		const SizeType loopStart = sourcePosition();
		rethrow(expression());
		rethrow(consume(Token::Type::semicolon, ";"));
		if (typeStack.pop() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'for' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType exitJMP = emitJMP(OPCode::JIF);
		const SizeType cutExpression = sourcePosition();
		rethrow(expression());
		emitOperation(OPCode::POP);
		typeStack.decrease();
		rethrow(consume(Token::Type::closeParenthesis, ")"));
		const Array<ByteCode> codes = cutCodes(cutExpression);
		rethrow(statement()); // Body.
		while (!continueStack.isEmpty()) {
			if (continueStack.top().scope <= scopeDepth) break;
			const SizeType pos = continueStack.pop().line;
			patchOP(pos, OPCode::JMP);
			patchJMP(pos);
		}
		pasteCodes(codes); // Increment.
		emitJMB(loopStart);
		patchJMP(exitJMP);
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJMP(breakStack.pop().line);
		}
		endScope();
	}
	void Compiler::breakStatement() {
		const Token token = previous;
		if (cycleScopes.isEmpty()) {
			throw Program::Error(
				currentUnit,
				"Found unexpected 'break' statement ouside of a cycle!",
				token, ErrorCode::lgc
			);
		}
		rethrow(consume(Token::Type::semicolon, ";"));
		const SizeType cycleScope = cycleScopes.top();
		SizeType localCount = 0;
		for (Int64 i = locals.size() - 1; i >= 0; i -= 1) {
			if (locals[i].depth <= cycleScope) break;
			localCount += 1;
		}
		if (localCount > 0) {
			if (localCount == 1) {
				emitOperation(OPCode::POP);
			} else emitOperation({ OPCode::DSK, { .index = localCount } });
		}
		const SizeType breakJMP = emitJMP(OPCode::JMP);
		breakStack.push({ breakJMP, scopeDepth });
	}
	void Compiler::continueStatement() {
		const Token token = previous;
		if (cycleScopes.isEmpty()) {
			throw Program::Error(
				currentUnit,
				"Found unexpected 'break' statement ouside of a cycle!",
				token, ErrorCode::lgc
			);
		}
		rethrow(consume(Token::Type::semicolon, ";"));
		const SizeType cycleScope = cycleScopes.top();
		SizeType localCount = 0;
		for (Int64 i = locals.size() - 1; i >= 0; i -= 1) {
			if (locals[i].depth <= cycleScope) break;
			localCount += 1;
		}
		if (localCount > 0) {
			if (localCount == 1) {
				emitOperation(OPCode::POP);
			} else emitOperation({ OPCode::DSK, { .index = localCount } });
		}
		// We don't know yet if the jump is going to be
		// JMB or JMP for a continue so we use a rest:
		const SizeType continueJMP = emitRST();
		continueStack.push({ continueJMP, scopeDepth });
	}
	void Compiler::procStatement() {

	}
	void Compiler::funcStatement() {
		
	}
	void Compiler::swapStatement() {
		const Token token = previous;
		rethrow(consume(Token::Type::openParenthesis, "("));
		rethrow(consume(Token::Type::symbol, "identifier"));
		Token varA = previous;
		rethrow(
			consume(Token::Type::colon, ":");
			consume(Token::Type::symbol, "identifier");
		);
		Token varB = previous;
		Local localA, localB;
		SizeType argumentA = resolve(varA.lexeme, localA);
		SizeType argumentB = resolve(varB.lexeme, localB);
		if (argumentA == - 1) {
			throw Program::Error(
				currentUnit,
				"Cannot access local variable '" + varA.lexeme +
				"'!",
				previous, ErrorCode::lgc
			);
		}
		if (argumentB == - 1) {
			throw Program::Error(
				currentUnit,
				"Cannot access local variable '" + varA.lexeme +
				"'!",
				previous, ErrorCode::lgc
			);
		}
		if (localA.type != localB.type) {
			throw Program::Error(
				currentUnit,
				"Swap statement doesn't support values of non matching types '" +
				Converter::typeToString(localA.type) + "' and '" +
				Converter::typeToString(localB.type) + "'!",
				token, ErrorCode::typ
			);
		}
		if (localA.type > Type::StringType) {
			throw Program::Error(
				currentUnit,
				"Swap statement doesn't support '" +
				Converter::typeToString(localA.type) + "' types!",
				token, ErrorCode::typ
			);
		}
		emitOperation({ OPCode::CNS, { .index = argumentA } });
		emitOperation({ OPCode::CNS, { .index = argumentB } });
		emitOperation(OPCode::SWP);
		rethrow(
			consume(Token::Type::closeParenthesis, ")");
			consume(Token::Type::semicolon, ";");
		);
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
		const Token token = previous;
		if (canAssign && matchAssignment()) {
			throw Program::Error(
				currentUnit,
				"Found invalid assignment target '" +
				token.lexeme + "'!",
				token, ErrorCode::lgc
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
	inline Boolean Compiler::matchAssignment() {
		return match(Token::Type::equal)          ||
			   match(Token::Type::plusEqual)      ||
			   match(Token::Type::minusEqual)     ||
			   match(Token::Type::pipeEqual)      ||
			   match(Token::Type::starEqual)      ||
			   match(Token::Type::slashEqual)     ||
			   match(Token::Type::ampersandEqual) ||
			   match(Token::Type::modulusEqual)   ||
			   match(Token::Type::dollarEqual);
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
	inline SizeType Compiler::sourcePosition() {
		return program -> instructions.size();
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
		jmb = sourcePosition() - jmb + 1;
		emitOperation({ OPCode::JMB, { .index = jmb } });
	}
	inline SizeType Compiler::emitJMP(OPCode code) {
		const SizeType count = sourcePosition();
		emitOperation(code);
		return count;
	}
	inline void Compiler::patchJMP(SizeType jmp) {
		const SizeType jump = (sourcePosition() - jmp);
		program -> instructions[jmp].as.index = jump - 1;
	}
	inline void Compiler::patchJMB(SizeType pos, SizeType jmb) {
		jmb = pos - jmb + 1;
		program -> instructions[pos].as.index = jmb;
	}
	inline void Compiler::patchOP(SizeType op, OPCode code) {
		program -> instructions[op].code = code;
	}
	inline Array<ByteCode> Compiler::cutCodes(SizeType cut) {
		const SizeType position = sourcePosition();
		if (cut >= position) return Array<ByteCode>();
		Array<ByteCode> codes;
		for (SizeType i = cut; i < position; i += 1) {
			codes.push_back(program -> instructions.at(i));
		}
		program -> instructions.erase(
			program -> instructions.begin() + cut,
			program -> instructions.end()
		);
		return codes;
	}
	inline void Compiler::pasteCodes(Array<ByteCode> codes) {
		for (ByteCode & code : codes) {
			program -> instructions.push_back(code);
		}
	}
	inline void Compiler::beginScope() {
		scopeDepth += 1;
	}
	inline void Compiler::endScope() {
		scopeDepth -= 1;
		SizeType localCount = 0;
		while (locals.size() > 0 &&
			   locals[locals.size() - 1].depth > scopeDepth) {
			localCount += 1;
			locals.pop_back();
		}
		if (!localCount) return;
		if (localCount == 1) {
			emitOperation(OPCode::POP);
		} else emitOperation({ OPCode::DSK, { .index = localCount } });
	}

	inline SizeType Compiler::emitRST() {
		const SizeType count = sourcePosition();
		emitOperation(OPCode::RST);
		return count;
	}
	inline void Compiler::emitRET() {
		emitOperation(OPCode::RET);
	}
	inline void Compiler::emitHLT() {
		emitOperation(OPCode::HLT);
	}

	void Compiler::reset() {
		globals.clear();
		globalIndex = 0;
		assignmentStack.clear();
		locals.clear();
		scopeDepth = 0;
		cycleScopes.clear();
		breakStack.clear();
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
			emitHLT();
		);

		reset();

		program -> instructions.shrink_to_fit();

		return program;
	}

}

#undef rethrow

#endif
