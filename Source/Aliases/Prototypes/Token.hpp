
#include "../Aliases.hpp"

#ifndef SPIN_TOKEN_HPP
#define SPIN_TOKEN_HPP

namespace Spin {

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
		ketbraSymbol,

		intLiteral,
		imaginaryLiteral,
		basisBraLiteral,
		basisKetLiteral,
		realLiteral,
		stringLiteral,
		charLiteral,
		boolLiteral,

		arrow,
		doublecolon,
		semicolon,
		colon,
		comma,
		dot,

		/* Low Priority */

		plusEqual,
		minusEqual,
		pipeEqual,
		plus,
		minus,
		pipe,

		/* Medium Priority */

		starEqual,
		slashEqual,
		ampersandEqual,
		hatEqual,
		modulusEqual,
		star,
		slash,
		ampersand,
		hat,
		modulus,

		minor,
		minorEqual,
		major,
		majorEqual,
		equal,
		equality,
		inequality,
		questionMark,
		exclamationMark,

		AND,
		OR,

		conjugate,
		transpose,
		dagger,

		tilde,
		backslash,

		openParenthesis,
		closeParenthesis,

		openBracket,
		closeBracket,

		openBrace,
		closeBrace,

		ifKeyword,
		elseKeyword,
		swapKeyword,
		whileKeyword,
		doKeyword,
		loopKeyword,
		forKeyword,
		repeatKeyword,
		untilKeyword,
		breakKeyword,
		continueKeyword,
		selfKeyword,

		importKeyword,

		funcKeyword,
		procKeyword,

		classKeyword,

		publicModifier,
		hiddenModifier,
		secureModifier,
		immuneModifier,
		staticModifier,
		sharedModifier,

		createSpecifier,
		deleteSpecifier,

		returnKeyword,
		restKeyword,

		newKeyword,
		deleteKeyword,

		invalid,

		endFile

	};

	class Token {
		public:
		String lexeme;
		TokenType type;
		SizeType position;
		Token(String lexeme,
			  TokenType type,
			  SizeType position = 0);
		Boolean isTypeLiteral() const;
		Boolean isTypeNumeral() const;
		Boolean isTypeBasicType() const;
		Boolean isTypeType() const;
		Boolean isTypeBraKet() const;
	};

}

#endif
