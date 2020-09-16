
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

#include "../Types/Complex.hpp"
#include "../Virtual/Processor.hpp"

#define rethrow(A) try { A; } catch (Program::Error & e) { throw; }

namespace Spin {

	const Dictionary<Token::Type, Compiler::ParseRule> Compiler::rules = {
	
		{ Token::Type::openParenthesis, { & Compiler::grouping, & Compiler::call, Precedence::call } },
		{     Token::Type::openBracket, { & Compiler::array, & Compiler::subscription, Precedence::call } },

		{ Token::Type::questionMark, { nullptr, & Compiler::ternary, Precedence::assignment } },
		{        Token::Type::colon, { nullptr, & Compiler::cast, Precedence::assignment } },

		{  Token::Type::shiftL, { nullptr, & Compiler::binary, Precedence::shift } },
		{  Token::Type::shiftR, { nullptr, & Compiler::binary, Precedence::shift } },
		{ Token::Type::rotateL, { nullptr, & Compiler::binary, Precedence::shift } },
		{ Token::Type::rotateR, { nullptr, & Compiler::binary, Precedence::shift } },

		{       Token::Type::conjugate, { nullptr, & Compiler::postfix, Precedence::call } },
		{ Token::Type::exclamationMark, { & Compiler::prefix, nullptr, Precedence::none } },
		{           Token::Type::tilde, { & Compiler::prefix, nullptr, Precedence::term } },
		{           Token::Type::minus, { & Compiler::prefix, & Compiler::binary, Precedence::term } },
		{            Token::Type::plus, { & Compiler::prefix, & Compiler::binary, Precedence::term } },
		{           Token::Type::slash, { nullptr, & Compiler::binary, Precedence::factor } },
		{         Token::Type::modulus, { nullptr, & Compiler::binary, Precedence::factor } },
		{            Token::Type::star, { nullptr, & Compiler::binary, Precedence::factor } },

		{ Token::Type::lamda, { & Compiler::lamda, nullptr, Precedence::none } },

		{ Token::Type::symbol, { & Compiler::identifier, nullptr, Precedence::none } },

		{ Token::Type::ampersand, { nullptr, & Compiler::binary, Precedence::bitwiseAND } },
		{      Token::Type::pipe, { nullptr, & Compiler::binary, Precedence::bitwiseOR } },
		{    Token::Type::dollar, { nullptr, & Compiler::binary, Precedence::bitwiseXOR } },

		{ Token::Type::AND, { nullptr, & Compiler::logicAND, Precedence::logicAND } },
		{  Token::Type::OR, { nullptr, & Compiler::logicOR, Precedence::logicOR } },

		{       Token::Type::intLiteral, { & Compiler::integerLiteral, nullptr, Precedence::none } },
		{      Token::Type::realLiteral, { & Compiler::realLiteral, nullptr, Precedence::none } },
		{ Token::Type::imaginaryLiteral, { & Compiler::imaginaryLiteral, nullptr, Precedence::none } },
		{    Token::Type::stringLiteral, { & Compiler::stringLiteral, nullptr, Precedence::none } },
		{      Token::Type::charLiteral, { & Compiler::characterLiteral, nullptr, Precedence::none } },
		{      Token::Type::boolLiteral, { & Compiler::booleanLiteral, nullptr, Precedence::none } },
		{      Token::Type::selfKeyword, { & Compiler::selfLiteral, nullptr, Precedence::none } },

		{ Token::Type::realIdiom, { & Compiler::realIdioms, nullptr, Precedence::none } },

		{ Token::Type::readKeyword, { & Compiler::read, nullptr, Precedence::none } },

		{ Token::Type::inequality, { nullptr, & Compiler::binary, Precedence::equality } },
		{   Token::Type::equality, { nullptr, & Compiler::binary, Precedence::equality } },

		{      Token::Type::major, { nullptr, & Compiler::binary, Precedence::comparison } },
		{ Token::Type::majorEqual, { nullptr, & Compiler::binary, Precedence::comparison } },
		{      Token::Type::minor, { nullptr, & Compiler::binary, Precedence::comparison } },
		{ Token::Type::minorEqual, { nullptr, & Compiler::binary, Precedence::comparison } },

	};

