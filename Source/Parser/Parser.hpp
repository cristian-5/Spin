
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

#include "../Collection/Collection.hpp"

#include "../Syntax/SRule.hpp"

#include "ASTree.hpp"

using String = std::string;
using UInt32 = std::uint32_t;
using Exception = std::exception;

#define Boolean bool

using namespace Collection;

/*! @brief Namespace Stack */
namespace Stack {

	/*!
	 *   @brief Invalid Grammar Exception.
	 *   Raised when the grammar is not invaid.
	 *   @author Cristian A.
	 */
	class InvalidGrammarException: public Exception {
		public: InvalidGrammarException(): Exception() { }
	};

	/*!
	 *   @brief Empty Grammar Exception.
	 *   Raised when the grammar is empty.
	 *   @author Cristian A.
	 */
	class EmptyGrammarException: public Exception {
		public: EmptyGrammarException(): Exception() { }
	};

	/*!
	 *   @brief SyntaxErrorException.
	 *   Raised when a token is unexcepted.
	 *   @author Cristian A.
	 */
	class SyntaxErrorException: public Exception {
		private: Token token = Token();
		public: Token getToken() { return token; }
		SyntaxErrorException(Token t):
		Exception(), token(t) { }
	};

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
