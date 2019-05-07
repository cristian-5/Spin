
/*!
 *
 *    + --------------------------------------- +
 *    |  SyntaxRule.hpp                         |
 *    |                                         |
 *    |                SyntaxRule               |
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

#ifndef STACKSYNTAXRULE
#define STACKSYNTAXRULE

#include <iostream>
#include <string>

#include "../Collection/Collection.hpp"

#include "../Token/Token.hpp"

using String = std::string;
using UInt32 = std::uint32_t;

using Boolean = bool;

/*! @brief Namespace Stack */
namespace Stack {

	class SyntaxRule {

	private:

		Boolean selfRef = false;

	public:

		StrongList<SyntaxRule *> nextRules = StrongList<SyntaxRule *>();

		Token * token = new Token();

		SyntaxRule() { }

		SyntaxRule(StrongList<SyntaxRule *> & r, Token * t) {
			nextRules = r;
			token = t;
		}
		SyntaxRule(SyntaxRule * r, Token * t) {
			nextRules.link(r);
			token = t;
		}
		SyntaxRule(Token * t) { token = t; }

		void addNextRule(SyntaxRule * r) {
			if (r == this) selfRef = true;
			nextRules.link(r);
		}

		Boolean matches(Token t) { return t.type == token -> type; }

		Boolean isSelfRef() { return selfRef; }

		Boolean isTerminal() { return nextRules.isEmpty(); }

	};

	class Grammar {

	public:

		StrongList<SyntaxRule *> nextRules = StrongList<SyntaxRule *>();

		Grammar() { }

		Grammar(StrongList<SyntaxRule *> & r) {
			nextRules = r;
		}
		Grammar(SyntaxRule * & r) {
			nextRules.link(r);
		}

		Boolean isEmpty() { return nextRules.isEmpty(); }

	};

}

#endif
