
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

using std::string = String;

/*! @brief Namespace Stack */
namespace StackCompiler {

	enum TokenType {

		empty,
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
		String token = "";

		Token(String token, TokenType type) {
			this.token = token;
			this.type = type;
		}

	};

}

#endif
