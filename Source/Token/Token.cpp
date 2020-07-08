
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

#include "Token.hpp"

#ifndef SPIN_TOKEN
#define SPIN_TOKEN

namespace Spin {

	Token::Token(String lexeme, Type type, SizeType position) {
		this -> lexeme = lexeme;
		this -> type = type;
		this -> position = position;
	}
	Boolean Token::isTypeLiteral() const {
		return type >= Type::intLiteral &&
			   type <= Type::boolLiteral;
	}
	Boolean Token::isTypeNumeral() const {
		return type >= Type::intLiteral &&
			   type <= Type::realLiteral;
	}
	Boolean Token::isTypeBasicType() const {
		return type == Type::basicType;
	}
	Boolean Token::isTypeType() const {
		return type == Type::classKeyword;
	}
	Boolean Token::isTypeBraKet() const {
		return type >= Type::braketSymbol &&
			   type <= Type::ketbraSymbol;
	}

}

#endif
