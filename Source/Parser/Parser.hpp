
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

#include "../Syntax/SyntaxRule.hpp"

//#include "AST.hpp"

using String = std::string;
using UInt32 = std::uint32_t;
using Exception = std::exception;

#define Boolean bool

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

	class Parser {

	private:

		StrongList<Token> tokens = StrongList<Token>();

		StrongList<SyntaxRule *> grammarRules = StrongList<SyntaxRule *>();

		//HeapStack<String> stack = HeapStack<String>();

	public:

		Parser(StrongList<Token> * t, StrongList<SyntaxRule *> & r) {
			tokens = * t;
			if (r.isEmpty()) throw InvalidGrammarException();
			grammarRules = r;
		}

		/*Boolean parse(AbstractSyntaxTree & ast) {
			if (tokens.isEmpty()) return ast;

			Token currentToken = tokens[0];

			stack.push(grammar -> name);

			for (UInt32 i = 0; i < grammar -> nextSyntaxRules.count(); i++) {

				SyntaxRule currentSyntaxRule = grammar -> nextSyntaxRules[i];
				while (currentSyntaxRule.matches(currentToken)) {
					if (currentSyntaxRule.isTerminal()) {
						// Stop
						break;
					}
					currentSyntaxRule
				}


			}

		}*/

	};

}

#endif
