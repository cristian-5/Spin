
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

		braketSymbol,
		braSymbol,
		ketSymbol,

		intLiteral,
		stringLiteral,
		charLiteral,
		boolLiteral,
		imaginaryLiteral,
		realLiteral,
		colourLiteral,
		nullLiteral,

		colon,
		semicolon,
		comma,
		dot,

		diamond,

		plusEqual,
		minusEqual,
		starEqual,
		slashEqual,
		tildeEqual,
		pipeEqual,
		ampersandEqual,
		modulusEqual,
		hatEqual,

		minor,
		minorEqual,
		major,
		majorEqual,
		equal,
		equality,
		inequality,
		questionMark,
		exclamationMark,

		plus,
		minus,
		tilde,
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

		printKeyword,

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
		constKeyword,
		returnKeyword,
		nevermind,

		refKeyword,
		cpyKeyword,

		invalid,

		endFile

	};

	class Token {

		public:

		TokenType type = TokenType::empty;
		String lexeme = "";
		UInt32 position = 0;

		Token() { }

		Token(String l, TokenType t, UInt32 p = 0) {
			lexeme = l; type = t; position = p;
		}

		Bool isTypeLiteral() const {
			return type >= TokenType::intLiteral &&
				   type <= TokenType::nullLiteral;
		}

	};

	class TokenRule {

		public:

		TokenType type = TokenType::empty;
		String pattern = "";

		TokenRule() { }

		TokenRule(String p, TokenType t) {
			pattern = p; type = t;
		}

	};

}

#endif
