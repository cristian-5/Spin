
/*!
 *
 *    + --------------------------------------- +
 *    |  Parser.hpp                             |
 *    |                                         |
 *    |                  Parser                 |
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

#ifndef STACKPARSER
#define STACKPARSER

#include <iostream>
#include <string>

#include "../Aliases/Aliases.hpp"
#include "../Collection/Collection.hpp"
#include "../Syntax/SRule.hpp"

using namespace Collection;

namespace Stack {

	/*!
	 *   @brief Invalid Grammar Exception.
	 *   Raised when the grammar is not invaid.
	 */
	class InvalidGrammarException: public Exception {
		public: InvalidGrammarException(): Exception() { }
	};

	/*!
	 *   @brief Invalid Pointer Exception.
	 *   Raised when the pointer is not invaid.
	 */
	class InvalidPointerException: public Exception {
		public: InvalidPointerException(): Exception() { }
	};

	/*!
	 *   @brief Empty Grammar Exception.
	 *   Raised when the grammar is empty.
	 */
	class EmptyGrammarException: public Exception {
		public: EmptyGrammarException(): Exception() { }
	};

	/*!
	 *   @brief Empty Token Exception.
	 *   Raised when the grammar is empty.
	 */
	class EmptyTokenException: public Exception {
		public: EmptyTokenException(): Exception() { }
	};

	/*!
	 *   @brief Parser Class.
	 *   Used to parse a grammar.
	 */
	class Parser {

		private:

		StrongList<Token> * tokens = nullptr;

		Grammar * grammar = nullptr;

		UInt32 lastTerminal = 0;

		Boolean parse(SRule * r, UInt32 index) {
			if (index >= tokens -> count()) return false;
			Token t = tokens -> getNode(index);
			Boolean matches = r -> matches(& t);
			if (r -> isTerminal()) {
				if (matches) lastTerminal = index;
				return matches;
			}
			if (!matches) return false;
			index += 1;
			for (UInt32 j = 0; j < r -> nextRules.count(); j++) {
				if (parse(r -> nextRules[j], index)) return true;
			}
			return false;
		}

		public:

		Parser(StrongList<Token> * t, Grammar * g) {
			if (t == nullptr) throw InvalidPointerException();
			if (g == nullptr) throw InvalidPointerException();
			if (t -> isEmpty()) throw EmptyTokenException();
			if (g -> isEmpty()) throw EmptyGrammarException();
			tokens = t; grammar = g;
		}

		Boolean parseFrom(UInt32 start = 0) {
			return parse(grammar, start);
		}

		Boolean parseRange(UInt32 start, UInt32 & end) {
			Boolean result = parse(grammar, start);
			end = lastTerminal;
			return result;
		}

	};

}

#endif
