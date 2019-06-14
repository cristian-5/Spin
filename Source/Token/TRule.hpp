
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

#include "../Aliases/Aliases.hpp"

#include "Token.hpp"

namespace Stack {

	/*! @brief Class TokenRule. */
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