	const Dictionary<Binary, Type> Compiler::infixTable = {
		// # << # ------------------------------------------------------------- # Composing Bitwise Shift Left#
		{ compose(Token::Type::shiftL, Type::CharacterType, Type::IntegerType), Type::CharacterType },
		{ compose(Token::Type::shiftL, Type::ByteType, Type::IntegerType), Type::ByteType },
		{ compose(Token::Type::shiftL, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		// # >> # ------------------------------------------------------------- # Composing Bitwise Shift Right#
		{ compose(Token::Type::shiftR, Type::CharacterType, Type::IntegerType), Type::CharacterType },
		{ compose(Token::Type::shiftR, Type::ByteType, Type::IntegerType), Type::ByteType },
		{ compose(Token::Type::shiftR, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		// # <* # ------------------------------------------------------------- # Composing Bitwise Rotation Left #
		{ compose(Token::Type::rotateL, Type::CharacterType, Type::IntegerType), Type::CharacterType },
		{ compose(Token::Type::rotateL, Type::ByteType, Type::IntegerType), Type::ByteType },
		{ compose(Token::Type::rotateL, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		// # *> # ------------------------------------------------------------- # Composing Bitwise Rotation Right #
		{ compose(Token::Type::rotateR, Type::CharacterType, Type::IntegerType), Type::CharacterType },
		{ compose(Token::Type::rotateR, Type::ByteType, Type::IntegerType), Type::ByteType },
		{ compose(Token::Type::rotateR, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		// # & # ------------------------------------------------------------- # Composing Bitwise AND #
		{ compose(Token::Type::ampersand, Type::CharacterType, Type::CharacterType), Type::CharacterType },
		{ compose(Token::Type::ampersand, Type::ByteType, Type::ByteType), Type::ByteType },
		{ compose(Token::Type::ampersand, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::ampersand, Type::BooleanType, Type::BooleanType), Type::BooleanType },
		// # | # ------------------------------------------------------------- # Composing Bitwise OR #
		{ compose(Token::Type::pipe, Type::CharacterType, Type::CharacterType), Type::CharacterType },
		{ compose(Token::Type::pipe, Type::ByteType, Type::ByteType), Type::ByteType },
		{ compose(Token::Type::pipe, Type::IntegerType, Type::IntegerType), Type::IntegerType },
		{ compose(Token::Type::pipe, Type::BooleanType, Type::BooleanType), Type::BooleanType },
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

		{ compose(Token::Type::plus, Type::StringType, Type::CharacterType), Type::StringType },
		{ compose(Token::Type::plus, Type::CharacterType, Type::StringType), Type::StringType },
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
	const Dictionary<Unary, Type> Compiler::prefixTable = {
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
	const Dictionary<Unary, Type> Compiler::postfixTable = {
		{ compose(Token::Type::conjugate, Type::ComplexType), Type::ComplexType },
	};
	const Dictionary<Types, Boolean> Compiler::castTable = {
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

	Compiler::TypeNode * Compiler::type() {
		if (match(Token::Type::basicType)) {
			return new TypeNode(
				Converter::stringToType(
					previous.lexeme
				)
			);
		}
		if (match(Token::Type::openBracket)) {
			TypeNode * node = new TypeNode(Type::ArrayType);
			rethrow(
				node -> next = type();
				consume(Token::Type::closeBracket, "]");
			);
			return node;
		}
		if (match(Token::Type::lamda)) {
			TypeNode * node = new TypeNode(Type::LamdaType);
			rethrow(consume(Token::Type::openParenthesis, "("));
			LamdaType * lamda = new LamdaType();
			if (!check(Token::Type::closeParenthesis)) {
				do {
					TypeNode * parameter;
					rethrow(parameter = type());
					lamda -> parameters.push_back(parameter);
				} while (match(Token::Type::comma));
			}
			rethrow(consume(Token::Type::closeParenthesis, ")"));
			if (match(Token::Type::colon)) {
				TypeNode * returnType;
				rethrow(returnType = type());
				lamda -> returnType = returnType;
			} else lamda -> returnType = new TypeNode(Type::VoidType);
			node -> setData(lamda);
			return node;
		}
		throw Program::Error(
			currentUnit,
			"Expected 'type' but found '" + current.lexeme + "'!",
			(current.type == Token::Type::endFile ? previous : current),
			ErrorCode::syx
		);
	}

	void Compiler::produceInitialiser(Compiler::TypeNode * type) {
		if (!type -> isContainer()) {
			switch (type -> type) {
				case   Type::BooleanType: emitOperation(OPCode::PSF); break;
				case Type::CharacterType: 
				case      Type::ByteType: emitOperation({
					OPCode::PSH, { .value = { .byte = 0 } }
				}); break;
				case   Type::IntegerType: emitOperation({
					OPCode::PSH, { .value = { .integer = 0 } }
				}); break;
				case      Type::RealType:
				case Type::ImaginaryType: emitOperation({
					OPCode::PSH, { .value = { .real = 0.0 } }
				}); break;
				case   Type::ComplexType: emitOperation(OPCode::PEC); break;
				case    Type::StringType: emitOperation(OPCode::PES); break;
				case     Type::ArrayType: emitOperation(OPCode::PEA); break;
				default: emitOperation({
					OPCode::PSH, { .value = { .integer = 0 } }
				}); break;
			}
			return;
		}
	}

	void Compiler::booleanLiteral() {
		if (previous.lexeme[0] == 't') {
			emitOperation(OPCode::PST);
		} else emitOperation(OPCode::PSF);
		pushType(Type::BooleanType);
	}
	void Compiler::characterLiteral() {
		String lexeme = previous.lexeme.substr(
			1, previous.lexeme.length() - 2
		);
		Character literal = Converter::escapeChar(lexeme);
		emitOperation(
			{ OPCode::PSH, { .value = { .byte = ((Byte)(literal)) } } }
		);
		pushType(Type::CharacterType);
	}
	void Compiler::stringLiteral() {
		String literal = previous.lexeme.substr(
			1, previous.lexeme.length() - 2
		);
		literal = Converter::escapeString(literal);
		emitString(literal);
		pushType(Type::StringType);
	}
	void Compiler::imaginaryLiteral() {
		Real literal = Converter::stringToImaginary(
			previous.lexeme
		);
		emitOperation(
			{ OPCode::PSH, { .value = { .real = literal } } }
		);
		pushType(Type::ImaginaryType);
	}
	void Compiler::realLiteral() {
		Real literal = Converter::stringToReal(
			previous.lexeme
		);
		emitOperation(
			{ OPCode::PSH, { .value = { .real = literal } } }
		);
		pushType(Type::RealType);
	}
	void Compiler::realIdioms() {
		if (previous.lexeme[0] == 'i') {
			emitOperation(OPCode::PSI);
		} else emitOperation(OPCode::PSU);
		pushType(Type::RealType);
	}
	void Compiler::integerLiteral() {
		Int64 literal = Converter::stringToInteger(
			previous.lexeme
		);
		emitOperation(
			{ OPCode::PSH, { .value = { .integer = literal } } }
		);
		pushType(Type::IntegerType);
	}
	void Compiler::selfLiteral() {
		if (routineIndexes.isEmpty()) {
			throw Program::Error(
				currentUnit,
				"Unexpected use of 'self' keyword outside of allowed lamda context!",
				previous, ErrorCode::lgc
			);
		}
		const Routine & lamda = routines[routineIndexes.top()];
		if (!lamda.name.empty() || !lamda.type) {
			throw Program::Error(
				currentUnit,
				"Unexpected use of 'self' keyword outside of allowed lamda context!",
				previous, ErrorCode::lgc
			);
		}
		emitOperation(OPCode::ULA);
		typeStack.push(TypeNode::copy(lamda.type));
	}

	void Compiler::expression() {
		rethrow(parsePrecedence(Precedence::assignment));
	}
	void Compiler::statement() {
		switch (current.type) {
			case       Token::Type::ifKeyword: advance(); ifStatement(); break;
			case    Token::Type::writeKeyword: advance(); writeStatement(); break;
			case     Token::Type::procKeyword: advance(); procStatement(); break;
			case     Token::Type::funcKeyword: advance(); funcStatement(); break;
			case   Token::Type::returnKeyword: advance(); returnStatement(); break;
			case      Token::Type::forKeyword: advance(); forStatement(); break;
			case    Token::Type::whileKeyword: advance(); whileStatement(); break;
			case    Token::Type::untilKeyword: advance(); untilStatement(); break;
			case       Token::Type::doKeyword: advance(); doWhileStatement(); break;
			case   Token::Type::repeatKeyword: advance(); repeatUntilStatement(); break;
			case     Token::Type::loopKeyword: advance(); loopStatement(); break;
			case    Token::Type::breakKeyword: advance(); breakStatement(); break;
			case Token::Type::continueKeyword: advance(); continueStatement(); break;
			case    Token::Type::sleepKeyword: advance(); sleepStatement(); break;
			case     Token::Type::swapKeyword: advance(); swapStatement(); break;
			case     Token::Type::restKeyword: advance(); emitRest(); break;
			case       Token::Type::openBrace: advance();
				beginScope();
				rethrow(block());
				endScope();
			break;
			case       Token::Type::semicolon: advance(); break;
			default: rethrow(expressionStatement());
		}
	}
	void Compiler::declaration() {
		switch (current.type) {
			case Token::Type::varKeyword: advance(); variable(); break;
			case Token::Type::conKeyword: advance(); constant(); break;
			case Token::Type::vecKeyword: advance(); break;
			case Token::Type::matKeyword: advance(); break;
			default: rethrow(statement());
		}
	}
	void Compiler::variable() {

		rethrow(consume(Token::Type::symbol, "identifier"));
		const String id = previous.lexeme;
		Token token = previous;

		for (Local local : locals) {
			if (local.ready && local.depth < scopeDepth) break;
			if (local.name == id) {
				throw Program::Error(
					currentUnit,
					"Variable redefinition! The identifier '" +
					id + "' was already declared in the current scope!",
					token, ErrorCode::lgc
				);
			}
		}
		// If we are compiling stuff inside a routine, the
		// value of its index will be at the top of the
		// stack, making it not empty:
		const Boolean isInRoutine = !routineIndexes.isEmpty();

		const SizeType currentLocal = locals.size();
		
		locals.push_back({
			id, scopeDepth,
			nullptr,
			false, false,
			isInRoutine
		});

		Boolean hasType = false;

		TypeNode * typeA = nullptr;
		TypeNode * typeB = nullptr;

		if (match(Token::Type::colon)) {
			hasType = true;
			rethrow(typeA = type());
		}

		if (match(Token::Type::equal)) {
			token = previous;
			rethrow(expression());
			typeB = popType();
			if (hasType) {
				if (!match(typeA, typeB)) {
					if (typeA -> isContainer() || typeB -> isContainer()) {
						const String descA = typeA -> description();
						const String descB = typeB -> description();
						delete typeA;
						delete typeB;
						throw Program::Error(
							currentUnit,
							"Assignment operator '=' doesn't support implicit cast of '" +
							descB + "' in '" + descA + "'!",
							token, ErrorCode::lgc
						);
					}
					const Types composed = runtimeCompose(typeB -> type, typeA -> type);
					auto casting = castTable.find(composed);
					if (casting == castTable.end()) {
						const String descA = typeA -> description();
						const String descB = typeB -> description();
						delete typeA;
						delete typeB;
						throw Program::Error(
							currentUnit,
							"Assignment operator '=' doesn't support implicit cast of '" +
							descB + "' in '" + descA + "'!",
							token, ErrorCode::lgc
						);
					}
					// If needed produce a CAST:
					if (casting -> second) {
						emitOperation({ OPCode::CST, { .types = composed } });
					}
				}
				delete typeB;
			} else typeA = typeB;
		} else if (hasType) {
			produceInitialiser(typeA);
		} else {
			// Has no type specification and no assignment:
			delete typeA;
			throw Program::Error(
				currentUnit,
				"Non initialised variable needs type specification!",
				token, ErrorCode::lgc
			);
		}

		// Locals sentinel:
		locals[currentLocal].ready = true;
		// Type addition:
		locals[currentLocal].type = typeA;

		rethrow(consume(Token::Type::semicolon, ";"));
	}
	void Compiler::constant() {

		rethrow(consume(Token::Type::symbol, "identifier"));
		const String id = previous.lexeme;
		Token token = previous;

		for (Local local : locals) {
			if (local.ready && local.depth < scopeDepth) break;
			if (local.name == id) {
				throw Program::Error(
					currentUnit,
					"Constant redefinition! The identifier '" +
					id + "' was already declared in the current scope!",
					token, ErrorCode::lgc
				);
			}
		}
		// If we are compiling stuff inside a routine, the
		// value of its index will be at the top of the
		// stack, making it not empty:
		const Boolean isInRoutine = !routineIndexes.isEmpty();
		
		const SizeType currentLocal = locals.size();
		
		locals.push_back({
			id, scopeDepth,
			nullptr,
			false, true,
			isInRoutine
		});

		Boolean hasType = false;

		TypeNode * typeA = nullptr;
		TypeNode * typeB = nullptr;

		if (match(Token::Type::colon)) {
			hasType = true;
			rethrow(typeA = type());
		}

		if (typeA -> isContainer()) {
			const String descA = typeA -> description();
			delete typeA;
			throw Program::Error(
				currentUnit,
				"Constant value doesn't support container type '" +
				descA + "'!",
				token, ErrorCode::lgc
			);
		}

		if (match(Token::Type::equal)) {
			token = previous;
			rethrow(expression());
			typeB = popType();
			if (hasType) {
				if (typeB -> isContainer()) {
					const String descA = typeA -> description();
					const String descB = typeB -> description();
					delete typeA;
					delete typeB;
					throw Program::Error(
						currentUnit,
						"Assignment operator '=' doesn't support implicit cast of '" +
						descB + "' in '" + descA + "'!",
						token, ErrorCode::lgc
					);
				}
				if ((typeA -> type) != (typeB -> type)) {
					// Since we're working with B -> A (A = B):
					const Types composed = runtimeCompose(typeB -> type, typeA -> type);
					auto casting = castTable.find(composed);
					if (casting == castTable.end()) {
						const String descA = typeA -> description();
						const String descB = typeB -> description();
						delete typeA;
						delete typeB;
						throw Program::Error(
							currentUnit,
							"Assignment operator '=' doesn't support implicit cast of '" +
							descB + "' in '" + descA + "'!",
							token, ErrorCode::lgc
						);
					}
					// If needed produce a CAST:
					if (casting -> second) {
						emitOperation({ OPCode::CST, { .types = composed } });
					}
				}
			} else typeA = typeB;
		} else {
			// Has no assignment:
			throw Program::Error(
				currentUnit,
				"Constant requires inline initialisation!",
				token, ErrorCode::lgc
			);
		}

		// Locals sentinel:
		locals[currentLocal].ready = true;
		// Type addition:
		locals[currentLocal].type = typeA;

		rethrow(consume(Token::Type::semicolon, ";"));
	}
	void Compiler::identifier() {
		Local local = resolve(previous.lexeme);
		TypeNode * typeA = TypeNode::copy(local.type);
		if (typeA && (typeA -> type == Type::RoutineType)) {
			delete local.type;
			if (current.type != Token::Type::openParenthesis) {
				delete typeA;
				throw Program::Error(
					currentUnit,
					"Routine identifier '" +
					previous.lexeme + "' requires call operator '()'!",
					previous, ErrorCode::lgc
				);
			}
			pushType(typeA);
			return;
		}
		const Token token = previous;
		if (local.index == - 1) {
			throw Program::Error(
				currentUnit,
				"Unexpected identifier '" +
				token.lexeme + "'!",
				token, ErrorCode::lgc
			);
		}
		const Boolean canAssign = assignmentStack.top();
		if (canAssign && matchAssignment()) {
			if (local.isConstant) {
				throw Program::Error(
					currentUnit,
					"Invalid assignment of constant '" + token.lexeme + "'!",
					token, ErrorCode::lgc
				);
			}
			// Takes care of operation assignments:
			const Token token = previous;
			OPCode o = OPCode::RST;
			Token::Type t;
			switch (token.type) {
				case      Token::Type::plusEqual: o = OPCode::ADD; t = Token::Type::plus; break;
				case     Token::Type::minusEqual: o = OPCode::SUB; t = Token::Type::minus; break;
				case      Token::Type::pipeEqual: o = OPCode::BWO; t = Token::Type::pipe; break;
				case      Token::Type::starEqual: o = OPCode::MUL; t = Token::Type::star; break;
				case     Token::Type::slashEqual: o = OPCode::DIV; t = Token::Type::slash; break;
				case Token::Type::ampersandEqual: o = OPCode::BWA; t = Token::Type::ampersand; break;
				case   Token::Type::modulusEqual: o = OPCode::MOD; t = Token::Type::modulus; break;
				case    Token::Type::dollarEqual: o = OPCode::BWX; t = Token::Type::dollar; break;
				default: break;
			}
			// If we have an mutation assignment:
			if (o != OPCode::RST) {
				// Get the item before mutation:
				emitOperation({
					(local.isStack ? OPCode::GLF : OPCode::GET),
					{ .index = local.index }
				});
			}
			rethrow(expression());
			TypeNode * typeB = popType();
			// If we have an mutation assignment:
			if (o != OPCode::RST) {
				if (typeA -> isContainer() || typeB -> isContainer()) {
					const String descA = typeA -> description();
					const String descB = typeA -> description();
					delete typeA;
					delete typeB;
					throw Program::Error(
						currentUnit,
						"Mutation assignment operator '" + token.lexeme +
						"' doesn't support operands of type '" +
						descA + "' and '" + descB + "'!",
						token, ErrorCode::typ
					);
				}
				const Types types = runtimeCompose(typeA -> type, typeB -> type);
				auto search = infixTable.find(
					runtimeCompose(t, typeA -> type, typeB -> type)
				);
				if (search == infixTable.end()) {
					const String descA = typeA -> description();
					const String descB = typeA -> description();
					delete typeA;
					delete typeB;
					throw Program::Error(
						currentUnit,
						"Mutation assignment operator '" + token.lexeme +
						"' doesn't support operands of type '" +
						descA + "' and '" + descB + "'!",
						token, ErrorCode::typ
					);
				}
				emitOperation({ o, { .types = types } });
				delete typeB;
				typeB = new TypeNode(search -> second);
			}
			if (!match(typeA, typeB)) {
				const Types composed = runtimeCompose(typeB -> type, typeA -> type);
				// Since we're working with B -> A (A = B):
				auto casting = castTable.find(composed);
				if (casting == castTable.end()) {
					const String descA = typeA -> description();
					const String descB = typeA -> description();
					delete typeA;
					delete typeB;
					throw Program::Error(
						currentUnit,
						"Operator '" + token.lexeme + "' doesn't support implicit cast of '" +
						descB + "' in '" + descA + "'!",
						token, ErrorCode::lgc
					);
				}
				// If needed produce a CAST:
				if (casting -> second) {
					emitOperation({ OPCode::CST, { .types = composed } });
				}
			}
			emitOperation({
				(local.isStack ? OPCode::SLF : OPCode::SET),
				{ .index = local.index }
			});
			delete typeB;
		} else {
			emitOperation({
				(local.isStack ? OPCode::GLF : OPCode::GET),
				{ .index = local.index }
			});
		}
		pushType(typeA);
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
		Type typeA = popAbsoluteType();
		const SizeType endJMP = emitJumpNext(OPCode::JAF);
		emitOperation(OPCode::POP);
		rethrow(parsePrecedence(Precedence::logicAND));
		Type typeB = popAbsoluteType();
		if (typeA != Type::BooleanType || typeB != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Binary operator '&&' doesn't support operands of type '" +
				Converter::typeToString(typeA) + "' and '" +
				Converter::typeToString(typeB) + "'!",
				token, ErrorCode::typ
			);
		}
		patchJumpNext(endJMP);
	}
	void Compiler::logicOR() {
		const Token token = previous;
		Type typeA = popAbsoluteType();
		const SizeType endJMP = emitJumpNext(OPCode::JAT);
		emitOperation(OPCode::POP);
		rethrow(parsePrecedence(Precedence::logicOR));
		Type typeB = popAbsoluteType();
		if (typeA != Type::BooleanType || typeB != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Binary operator '||' doesn't support operands of type '" +
				Converter::typeToString(typeA) + "' and '" +
				Converter::typeToString(typeB) + "'!",
				token, ErrorCode::typ
			);
		}
		patchJumpNext(endJMP);
	}

	void Compiler::grouping() {
		rethrow(
			expression();
			consume(Token::Type::closeParenthesis, ")");
		);
	}
	void Compiler::functional() {
		TypeNode * node = typeStack.pop();
		const Token token = previous;
		if (node -> type != Type::LamdaType) {
			throw Program::Error(
				currentUnit,
				"Expected valid lamda function before call operator '()'!",
				token, ErrorCode::lgc
			);
		}
		// Load the lambda address to later call:
		emitOperation(OPCode::LLA);
		Array<TypeNode *> types;
		if (!check(Token::Type::closeParenthesis)) {
			do {
				rethrow(expression());
				types.push_back(typeStack.pop());
			} while (match(Token::Type::comma));
		}
		rethrow(consume(Token::Type::closeParenthesis, ")"));
		LamdaType * lamda = (LamdaType *)(node -> getData());
		delete node;
		const SizeType size = lamda -> parameters.size();
		if (size != types.size()) {
			throw Program::Error(
				currentUnit,
				"Invalid arguments for lamda '" +
				node -> description() +"'!",
				token, ErrorCode::lgc
			);
		}
		for (SizeType i = 0; i < size; i += 1) {
			if (!match(lamda -> parameters[i], types[i])) {
				throw Program::Error(
					currentUnit,
					"Invalid arguments for lamda '" +
					node -> description() +"'!",
					token, ErrorCode::lgc
				);
			}
		}
		emitOperation({ OPCode::SSF, { .index = types.size() } });
		emitOperation(OPCode::LAM);
		pushType(TypeNode::copy(lamda -> returnType));
	}
	void Compiler::subscription() {
		const Token token = previous;
		TypeNode * node = popType();
		const Type type = node -> type;
		if (type != Type::StringType && type != Type::ArrayType) {
			delete node;
			throw Program::Error(
				currentUnit,
				"Expected subscriptable expression before subscript '[ ]' operator!",
				token, ErrorCode::lgc
			);
		}
		rethrow(expression());
		if (popAbsoluteType() != Type::IntegerType) {
			delete node;
			throw Program::Error(
				currentUnit,
				"Expected Integer expression inside subscription '[ ]' operator!",
				token, ErrorCode::lgc
			);
		}
		rethrow(consume(Token::Type::closeBracket, "]"));
		if (type == Type::ArrayType) {
			emitOperation(OPCode::ASC);
			pushType(TypeNode::copy(node -> next));
		} else {
			emitOperation(OPCode::SSC);
			pushType(Type::CharacterType);
		}
		delete node;
	}
	void Compiler::array() {
		const Token token = previous;
		if (match(Token::Type::closeBracket)) {
			emitOperation(OPCode::PEA);
			pushType(new TypeNode(Type::EmptyArray));
			return;
		}
		Array<Array<ByteCode>> elements;
		Array<TypeNode *> types;
		do {
			const SizeType i = sourcePosition();
			rethrow(expression());
			elements.push_back(cutCodes(i));
			types.push_back(typeStack.pop());
		} while (match(Token::Type::comma));
		rethrow(consume(Token::Type::closeBracket, "]"));
		Type topType = types[0] -> type;
		const SizeType size = types.size();
		TypeNode * finalType = nullptr;
		switch (topType) {
			case Type::BooleanType:
				for (TypeNode * type : types) {
					if (type -> type != Type::BooleanType) {
						throw Program::Error(
							currentUnit,
							"Found incompatible types in '[ ]' array intialisation!",
							token, ErrorCode::lgc
						);
					}
				}
				finalType = new TypeNode(Type::BooleanType);
			break;
			case Type::CharacterType: {
				Boolean isString = false;
				for (TypeNode * type : types) {
					if (type -> type != Type::CharacterType) {
						if (type -> type != Type::StringType) {
							throw Program::Error(
								currentUnit,
								"Found incompatible types in '[ ]' array intialisation!",
								token, ErrorCode::lgc
							);
						}
						isString = true;
					}
				}
				finalType = new TypeNode(
					isString ? Type::StringType : Type::CharacterType
				);
			} break;
			case Type::StringType:
				for (TypeNode * type : types) {
					if (type -> type != Type::CharacterType &&
						type -> type != Type::StringType) {
						throw Program::Error(
							currentUnit,
							"Found incompatible types in '[ ]' array intialisation!",
							token, ErrorCode::lgc
						);
					}
				}
				finalType = new TypeNode(Type::StringType);
			break;
			case Type::ByteType:
				for (TypeNode * type : types) {
					if (type -> type != Type::ByteType &&
						type -> type != Type::IntegerType ) {
						throw Program::Error(
							currentUnit,
							"Found incompatible types in '[ ]' array intialisation!",
							token, ErrorCode::lgc
						);
					}
				}
				finalType = new TypeNode(Type::ByteType);
			break;
			case Type::IntegerType:
				for (TypeNode * type : types) {
					switch (type -> type) {
						case Type::IntegerType: break;
						case Type::RealType:
							if (topType == Type::ComplexType) break;
							topType = Type::RealType;
						break;
						case Type::ImaginaryType:
							topType = Type::ComplexType;
						break;
						case Type::ComplexType:
							topType = Type::ComplexType;
						break;
						default:
							throw Program::Error(
								currentUnit,
								"Found incompatible types in '[ ]' array intialisation!",
								token, ErrorCode::lgc
							);
						break;
					}
				}
				finalType = new TypeNode(topType);
			break;
			case Type::RealType:
				for (TypeNode * type : types) {
					switch (type -> type) {
						case Type::IntegerType: break;
						case Type::RealType: break;
						case Type::ImaginaryType:
							topType = Type::ComplexType;
						break;
						case Type::ComplexType:
							topType = Type::ComplexType;
						break;
						default:
							throw Program::Error(
								currentUnit,
								"Found incompatible types in '[ ]' array intialisation!",
								token, ErrorCode::lgc
							);
						break;
					}
				}
				finalType = new TypeNode(topType);
			break;
			case Type::ImaginaryType:
				for (TypeNode * type : types) {
					switch (type -> type) {
						case Type::IntegerType:
							topType = Type::ComplexType;
						break;
						case Type::RealType:
							topType = Type::ComplexType;
						break;
						case Type::ImaginaryType:
							if (topType == Type::ComplexType) break;
							topType = Type::ImaginaryType;
						break;
						case Type::ComplexType:
							topType = Type::ComplexType;
						break;
						default:
							throw Program::Error(
								currentUnit,
								"Found incompatible types in '[ ]' array intialisation!",
								token, ErrorCode::lgc
							);
						break;
					}
				}
				finalType = new TypeNode(topType);
			break;
			case Type::ComplexType:
				for (TypeNode * type : types) {
					switch (type -> type) {
						case Type::IntegerType: break;
						case Type::RealType: break;
						case Type::ImaginaryType: break;
						case Type::ComplexType: break;
						default:
							throw Program::Error(
								currentUnit,
								"Found incompatible types in '[ ]' array intialisation!",
								token, ErrorCode::lgc
							);
						break;
					}
				}
				finalType = new TypeNode(Type::ComplexType);
			break;
			case Type::ArrayType:
			case Type::LamdaType:
				finalType = TypeNode::copy(types[0]);
				for (SizeType i = 1; i < size; i += 1) {
					if (!match(finalType, types[i])) {
						throw Program::Error(
							currentUnit,
							"Found incompatible types in '[ ]' array intialisation!",
							token, ErrorCode::lgc
						);
					}
				}
			break;
			default:
				throw Program::Error(
					currentUnit,
					"Found incompatible types in '[ ]' array intialisation!",
					token, ErrorCode::lgc
				);
			break;
		} 
		if (!finalType) {
			throw Program::Error(
				currentUnit,
				"Found incompatible types in '[ ]' array intialisation!",
				token, ErrorCode::lgc
			);
		}
		TypeNode * typeA = finalType;
		for (SizeType i = 0; i < size; i += 1) {
			pasteCodes(elements[i]);
			TypeNode * typeB = types[i];
			if (!match(typeA, typeB)) {
				const Types composed = runtimeCompose(typeB -> type, typeA -> type);
				// Since we're working with B -> A (A = B):
				auto casting = castTable.find(composed);
				if (casting == castTable.end()) {
					const String descA = typeA -> description();
					const String descB = typeA -> description();
					throw Program::Error(
						currentUnit,
						"Found incompatible types in '[ ]' array intialisation!",
						token, ErrorCode::lgc
					);
				}
				// If needed produce a CAST:
				if (casting -> second) {
					emitOperation({ OPCode::CST, { .types = composed } });
				}
			}
			delete typeB;
		}
		finalType = new TypeNode(
			Type::ArrayType, finalType
		);
		emitOperation({ OPCode::PSA, { .index = size } });
		typeStack.push(finalType);
	}
	void Compiler::cast() {
		const Token token = previous;
		Type typeB = popAbsoluteType();
		rethrow(consume(Token::Type::basicType, "Type"));
		Type typeA = Converter::stringToType(previous.lexeme);
		if (typeA != typeB) {
			// Since we're working with B -> A (B : A):
			auto casting = castTable.find(
				runtimeCompose(typeB, typeA)
			);
			if (casting == castTable.end()) {
				throw Program::Error(
					currentUnit,
					"Explicit cast operator ':' doesn't support conversion from '" +
					Converter::typeToString(typeB) + "' to '" +
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
		pushType(typeA);
	}
	void Compiler::call() {
		if (typeStack.top() -> type == Type::LamdaType) {
			functional();
			return;
		}
		if (popAbsoluteType() != Type::RoutineType) {
			throw Program::Error(
				currentUnit,
				"Expected identifier before call operator '()'!",
				previous, ErrorCode::lgc
			);
		}
		Token token = tokens -> at(index - 3);
		Array<TypeNode *> types;
		if (!check(Token::Type::closeParenthesis)) {
			do {
				rethrow(expression());
				types.push_back(popType());
			} while (match(Token::Type::comma));
		}
		SizeType prototypeIndex;
		rethrow(prototypeIndex = locate(token.lexeme, types));
		if (prototypeIndex == - 1) {
			if (types.empty()) {
				throw Program::Error(
					currentUnit,
					"No matching routine for call '" +
					token.lexeme + "()'!",
					token, ErrorCode::typ
				);
			}
			String eTypes;
			for (TypeNode * type : types) {
				eTypes += type -> description() + ", ";
			}
			eTypes.pop_back();
			eTypes.pop_back();
			throw Program::Error(
				currentUnit,
				"No matching routine for call '" +
				token.lexeme + "(" + eTypes + ")'!",
				token, ErrorCode::typ
			);
		}
		rethrow(consume(Token::Type::closeParenthesis, ")"));
		// Setting the stack frame to the current position,
		// minus the number of arguments passed in input:
		emitOperation({ OPCode::SSF, { .index = types.size() } });
		emitCall(prototypeIndex);
		pushType(
			new TypeNode(
				* prototypes.at(prototypeIndex).returnType
			)
		);
	}
	void Compiler::lamda() {
		Token token = previous;
		const SizeType scope = scopeDepth;
		beginVirtualScope();
		cycleScopes.push(- 1);
		rethrow(consume(Token::Type::openParenthesis, "("));
		const SizeType frame = locals.size();
		Array<TypeNode *> types; String name;
		const SizeType cutPosition = sourcePosition();
		if (!check(Token::Type::closeParenthesis)) {
			do {
				rethrow(consume(Token::Type::symbol, "identifier"));
				name = previous.lexeme;
				TypeNode * node;
				rethrow(
					consume(Token::Type::colon, ":");
					node = type();
				);
				try {
					for (Local local : locals) {
						if (local.ready && local.depth < scopeDepth) break;
						if (local.name == name) {
							throw Program::Error(
								currentUnit,
								"Parameter redefinition! The identifier '" +
								name + "' was already declared in the current scope!",
								previous, ErrorCode::lgc
							);
						}
					}
				} catch (Program::Error & e) { throw; }
				locals.push_back({
					name, scopeDepth, node,
					true, false, true
				});
				types.push_back(node);
			} while (match(Token::Type::comma));
		}
		rethrow(consume(Token::Type::closeParenthesis, ")"));
		TypeNode * returnType;
		if (match(Token::Type::colon)) {
			rethrow(returnType = type());
		} else returnType = new TypeNode(Type::VoidType);
		rethrow(consume(Token::Type::openBrace, "{"));
		TypeNode * lamdaNode = new TypeNode(Type::LamdaType);
		LamdaType * lamdaType = new LamdaType(
			types, TypeNode::copy(returnType)
		);
		lamdaNode -> setData(lamdaType);
		Routine routine; routine.type = lamdaNode;
		routine.parameters = types; routine.scope = scope;
		routine.returnType = returnType; routine.frame = frame;
		const SizeType routineIndex = routines.size();
		// Notifying the return statements:
		routineIndexes.push(routineIndex);
		routines.push_back(routine);
		rethrow(block());
		emitPop(countLocals(scope));
		if (!routines[routineIndex].returns &&
			returnType -> type != Type::VoidType) {
			throw Program::Error(
				currentUnit,
				"Missing return statement in anonymous lamda!",
				token, ErrorCode::lgc
			);
		}
		produceInitialiser(returnType);
		emitOperation(OPCode::RET);
		routines[routineIndex].code = cutCodes(cutPosition);
		routineIndexes.decrease();
		cycleScopes.decrease();
		lamdas.insert({ routineIndex, sourcePosition() });
		// Placeholder value, to be later replaced with
		// the real lambda address in resolveRoutines().
		emitOperation({ OPCode::PSH, { .index = 0 } });
		endVirtualScope();
		pushType(lamdaNode);
	}
	void Compiler::ternary() {
		Token token = previous;
		ParseRule rule = getRule(token.type);
		if (popAbsoluteType() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside ternary condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType thenJMP = emitJumpNext(OPCode::JIF);
		// Then Condition:
		rethrow(parsePrecedence((Precedence)(rule.precedence + 1)));
		TypeNode * typeA = popType();
		// End then.
		const SizeType elseJMP = emitJumpNext(OPCode::JMP);
		patchJumpNext(thenJMP);

		rethrow(consume(Token::Type::colon, ":"));
		token = previous;

		// Else Condition:
		rethrow(parsePrecedence((Precedence)(rule.precedence + 1)));
		TypeNode * typeB = popType();
		// End else.
		patchJumpNext(elseJMP);

		if (!match(typeA, typeB)) {
			const String descA = typeA -> description();
			const String descB = typeB -> description();
			delete typeA;
			delete typeB;
			throw Program::Error(
				currentUnit,
				"Ternary operator ' ? : ' doesn't support operands of non matching types '" +
				descA + "' and '" + descB + "'!",
				token, ErrorCode::typ
			);
		}
		delete typeB;
		pushType(typeA);
	}
	void Compiler::postfix() {
		const Token token = previous;
		Type type = popAbsoluteType();
		auto search = postfixTable.find(
			runtimeCompose(token.type, type)
		);
		if (search == prefixTable.end()) {
			throw Program::Error(
				currentUnit,
				"Unary operator '" + token.lexeme + "' doesn't support any operand of type '" +
				Converter::typeToString(type) + "'!",
				token, ErrorCode::typ
			);
		}
		switch (token.type) {
			case Token::Type::conjugate: emitOperation(OPCode::CCJ); break;
			default: break;
		}
		pushType(search -> second);
		foldUnary(token);
	}
	void Compiler::binary() {
		const Token token = previous;
		ParseRule rule = getRule(token.type);
		Type typeA = popAbsoluteType();
		rethrow(parsePrecedence((Precedence)(rule.precedence + 1)));
		Type typeB = popAbsoluteType();
		const Types types = runtimeCompose(typeA, typeB);
		auto search = infixTable.find(
			runtimeCompose(token.type, typeA, typeB)
		);
		if (search == infixTable.end()) {
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
			case Token::Type::modulus: emitOperation({ OPCode::DIV, { .types = types } }); break;
			case   Token::Type::equality: emitOperation({ OPCode::EQL, { .types = types } }); break;
			case Token::Type::inequality: emitOperation({ OPCode::NEQ, { .types = types } }); break;
			case      Token::Type::major: emitOperation({ OPCode::GRT, { .types = types } }); break;
			case Token::Type::majorEqual: emitOperation({ OPCode::GEQ, { .types = types } }); break;
			case      Token::Type::minor: emitOperation({ OPCode::LSS, { .types = types } }); break;
			case Token::Type::minorEqual: emitOperation({ OPCode::LEQ, { .types = types } }); break;
			case  Token::Type::ampersand: emitOperation({ OPCode::BWA, { .types = types } }); break;
			case       Token::Type::pipe: emitOperation({ OPCode::BWO, { .types = types } }); break;
			case     Token::Type::shiftL: emitOperation({ OPCode::BSL, { .type = typeA } }); break;
			case     Token::Type::shiftR: emitOperation({ OPCode::BSR, { .type = typeA } }); break;
			case    Token::Type::rotateL: emitOperation({ OPCode::BRL, { .type = typeA } }); break;
			case    Token::Type::rotateR: emitOperation({ OPCode::BRR, { .type = typeA } }); break;
			default: break;
		}
		pushType(search -> second);
		foldBinary(token);
	}
	void Compiler::prefix() {
		const Token token = previous;
		rethrow(parsePrecedence(Precedence::unary));
		Type type = popAbsoluteType();
		auto search = prefixTable.find(
			runtimeCompose(token.type, type)
		);
		if (search == prefixTable.end()) {
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
		pushType(search -> second);
		foldUnary(token);
	}
	void Compiler::read() {
		const Token token = previous;
		if (match(Token::Type::openParenthesis)) {
			if (!check(Token::Type::closeParenthesis)) {
				Array<Array<ByteCode>> codes;
				do {
					const SizeType cutPosition = sourcePosition();
					rethrow(expression());
					TypeNode * type = popType();
					if (type -> type > Type::StringType) {
						throw Program::Error(
							currentUnit,
							"Read statement doesn't support any operand of type '" +
							type -> description() + "'!",
							token, ErrorCode::typ
						);
					}
					emitOperation({ OPCode::TYP, { .value = { .byte = type -> type } } });
					emitOperation({ OPCode::INT, { .type = (Type)Interrupt::write } });
					delete type;
					codes.push_back(cutCodes(cutPosition));
				} while (match(Token::Type::comma));
				for (auto & code : codes) pasteCodes(code);
			}
			rethrow(consume(Token::Type::closeParenthesis, ")"));
		}
		emitOperation({ OPCode::INT, { .type = (Type)Interrupt::readln } });
		pushType(Type::StringType);
	}

	void Compiler::expressionStatement() {
		rethrow(
			expression();
			consume(Token::Type::semicolon, ";");
		);
		decreaseType();
		emitOperation(OPCode::POP);
	}
	void Compiler::writeStatement() {
		const Token token = previous;
		Array<Array<ByteCode>> codes;
		do {
			const SizeType cutPosition = sourcePosition();
			rethrow(expression());
			TypeNode * type = popType();
			if (type -> type > Type::StringType) {
				throw Program::Error(
					currentUnit,
					"Print statement doesn't support any operand of type '" +
					type -> description() + "'!",
					token, ErrorCode::typ
				);
			}
			emitOperation({ OPCode::TYP, { .value = { .byte = type -> type } } });
			emitOperation({ OPCode::INT, { .type = (Type)Interrupt::write } });
			delete type;
			codes.push_back(cutCodes(cutPosition));
		} while (match(Token::Type::comma));
		for (auto & code : codes) pasteCodes(code);
		program -> instructions.pop_back();
		emitOperation({ OPCode::INT, { .type = (Type)Interrupt::writeln } });
		rethrow(consume(Token::Type::semicolon, ";"));
	}
	void Compiler::ifStatement() {
		const Token token = previous;
		rethrow(
			consume(Token::Type::openParenthesis, "(");
			expression();
			consume(Token::Type::closeParenthesis, ")");
		);
		if (popAbsoluteType() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'if' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType tempJMP = emitRest();
		const SizeType cutPosition = sourcePosition();
		rethrow(statement());
		if (!match(Token::Type::elseKeyword)) {
			// If statement with only then:
			patchOperation(tempJMP, OPCode::JIF);
			patchJumpNext(tempJMP);
		} else {
			// If statement with then and else:
			patchOperation(tempJMP, OPCode::JIT);
			Array<ByteCode> thenCodes = cutCodes(cutPosition);
			rethrow(statement());
			const SizeType endJMP = emitJumpNext(OPCode::JMP);
			patchJumpNext(tempJMP);
			pasteCodes(thenCodes);
			patchJumpNext(endJMP);
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
		if (popAbsoluteType() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'while' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType exitJMP = emitJumpNext(OPCode::JIF);
		rethrow(statement());
		emitJumpBack(loopStart);
		patchJumpNext(exitJMP);
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJumpNext(breakStack.pop().line);
		}
		while (!continueStack.isEmpty()) {
			if (continueStack.top().scope <= scopeDepth) break;
			const SizeType pos = continueStack.pop().line;
			patchOperation(pos, OPCode::JMP);
			patchJumpBack(pos, loopStart);
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
		if (popAbsoluteType() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'until' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType exitJMP = emitJumpNext(OPCode::JIT);
		rethrow(statement());
		emitJumpBack(loopStart);
		patchJumpNext(exitJMP);
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJumpNext(breakStack.pop().line);
		}
		while (!continueStack.isEmpty()) {
			if (continueStack.top().scope <= scopeDepth) break;
			const SizeType pos = continueStack.pop().line;
			patchOperation(pos, OPCode::JMP);
			patchJumpBack(pos, loopStart);
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
			patchOperation(pos, OPCode::JMP);
			patchJumpNext(pos);
		}
		rethrow(
			expression();
			consume(Token::Type::closeParenthesis, ")");
			consume(Token::Type::semicolon, ";");
		);
		if (popAbsoluteType() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'do while' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType exitJMP = emitJumpNext(OPCode::JIF);
		emitJumpBack(loopStart);
		patchJumpNext(exitJMP);
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJumpNext(breakStack.pop().line);
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
			patchOperation(pos, OPCode::JMP);
			patchJumpNext(pos);
		}
		rethrow(
			expression();
			consume(Token::Type::closeParenthesis, ")");
			consume(Token::Type::semicolon, ";");
		);
		if (popAbsoluteType() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'repeat until' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType exitJMP = emitJumpNext(OPCode::JIT);
		emitJumpBack(loopStart);
		patchJumpNext(exitJMP);
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJumpNext(breakStack.pop().line);
		}
	}
	void Compiler::loopStatement() {
		const SizeType loopStart = sourcePosition();
		cycleScopes.push(scopeDepth);
		rethrow(statement());
		emitJumpBack(loopStart);
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJumpNext(breakStack.pop().line);
		}
		while (!continueStack.isEmpty()) {
			if (continueStack.top().scope <= scopeDepth) break;
			const SizeType pos = continueStack.pop().line;
			patchOperation(pos, OPCode::JMP);
			patchJumpBack(pos, loopStart);
		}
	}
	void Compiler::forStatement() {
		beginScope();
		cycleScopes.push(scopeDepth);
		const Token token = previous;
		rethrow(consume(Token::Type::openParenthesis, "("));
		if (match(Token::Type::varKeyword)) {
			rethrow(variable());
			// Variable already consumes ';'.
		} else if (match(Token::Type::conKeyword)) {
			rethrow(constant());
			// Constant already consumes ';'.
		} else {
			rethrow(
				expression();
				emitOperation(OPCode::POP);
				consume(Token::Type::semicolon, ";");
			);
			decreaseType();
		}
		const SizeType loopStart = sourcePosition();
		rethrow(expression());
		rethrow(consume(Token::Type::semicolon, ";"));
		if (popAbsoluteType() != Type::BooleanType) {
			throw Program::Error(
				currentUnit,
				"Expected Boolean expression inside 'for' condition!",
				token, ErrorCode::lgc
			);
		}
		const SizeType exitJMP = emitJumpNext(OPCode::JIF);
		const SizeType cutExpression = sourcePosition();
		rethrow(expression());
		emitOperation(OPCode::POP);
		decreaseType();
		rethrow(consume(Token::Type::closeParenthesis, ")"));
		const Array<ByteCode> codes = cutCodes(cutExpression);
		rethrow(statement()); // Body.
		while (!continueStack.isEmpty()) {
			if (continueStack.top().scope <= scopeDepth) break;
			const SizeType pos = continueStack.pop().line;
			patchOperation(pos, OPCode::JMP);
			patchJumpNext(pos);
		}
		pasteCodes(codes); // Increment.
		emitJumpBack(loopStart);
		patchJumpNext(exitJMP);
		cycleScopes.decrease();
		while (!breakStack.isEmpty()) {
			if (breakStack.top().scope <= scopeDepth) break;
			patchJumpNext(breakStack.pop().line);
		}
		endScope();
	}
	void Compiler::breakStatement() {
		const Token token = previous;
		if (cycleScopes.isEmpty() || cycleScopes.top() == - 1) {
			throw Program::Error(
				currentUnit,
				"Found unexpected 'break' statement ouside of a cycle!",
				token, ErrorCode::lgc
			);
		}
		rethrow(consume(Token::Type::semicolon, ";"));
		emitPop(countLocals(cycleScopes.top()));
		const SizeType breakJMP = emitJumpNext(OPCode::JMP);
		breakStack.push({ breakJMP, scopeDepth });
	}
	void Compiler::continueStatement() {
		const Token token = previous;
		if (cycleScopes.isEmpty() || cycleScopes.top() == - 1) {
			throw Program::Error(
				currentUnit,
				"Found unexpected 'break' statement ouside of a cycle!",
				token, ErrorCode::lgc
			);
		}
		rethrow(consume(Token::Type::semicolon, ";"));
		emitPop(countLocals(cycleScopes.top()));
		// We don't know yet if the jump is going to be
		// JMB or JMP for a continue so we use a rest:
		const SizeType continueJMP = emitRest();
		continueStack.push({ continueJMP, scopeDepth });
	}
	void Compiler::procStatement() {
		const SizeType scope = scopeDepth;
		beginVirtualScope();
		cycleScopes.push(- 1);
		rethrow(consume(Token::Type::symbol, "identifier"));
		const String id = previous.lexeme;
		const Token routineToken = previous;
		rethrow(consume(Token::Type::openParenthesis, "("));
		const SizeType frame = locals.size();
		Array<TypeNode *> types; String name;
		if (!check(Token::Type::closeParenthesis)) {
			do {
				rethrow(consume(Token::Type::symbol, "identifier"));
				name = previous.lexeme;
				TypeNode * node;
				rethrow(
					consume(Token::Type::colon, ":");
					node = type();
				);
				try {
					for (Local local : locals) {
						if (local.ready && local.depth < scopeDepth) break;
						if (local.name == name) {
							throw Program::Error(
								currentUnit,
								"Parameter redefinition! The identifier '" +
								name + "' was already declared in the current scope!",
								previous, ErrorCode::lgc
							);
						}
					}
				} catch (Program::Error & e) { throw; }
				locals.push_back({
					name, scopeDepth, node,
					true, false, true
				});
				types.push_back(node);
			} while (match(Token::Type::comma));
		}
		rethrow(
			consume(Token::Type::closeParenthesis, ")");
			consume(Token::Type::openBrace, "{");
		);
		Routine routine; routine.name = id;
		routine.parameters = types; routine.scope = scope;
		routine.frame = frame;
		const SizeType routineIndex = routines.size();
		// Notifying the return statements:
		routineIndexes.push(routineIndex);
		for (Routine & r : routines) {
			if (r.name == id) {
				if (r.parameters.size() == types.size()) {
					Boolean found = true;
					for (SizeType i = 0; i < types.size(); i += 1) {
						if (!match(r.parameters.at(i), types.at(i))) {
							found = false;
							break;
						}
					}
					if (found) {
						throw Program::Error(
							currentUnit,
							"Routine redefinition! The routine '" +
							routineToken.lexeme + "' has already been declared!",
							routineToken, ErrorCode::lgc
						);
					}
				}
			}
		}
		// Linking prototype:
		routine.prototypeIndex = locate(routine.name, types);
		if (routine.prototypeIndex == - 1) {
			throw Program::Error(
				currentUnit,
				"Invalid prototype for routine definition '" +
				routineToken.lexeme + "'!",
				routineToken, ErrorCode::lgc
			);
		}
		routines.push_back(routine);
		// Moving the code in a temporary location to place
		// it later after the whole code:
		const SizeType cutPosition = sourcePosition();
		rethrow(block());
		emitPop(countLocals(scope));
		// Return safety net:
		// Pushing 0 because every <expression> returns something:
		emitOperation({ OPCode::PSH, { .index = 0 } });
		emitOperation(OPCode::RET);
		routines[routineIndex].code = cutCodes(cutPosition);
		routineIndexes.decrease();
		cycleScopes.decrease();
		endVirtualScope();
	}
	void Compiler::funcStatement() {
		const SizeType scope = scopeDepth;
		beginVirtualScope();
		cycleScopes.push(- 1);
		rethrow(consume(Token::Type::symbol, "identifier"));
		const String id = previous.lexeme;
		const Token routineToken = previous;
		rethrow(consume(Token::Type::openParenthesis, "("));
		const SizeType frame = locals.size();
		Array<TypeNode *> types; String name;
		if (!check(Token::Type::closeParenthesis)) {
			do {
				rethrow(consume(Token::Type::symbol, "identifier"));
				name = previous.lexeme;
				TypeNode * node;
				rethrow(
					consume(Token::Type::colon, ":");
					node = type();
				);
				try {
					for (Local local : locals) {
						if (local.ready && local.depth < scopeDepth) break;
						if (local.name == name) {
							throw Program::Error(
								currentUnit,
								"Parameter redefinition! The identifier '" +
								name + "' was already declared in the current scope!",
								previous, ErrorCode::lgc
							);
						}
					}
				} catch (Program::Error & e) { throw; }
				locals.push_back({
					name, scopeDepth, node,
					true, false, true
				});
				types.push_back(node);
			} while (match(Token::Type::comma));
		}
		TypeNode * returnType;
		rethrow(
			consume(Token::Type::closeParenthesis, ")");
			consume(Token::Type::colon, ":");
			returnType = type();
		);
		rethrow(consume(Token::Type::openBrace, "{"));
		Routine routine; routine.name = id;
		routine.parameters = types; routine.scope = scope;
		routine.returnType = returnType; routine.frame = frame;
		const SizeType routineIndex = routines.size();
		// Notifying the return statements:
		routineIndexes.push(routineIndex);
		for (Routine & r : routines) {
			if (r.name == id) {
				if (r.parameters.size() == types.size()) {
					Boolean found = true;
					for (SizeType i = 0; i < types.size(); i += 1) {
						if (!match(r.parameters.at(i), types.at(i))) {
							found = false;
							break;
						}
					}
					if (found) {
						throw Program::Error(
							currentUnit,
							"Routine redefinition! The routine '" +
							routineToken.lexeme + "' has already been declared!",
							routineToken, ErrorCode::lgc
						);
					}
				}
			}
		}
		// Linking prototype:
		routine.prototypeIndex = locate(routine.name, types);
		if (routine.prototypeIndex == - 1) {
			throw Program::Error(
				currentUnit,
				"Invalid prototype for routine definition '" +
				routineToken.lexeme + "'!",
				routineToken, ErrorCode::lgc
			);
		}
		routines.push_back(routine);
		// Moving the code in a temporary location to place
		// it later after the whole code:
		const SizeType cutPosition = sourcePosition();
		rethrow(block());
		emitPop(countLocals(scope));
		if (!routines[routineIndex].returns) {
			throw Program::Error(
				currentUnit,
				"Missing return statement in routine '" +
				routineToken.lexeme + "'!",
				routineToken, ErrorCode::lgc
			);
		}
		// Return safety net:
		produceInitialiser(returnType);
		emitOperation(OPCode::RET);
		routines[routineIndex].code = cutCodes(cutPosition);
		routineIndexes.decrease();
		cycleScopes.decrease();
		endVirtualScope();
	}
	void Compiler::returnStatement() {
		const Token token = previous;
		if (routineIndexes.isEmpty()) {
			throw Program::Error(
				currentUnit,
				"Found unexpected 'return' statement ouside of a function or procedure!",
				token, ErrorCode::lgc
			);
		}
		// Needs to be a reference for later usage:
		Routine & routine = routines.at(routineIndexes.top());
		if (match(Token::Type::semicolon)) {
			// Procedure:
			if (routine.returnType -> type != Type::VoidType) {
				throw Program::Error(
					currentUnit,
					"Return statement inside function scope must return a valid value!",
					token, ErrorCode::lgc
				);
			}
			emitPop(countLocals(routine.scope));
			// Pushing 0 because every <expression> returns something:
			emitOperation({ OPCode::PSH, { .index = 0 } });
		} else {
			// Function:
			if (routine.returnType -> type == Type::VoidType) {
				throw Program::Error(
					currentUnit,
					"Return statement inside procedure scope should never return a value!",
					token, ErrorCode::lgc
				);
			}
			// Eventual expression casting:
			TypeNode * typeA = routine.returnType;
			rethrow(expression());
			TypeNode * typeB = popType();
			if (!match(typeA, typeB)) {
				// Since we're working with B -> A (return (A)(B);):
				const Types composed = runtimeCompose(typeB -> type, typeA -> type);
				auto casting = castTable.find(composed);
				if (casting == castTable.end()) {
					throw Program::Error(
						currentUnit,
						"Implicit cast doesn't support conversion from '" +
						typeB -> description() + "' to '" +
						typeA -> description() + "'!",
						token, ErrorCode::lgc
					);
				}
				// If needed produce a CAST:
				if (casting -> second) {
					emitOperation({ OPCode::CST, { .types = composed } });
				}
			}
			routine.returns = true;
			// Back up of the top of the stack before POP:
			emitOperation(OPCode::CTP);
			emitPop(countLocals(routine.scope));
			emitOperation(OPCode::LTP);
		}
		emitOperation(OPCode::RET);
	}
	void Compiler::swapStatement() {
		// TODO: Make other stuff swappable...
		const Token token = previous;
		rethrow(consume(Token::Type::openParenthesis, "("));
		rethrow(consume(Token::Type::symbol, "identifier"));
		Token varA = previous;
		rethrow(
			consume(Token::Type::colon, ":");
			consume(Token::Type::symbol, "identifier");
		);
		Token varB = previous;
		Local localA = resolve(varA.lexeme);
		Local localB = resolve(varB.lexeme);
		if (localA.index == - 1) {
			throw Program::Error(
				currentUnit,
				"Cannot access local variable '" + varA.lexeme +
				"'!",
				previous, ErrorCode::lgc
			);
		}
		if (localB.index == - 1) {
			throw Program::Error(
				currentUnit,
				"Cannot access local variable '" + varA.lexeme +
				"'!",
				previous, ErrorCode::lgc
			);
		}
		if (localA.type -> type != localB.type -> type) {
			throw Program::Error(
				currentUnit,
				"Swap statement doesn't support values of non matching types '" +
				Converter::typeToString(localA.type -> type) + "' and '" +
				Converter::typeToString(localB.type -> type) + "'!",
				token, ErrorCode::typ
			);
		}
		if (localA.type -> type > Type::StringType) {
			throw Program::Error(
				currentUnit,
				"Swap statement doesn't support '" +
				Converter::typeToString(localA.type -> type) + "' types!",
				token, ErrorCode::typ
			);
		}
		emitOperation({ OPCode::PSH, { .index = localA.index } });
		emitOperation({ OPCode::PSH, { .index = localB.index } });
		emitOperation(OPCode::SWP);
		rethrow(
			consume(Token::Type::closeParenthesis, ")");
			consume(Token::Type::semicolon, ";");
		);
	}
	void Compiler::sleepStatement() {
		const Token token = previous;
		rethrow(expression());
		if (popAbsoluteType() != Type::IntegerType) {
			throw Program::Error(
				currentUnit,
				"Expected Integer expression after 'sleep' command!",
				token, ErrorCode::lgc
			);
		}
		emitOperation({ OPCode::INT, { .type = (Type)Interrupt::sleep } });
	}

	SizeType Compiler::locate(String & name, Array<TypeNode *> & types) {
		const SizeType size = prototypes.size();
		for (SizeType i = 0; i < size; i += 1) {
			Prototype pro = prototypes.at(i);
			if (pro.name == name) {
				if (types.size() != pro.parameters.size()) continue;
				if (types.size() == 0) return i;
				Boolean found = true;
				for (SizeType j = 0; j < types.size(); j += 1) {
					if (!match(types.at(j), pro.parameters[j].type)) {
						found = false;
						break;
					}
				}
				if (found) return i;
			}
		}
		return - 1;
	}
	Compiler::Local Compiler::resolve(String & name) {
		// Resolve local variable:
		for (SizeType i = locals.size() - 1; i != - 1; i -= 1) {
			if (locals[i].name == name) {
				if (!locals[i].ready) {
					throw Program::Error(
						currentUnit,
						"Cannot access local variable '" + locals[i].name +
						"' in it's own definition statement!",
						previous, ErrorCode::lgc
					);
				}
				if (locals[i].index != - 1) return locals[i];
				if (locals[i].isStack) {
					const SizeType frame = routines.at(
						routineIndexes.top()
					).frame;
					locals[i].index = i - frame;
					return locals[i];
				}
				locals[i].index = i;
				return locals[i];
			}
		}
		for (Prototype & p : prototypes) {
			if (p.name == name) {
				Local local;
				local.type = new TypeNode(Type::RoutineType);
				return local;
			}
		}
		return Local();
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

	void Compiler::foldUnary(Token token) {
		if (!options.folding) return;
		const SizeType size = program -> instructions.size();
		ByteCode op = program -> instructions.at(size - 2);
		switch (op.code) {
			case OPCode::PSH:
			case OPCode::PST:
			case OPCode::PSF: break;
			default: return;
		}
		Array<ByteCode> codes;
		codes.push_back(op);
		op = program -> instructions.at(size - 1);
		if (op.as.type > Type::ImaginaryType) return;
		codes.push_back(op);
		program -> instructions.pop_back();
		program -> instructions.pop_back();
		Value v;
		try { v = Processor::self() -> fold(codes); }
		catch (Processor::Crash & c) {
			throw Program::Error(
				currentUnit,
				"Detected invalid operation '" + token.lexeme +
				"' that will cause a runtime crash!",
				token, ErrorCode::evl
			);
		}
		emitOperation({ OPCode::PSH, { .value = v } });
	}
	void Compiler::foldBinary(Token token) {
		if (!options.folding) return;
		const SizeType size = program -> instructions.size();
		Array<ByteCode> codes;
		ByteCode op;
		for (SizeType i = 3; i > 1; i -= 1) {
			op = program -> instructions.at(size - i);
			switch (op.code) {
				case OPCode::PSH:
				case OPCode::PST:
				case OPCode::PSF: break;
				default: return;
			}
			codes.push_back(op);
		}
		op = program -> instructions.at(size - 1);
		const Type typeA = (Type)(op.as.types >> 8);
		const Type typeB = (Type)(op.as.types & 0xFF);
		if (typeA > Type::ImaginaryType ||
			typeB > Type::ImaginaryType) return;
		codes.push_back(op);
		program -> instructions.pop_back();
		program -> instructions.pop_back();
		program -> instructions.pop_back();
		Value v;
		try { v = Processor::self() -> fold(codes); }
		catch (Processor::Crash & c) {
			throw Program::Error(
				currentUnit,
				"Detected invalid operation '" + token.lexeme +
				"' that will cause a runtime crash!",
				token, ErrorCode::evl
			);
		}
		emitOperation({ OPCode::PSH, { .value = v } });
	}

	Compiler::ParseRule Compiler::getRule(Token::Type token) {
		auto search = rules.find(token);
		if (search == rules.end()) {
			return { nullptr, nullptr, Precedence::none };
		}
		return search -> second;
	}

	inline void Compiler::pushType(Type type) {
		typeStack.push(new TypeNode(type));
	}
	inline void Compiler::pushType(TypeNode * node) {
		typeStack.push(node);
	}
	inline void Compiler::decreaseType() {
		delete typeStack.pop();
	}
	inline Type Compiler::popAbsoluteType() {
		TypeNode * node = typeStack.top();
		Type type = node -> type;
		decreaseType();
		return type;
	}
	inline Compiler::TypeNode * Compiler::popType() {
		if (typeStack.isEmpty()) return nullptr;
		return typeStack.pop();
	}
	inline Compiler::TypeNode * Compiler::topType() {
		if (typeStack.isEmpty()) return nullptr;
		return typeStack.top();
	}
	inline Boolean Compiler::match(Compiler::TypeNode * a, Compiler::TypeNode * b) {
		if (a -> isContainer() && b -> isContainer()) {
			while (a && b) {
				if ((a -> type) != (b -> type)) return false;
				a = a -> next; b = b -> next;
			}
			if (!a && !b) return true;
			return false;
		}
		if (a -> type == Type::LamdaType || b -> type == Type::LamdaType) {
			if (a -> type != b -> type) return false;
			LamdaType * aL = (LamdaType *)(a -> getData());
			LamdaType * bL = (LamdaType *)(b -> getData());
			if (!match(aL -> returnType, bL -> returnType)) return false;
			if (aL -> parameters.size() != bL -> parameters.size()) return false;
			const SizeType size = aL -> parameters.size();
			for (SizeType i = 0; i < size; i += 1) {
				if (!match(aL -> parameters[i], bL -> parameters[i])) return false;
			}
			return true;
		}
		return (a -> type) == (b -> type);
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
	inline void Compiler::prototypeRoutine(Boolean function) {
		advance();
		Prototype prototype;
		try {
			consume(Token::Type::symbol, "identifier");
			prototype.name = previous.lexeme;
			consume(Token::Type::openParenthesis, "(");
			if (!check(Token::Type::closeParenthesis)) {
				Parameter parameter;
				do {
					consume(Token::Type::symbol, "identifier");
					parameter.name = previous.lexeme;
					consume(Token::Type::colon, ":");
					parameter.type = type();
					prototype.parameters.push_back(parameter);
				} while (match(Token::Type::comma));
			}
			consume(Token::Type::closeParenthesis, ")");
			if (function) {
				consume(Token::Type::colon, ":");
				prototype.returnType = type();
			} else prototype.returnType = new TypeNode(Type::VoidType);
		} catch (Program::Error & e) { return; }
		Array<TypeNode *> overloading;
		for (Parameter p : prototype.parameters) {
			overloading.push_back(p.type);
		}
		if (locate(prototype.name, overloading) != - 1) return;
		prototypes.push_back(prototype);
	}
	inline void Compiler::preparePrototypes() {
		while (!match(Token::Type::endFile)) {
			switch (current.type) {
				case Token::Type::procKeyword: prototypeRoutine(false); break;
				case Token::Type::funcKeyword: prototypeRoutine(true);  break;
				default: break;
			}
			advance();
		}
		// Resetting the environment:
		index = 1;
		advance();
		previous = tokens -> at(1);
	}
	inline void Compiler::resolveRoutines() {
		const SizeType size = routines.size();
		for (SizeType i = 0; i < size; i += 1) {
			Routine routine = routines[i];
			if (routine.name.empty()) {
				if (options.sectors) emitRest();
				program -> instructions.at(lamdas[i]).as.index = sourcePosition();
				pasteCodes(routine.code);
				continue;
			}
			const SizeType j = locate(
				routine.name,
				routine.parameters
			);
			if (j == - 1) continue;
			if (options.sectors) emitRest();
			prototypes.at(j).address = sourcePosition();
			pasteCodes(routine.code);
		}
	}
	inline void Compiler::resolveCalls() {
		for (ByteCode & byte : program -> instructions) {
			if (byte.code != OPCode::CAL) continue;
			byte.as.index = prototypes.at(byte.as.index).address;
		}
	}
	inline void Compiler::resolveJumps() {
		const SizeType size = program -> instructions.size();
		for (SizeType i = 0; i < size; i += 1) {
			switch (program -> instructions[i].code) {
				case OPCode::JMP:
				case OPCode::JIF:
				case OPCode::JIT:
				case OPCode::JAF:
				case OPCode::JAT:
					program -> instructions[i].as.index = (
						(Int64)program -> instructions[
							i
						].as.index + i
					);
				break;
				default: continue;
			}
		}
	}
	inline SizeType Compiler::countLocals(SizeType scope) {
		SizeType localCount = 0;
		for (Int64 i = locals.size() - 1; i >= 0; i -= 1) {
			if (locals[i].depth <= scope ||
				locals[i].depth == 0) break;
			localCount += 1;
		}
		return localCount;
	}
	inline SizeType Compiler::sourcePosition() {
		return program -> instructions.size();
	}
	inline void Compiler::emitOperation(ByteCode code) {
		program -> instructions.push_back(code);
	}
	inline void Compiler::emitOperation(OPCode code) {
		program -> instructions.push_back({ code, {
			.value = { .integer = 0 } }
		});
	}
	inline void Compiler::emitString(String s) {
		if (s.empty()) {
			emitOperation(OPCode::PES);
			return;
		}
		auto search = strings.find(s);
		if (search != strings.end()) {
			emitOperation({ OPCode::STR, { .index = search -> second } });
			return;
		}
		const SizeType position = program -> strings.size();
		strings.insert({ s, index });
		program -> strings.push_back(s);
		emitOperation({ OPCode::STR, { .index = position } });
	}
	inline void Compiler::emitJumpBack(SizeType jmb) {
		jmb = sourcePosition() - jmb;
		emitOperation({ OPCode::JMP, { .index = - jmb } });
	}
	inline SizeType Compiler::emitJumpNext(OPCode code) {
		const SizeType count = sourcePosition();
		emitOperation(code);
		return count;
	}
	inline void Compiler::patchJumpNext(SizeType jmp) {
		const SizeType jump = (sourcePosition() - jmp);
		program -> instructions[jmp].as.index = jump;
	}
	inline void Compiler::patchJumpBack(SizeType pos, SizeType jmb) {
		jmb = pos - jmb;
		program -> instructions[pos].as.index = - jmb;
	}
	inline void Compiler::patchOperation(SizeType op, OPCode code) {
		program -> instructions[op].code = code;
	}
	inline Array<ByteCode> Compiler::cutCodes(SizeType cut) {
		const SizeType position = sourcePosition();
		if (cut >= position) return { };
		Array<ByteCode> codes(
			program -> instructions.begin() + cut,
			program -> instructions.end()
		);
		program -> instructions.erase(
			program -> instructions.begin() + cut,
			program -> instructions.end()
		);
		codes.shrink_to_fit();
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
	inline void Compiler::beginVirtualScope() {
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
		emitPop(localCount);
	}
	inline void Compiler::endVirtualScope() {
		scopeDepth -= 1;
		while (locals.size() > 0 &&
			   locals[locals.size() - 1].depth > scopeDepth) {
			locals.pop_back();
		}
	}
	inline SizeType Compiler::emitRest() {
		const SizeType count = sourcePosition();
		emitOperation(OPCode::RST);
		return count;
	}
	inline void Compiler::emitCall(SizeType i) {
		emitOperation({ OPCode::CAL, { .index = i } });
	}
	inline void Compiler::emitPop(SizeType n) {
		if (!n) return;
		if (n == 1) emitOperation(OPCode::POP);
		else emitOperation({ OPCode::DSK, { .index = n } });
	}

	void Compiler::reset() {
		index = 0;
		assignmentStack.clear();
		locals.clear();
		scopeDepth = 0;
		cycleScopes.clear();
		breakStack.clear();
		routineIndexes.clear();
		strings.clear();
		prototypes.clear();
		lamdas.clear();
		TypeNode::resetNodes();
	}

	Program * Compiler::compile(SourceCode * source, Options options) {
		this -> options = options;
		return compile(source);
	}
	Program * Compiler::compile(SourceCode * source) {
		if (!source) return nullptr;
		currentUnit = source -> main;
		if (!currentUnit) return nullptr;
		tokens = source -> main -> tokens;
		if (!tokens) return nullptr;
		program = new Program();

		reset();
		advance();
		rethrow(consume(Token::Type::beginFile, "Begin File"));

		preparePrototypes();

		rethrow(
			beginScope();
			while (!match(Token::Type::endFile)) declaration();
			endScope();
			emitOperation(OPCode::HLT);
		);

		resolveRoutines();
		resolveCalls();
		resolveJumps();

		reset();

		program -> instructions.shrink_to_fit();
		program -> strings.shrink_to_fit();

		return program;
	}

}

#undef rethrow

#endif
