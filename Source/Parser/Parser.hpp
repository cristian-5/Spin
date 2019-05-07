
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

#include "AST.hpp"

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

	/*!
	 *   @brief Empty Grammar Exception.
	 *   Raised when the grammar is empty.
	 *   @author Cristian A.
	 */
	class EmptyGrammarException: public Exception {
	public: EmptyGrammarException(): Exception() { }
	};

	class Parser {

	private:

		StrongList<Token> tokens = StrongList<Token>();

		Grammar grammarRules = Grammar();

		//HeapStack<String> stack = HeapStack<String>();

		void parse(StrongList<Token> & tokens, AbstractSyntaxTree & ast) {
			
		}

	public:

		Parser(StrongList<Token> * t, Grammar & g) {
			tokens = * t;
			if (g.isEmpty()) throw EmptyGrammarException();
			grammarRules = g;
		}

		AbstractSyntaxTree parse() {
			AbstractSyntaxTree ast = AbstractSyntaxTree();
			parse(tokens, ast);
			return ast;
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
