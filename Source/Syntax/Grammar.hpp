
/*!
 *
 *    + --------------------------------------- +
 *    |  Grammar.hpp                            |
 *    |                                         |
 *    |                 Grammar                 |
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

#ifndef STACKGRAMMAR
#define STACKGRAMMAR

#include <iostream>
#include <string>

#include "../Collection/Collection.hpp"

#include "../Token/Token.hpp"

using String = std::string;
using UInt32 = std::uint32_t;

using Boolean = bool;

/*! @brief Namespace Stack */
namespace Stack {

	class Grammar;
	class Rule;

	class Rule {

	public:

		StrongList<Grammar *> nextGrammars = StrongList<Grammar *>();
		StrongList<Rule *> nextRules = StrongList<Rule *>();

		Token * token = new Token();

		Rule() { }

		Rule(StrongList<Rule *> & r, Token * t) {
			nextRules = r;
			token = t;
		}
		Rule(StrongList<Grammar *> & g, Token * t) {
			nextGrammars = g;
			token = t;
		}
		Rule(StrongList<Rule *> & r,
		     StrongList<Grammar *> & g,
			 Token * t) {
			nextRules = r;
			nextGrammars = g;
			token = t;
		}
		Rule(Rule * r, Token * t) {
			nextRules.link(r);
			token = t;
		}
		Rule(Grammar * g, Token * t) {
			nextGrammars.link(g);
			token = t;
		}
		Rule(Rule * r, Grammar * g, Token * t) {
			nextRules.link(r);
			nextGrammars.link(g);
			token = t;
		}
		Rule(Token * t) { token = t; }

		Boolean matches(Token t) { return t.type == token -> type; }

		Boolean isTerminal() {
			return nextGrammars.isEmpty() && nextRules.isEmpty();
		}

	};

	class Grammar {

	public:

		StrongList<Grammar *> nextGrammars = StrongList<Grammar *>();
		StrongList<Rule *> nextRules = StrongList<Rule *>();

		Grammar() { }

		Grammar(StrongList<Grammar *> & g) {
			nextGrammars = g;
		}
		Grammar(StrongList<Rule *> & r) {
			nextRules = r;
		}
		Grammar(StrongList<Grammar *> & g,
				StrongList<Rule *> & r) {
			nextGrammars = g;
			nextRules = r;
		}

	};

}

#endif
