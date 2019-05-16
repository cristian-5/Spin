
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

/*! @brief Namespace Stack. */
namespace Stack {

	enum TokenType {

		beginFile,

		empty,
		comment,

		type,
		symbol,
		identifier,

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
		constKeyword,
		nullKeyword,
		returnKeyword,

		prefixOperator,
		infixOperator,
		postfixOperator,

		nop,

		endFile

	};

	/*! @brief Token Class. */
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

}

#endif
