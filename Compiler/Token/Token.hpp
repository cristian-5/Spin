
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

using String = std::string;
using UInt32 = std::uint32_t;

/*! @brief Namespace Stack */
namespace Stack {

	enum TokenType {

		newFile,

		empty,
		identifier,
		comment,

		integerLiteral,
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

		funcKeyword,
		procKeyword,
		staticKeyword,
		classKeyword,
		enumKeyword,
		structKeyword,
		exceptKeyword,
		privateKeyword,
		publicKeyword,
		inoutKeyword,
		frozenKeyword,
		nullKeyword,
		returnKeyword,

		prefixOperator,
		infixOperator,
		postfixOperator,

		nop,

		endOfFile

	};

	/*! @brief Token Class. */
	class Token {

	public:

		TokenType type = empty;
		String value = "";

		UInt32 position = 0;

		Token() { }

		Token(String token, TokenType type, UInt32 pos) {
			this -> value = token;
			this -> type = type;
			this -> position = pos;
		}

	};

}

#endif
