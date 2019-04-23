
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

		equals,
		different,
		leftRotation,
		rightRotation,
		leftShift,
		rightShift,
		minor,
		major,
		equal,
		questionMark,
		exclamationMark,

		plusplus,
		plusEqual,
		plus,
		minusminus,
		minusEqual,
		minus,
		starEqual,
		star,
		backslash,
		slashEqual,
		slash,
		at,
		hashtag,
		ampersand,
		musulusEqual,
		modulus,
		dollar,
		hat,

		openRoundBracket,
		closeRoundBracket,

		openSquareBracket,
		closeSquareBracket,

		openCurlyBracket,
		closeCurlyBracket

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
