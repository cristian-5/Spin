
/*!
 *
 *    + --------------------------------------- +
 *    |  Token.hpp                              |
 *    |                                         |
 *    |                  Token                  |
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

#ifndef STACKTOKEN
#define STACKTOKEN

#include <iostream>
#include <string>

#include "../Aliases/Aliases.hpp"

namespace Stack {

	enum TokenType {

		beginFile,

		empty,
		comment,

		type,
		symbol,
		identifier,

		uIntLiteral,
		intLiteral,
		stringLiteral,
		charLiteral,
		boolLiteral,
		realLiteral,

		colon,
		semicolon,
		comma,
		dot,

		minor,
		major,
		equal,
		questionMark,
		exclamationMark,

		plus,
		minus,
		star,
		backslash,
		slash,
		at,
		pipe,
		hashtag,
		ampersand,
		modulus,
		dollar,
		hat,

		openRoundBracket,
		closeRoundBracket,

		openSquareBracket,
		closeSquareBracket,

		openCurlyBracket,
		closeCurlyBracket,

		tryKeyword,
		catchKeyword,
		throwKeyword,
		throwsKeyword,
		avoidKeyword,

		ifKeyword,
		elseKeyword,
		switchKeyword,
		caseKeyword,
		defaultKeyword,
		whileKeyword,
		doKeyword,
		loopKeyword,
		forKeyword,
		repeatKeyword,
		untilKeyword,
		breakKeyword,
		continueKeyword,

		libKeyword,
		importKeyword,
		funcKeyword,
		procKeyword,
		staticKeyword,
		classKeyword,
		enumKeyword,
		structKeyword,
		exceptKeyword,
		privateKeyword,
		publicKeyword,
		refKeyword,
		cpyKeyword,
		constKeyword,
		nullKeyword,
		returnKeyword,

		prefixOperator,
		infixOperator,
		postfixOperator,

		nop,

		endFile,

		everything

	};

	class Token {

		public:

		TokenType type = empty;
		String value = "";
		UInt32 position = 0;

		Token() { }

		Token(String v, TokenType t, UInt32 p = 0) {
			value = v; type = t; position = p;
		}

	};

	class TokenRule {

		public:

		TokenType type = empty;
		String pattern = "";

		TokenRule() { }

		TokenRule(String p, TokenType t) {
			pattern = p; type = t;
		}

	};

	enum ParseDataType {
		UInt8Type,
		UInt16Type,
		UInt32Type,
		UInt64Type,
		Int8Type,
		Int16Type,
		Int32Type,
		Int64Type,
		CharacterType,
		BooleanType,
		FloatType,
		DoubleType,
		RealType,
		StringType,
		ArrayType,
		FunctionType,  // Not Sure if we need proc
		ClassType,
		VoidType, // Not Sure
		AnyType,
		InvalidType
	};

	union Data {
		UInt64 integer;
		Real real;
		void * allocated;
	};

	struct ParseData {
		ParseDataType type;
		Data value;
	};

}

#endif
