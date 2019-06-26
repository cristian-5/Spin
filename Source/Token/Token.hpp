
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

#include "PData.hpp"

namespace Stack {

	enum TokenType {

		beginFile,

		empty,
		comment,

		basicType,
		symbol,
		identifier,

		uIntLiteral,
		intLiteral,
		stringLiteral,
		charLiteral,
		boolLiteral,
		realLiteral,
		colourLiteral,
		nullLiteral,

		bra_ketLiteral,
		braLiteral,
		ketLiteral,

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
		returnKeyword,

		prefixOperator,
		infixOperator,
		postfixOperator,

		nop,

		endFile

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

		Boolean isTypeTokenType() {
			if (type != symbol) return false;
			return value == "Int8"      ||
				   value == "Int16"     ||
				   value == "Int32"     ||
				   value == "Int64"     ||
				   value == "UInt8"     ||
				   value == "UInt16"    ||
				   value == "UInt32"    ||
				   value == "UInt64"    ||
				   value == "Character" ||
				   value == "Float"     ||
				   value == "Double"    ||
				   value == "Real"      ||
				   value == "Boolean"   ||
				   value == "String";
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

}

#endif
