
/*!
 *
 *    + --------------------------------------- +
 *    |  Token.cpp                              |
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
!*/

#include "../Aliases/Prototypes/Token.hpp"

#ifndef SPIN_TOKEN
#define SPIN_TOKEN

namespace Spin {

	Token::Token(String lexeme, TokenType type, SizeType position) {
		this -> lexeme = lexeme;
		this -> type = type;
		this -> position = position;
	}
	Bool Token::isTypeLiteral() const {
		return type >= TokenType::intLiteral &&
			   type <= TokenType::boolLiteral;
	}
	Bool Token::isTypeNumeral() const {
		return type >= TokenType::intLiteral &&
			   type <= TokenType::realLiteral;
	}
	Bool Token::isTypeBasicType() const {
		return type == TokenType::basicType;
	}
	Bool Token::isTypeType() const {
		return type == TokenType::classKeyword;
	}
	Bool Token::isTypeBraKet() const {
		return type >= TokenType::braketSymbol &&
			   type <= TokenType::ketbraSymbol;
	}

}

#endif
