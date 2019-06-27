
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

#include "../Aliases/Aliases.hpp"

namespace Stack {

	enum TokenType {

		beginFile,

		empty,
		comment,

		basicType,
		customType,
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
		String lexeme = "";
		UInt32 position = 0;

		Token() { }

		Token(String l, TokenType t, UInt32 p = 0) {
			lexeme = l; type = t; position = p;
		}

		/*Boolean isTypeTokenType() {
			if (type != symbol) return false;
			return lexeme == "Int8"      ||
				   lexeme == "Int16"     ||
				   lexeme == "Int32"     ||
				   lexeme == "Int64"     ||
				   lexeme == "UInt8"     ||
				   lexeme == "UInt16"    ||
				   lexeme == "UInt32"    ||
				   lexeme == "UInt64"    ||
				   lexeme == "Character" ||
				   lexeme == "Float"     ||
				   lexeme == "Double"    ||
				   lexeme == "Real"      ||
				   lexeme == "Boolean"   ||
				   lexeme == "String";
		}*/

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
