
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

#include "ASTree.hpp"

using namespace Collection;

/*! @brief Namespace Stack. */
namespace Stack {

	/*!
	 *   @brief Invalid Grammar Exception.
	 *   Raised when the grammar is not invaid.
	 */
	class InvalidGrammarException: public Exception {
		public: InvalidGrammarException(): Exception() { }
	};

	/*!
	 *   @brief Empty Grammar Exception.
	 *   Raised when the grammar is empty.
	 */
	class EmptyGrammarException: public Exception {
		public: EmptyGrammarException(): Exception() { }
	};

	/*!
	 *   @brief Syntax Error Exception.
	 *   Raised when a token is unexpected.
	 */
	class SyntaxErrorException: public Exception {
		private: Token token = Token();
		public: Token getToken() { return token; }
		SyntaxErrorException(Token t):
		Exception(), token(t) { }
	};

	/*!
	 *   @brief Parser Class.
	 *   Used to parse a grammar.
	 */
	class Parser {

		private:

		StrongList<Token> tokens = StrongList<Token>();

		SRule * grammarRules;

		Boolean parse(SRule * r, UInt32 index) {
			if (index >= tokens.count()) return false;
			Token * t = & tokens[index];
			Boolean matches = r -> matches(t);
			if (r -> isTerminal()) return matches;
			if (!matches) return false;
			index += 1;
			for (UInt32 j = 0; j < r -> nextRules.count(); j++) {
				if (parse(r -> nextRules[j], index)) return true;
			}
			return false;
		}

		public:

		Parser(StrongList<Token> * t, Grammar * g) {
			tokens = * t;
			if (g -> isEmpty()) {
				throw EmptyGrammarException();
			}
			grammarRules = g;
		}

		AbstractSyntaxTree parse() {
			AbstractSyntaxTree ast = AbstractSyntaxTree();
			std::cout << (parse(grammarRules, 0) ? "p" : "!") << std::endl;
			return ast;
		}

	};

}

#endif
