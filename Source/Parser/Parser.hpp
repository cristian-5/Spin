
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

#include "../Syntax/Grammar.hpp"

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

		Rule * grammarRules = nullptr;

		//HeapStack<String> stack = HeapStack<String>();

		Rule * grammarToRules(Rule * r) {
			if (r -> isTerminal()) return r;
			else {
				Rule * store = new Rule();
				for (UInt32 i = 0; i < r -> nextGrammars.count(); i++) {
					Rule * tmp = grammarToRules(r -> nextGrammars[i]);
					store -> nextRules.link(tmp);
				}
				for (UInt32 i = 0; i < r -> nextRules.count(); i++) {
					Rule * tmp = grammarToRules(r -> nextRules[i]);
					store -> nextRules.link(tmp);
				}
				return store;
			}
		}

		Rule * grammarToRules(Grammar * g) {
			Rule * store = new Rule();
			for (UInt32 i = 0; i < g -> nextGrammars.count(); i++) {
				Rule * tmp = grammarToRules(g -> nextGrammars[i]);
				store -> nextRules.link(tmp);
			}
			for (UInt32 i = 0; i < g -> nextRules.count(); i++) {
				Rule * tmp = grammarToRules(g -> nextRules[i]);
				store -> nextRules.link(tmp);
			}
			return store;
		}

		void printTree(Rule * r, UInt32 i) {
			if (r == nullptr) return;
			if (r -> isTerminal()) {
				String tabs = String("    ", i);
				std::cout << tabs << r -> token -> value << std::endl;
				return;
			}
			for (UInt32 i = 0; i < r -> nextRules.count(); i++) {
				printTree(r -> nextRules[i], i + 1);
			}
		}

	public:

		Parser(StrongList<Token> * t, Grammar * g) {
			tokens = * t;
			if (g == nullptr) throw InvalidGrammarException();
			grammarRules = grammarToRules(g);
		}

		void printRuleTree() {
			printTree(grammarRules, 0);
		}

		/*Boolean parse(AbstractSyntaxTree & ast) {
			if (tokens.isEmpty()) return ast;

			Token currentToken = tokens[0];

			stack.push(grammar -> name);

			for (UInt32 i = 0; i < grammar -> nextRules.count(); i++) {

				Rule currentRule = grammar -> nextRules[i];
				while (currentRule.matches(currentToken)) {
					if (currentRule.isTerminal()) {
						// Stop
						break;
					}
					currentRule
				}


			}

		}*/

	};

}

#endif
