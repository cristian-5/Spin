
/*!
 *
 *    + --------------------------------------- +
 *    |  TRule.hpp                              |
 *    |                                         |
 *    |                Token Rule               |
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

#ifndef STACKRULE
#define STACKRULE

#include <iostream>
#include <string>

#include "Token.hpp"

using String = std::string;

/*! @brief Namespace Stack. */
namespace Stack {

	/*! @brief TokenRule Class. */
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
