
#include "../Aliases.hpp"

#ifndef SPIN_TOKEN_HPP
#define SPIN_TOKEN_HPP

#include <regex>

using Regex = std::regex;

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

		virtualOperator,

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
		dollarEqual,
		modulusEqual,
		star,
		slash,
		ampersand,
		dollar,
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
		Bool isTypeLiteral() const;
		Bool isTypeNumeral() const;
		Bool isTypeBasicType() const;
		Bool isTypeType() const;
		Bool isTypeBraKet() const;
	};

	struct TokenRule {
		Regex pattern;
		TokenType type;
	};

}

#endif
