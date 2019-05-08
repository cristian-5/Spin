
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
using Exception = std::exception;
using Boolean = bool;

using namespace Collection;

/*! @brief Namespace Stack */
namespace Stack {

	class SyntaxRule {

		private:

		Boolean selfRef = false;

		public:

		StrongList<SyntaxRule *> nextRules = StrongList<SyntaxRule *>();

		Token * token = nullptr;

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

		Boolean matches(Token * t) {
			return t -> type == token -> type;
		}

		SyntaxRule * nextThatMatches(Token * t) {
			for (UInt32 i = 0; i < nextRules.count(); i++) {
				if (nextRules[i] -> matches(t)) {
					return nextRules[i];
				}
			}
			return nullptr;
		}

		Boolean isSelfRef() { return selfRef; }

		Boolean isTerminal() { return nextRules.isEmpty(); }

		Boolean isEmpty() { return nextRules.isEmpty(); }

	};

	/*!
	 *   @brief SyntaxErrorException.
	 *   Raised when a token is unexcepted.
	 *   @author Cristian A.
	 */
	class SyntaxErrorException: public Exception {

		private:

		Token token = Token();

		public:

		Token getToken() { return token; }

		SyntaxErrorException(Token t):
		Exception(), token(t) { }

	};

	class Grammar {

		private:

		SyntaxRule * rules = new SyntaxRule();

		public:

		Grammar() { }

		Grammar(StrongList<SyntaxRule *> & r) {
			rules -> nextRules = r;
		}
		Grammar(SyntaxRule * r) {
			rules -> nextRules.link(r);
		}

		void addRule(SyntaxRule * r) {
			rules -> nextRules.link(r);
		}

		Boolean consume(StrongList<Token> * t) {
			if (rules -> isEmpty()) return false;
			if (t -> isEmpty()) return false;
			// First time: try to match and
			// return false if it doesn't.
			Token token = t -> getNode(0);
			SyntaxRule * cur = rules -> nextThatMatches(& token);
			if (cur == nullptr) return false;
			// Deep consume the rule and throw error
			// if the next token is unexcepted.
			while (t -> count() > 0) {
				token = t -> getNode(0);
				cur = rules -> nextThatMatches(& token);
				if (cur == nullptr) {
					throw SyntaxErrorException(token);
				}
			}
			return true;
		}

		Boolean isEmpty() { return rules -> isEmpty(); }

	};

}

#endif
