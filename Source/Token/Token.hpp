
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

#include "../Aliases/Prototypes.hpp"

namespace Stack {

	Token::Token(String l, TokenType t, UInt32 p) {
		lexeme = l; type = t; position = p;
	}
	Bool Token::isTypeLiteral() const {
		return type >= TokenType::intLiteral &&
			   type <= TokenType::emptyLiteral;
	}
	Bool Token::isTypeBasicType() const {
		return type == TokenType::basicType;
	}
	Bool Token::isTypeType() const {
		return type >= TokenType::classKeyword &&
			   type <= TokenType::exceptKeyword;
	}
	Bool Token::isTypeBraKet() const {
		return type >= TokenType::braSymbol &&
			   type <= TokenType::ketSymbol;
	} 

	TokenRule::TokenRule(String p, TokenType t) {
		pattern = p; type = t;
	}

}

#endif
