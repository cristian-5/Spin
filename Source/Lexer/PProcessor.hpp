
/*!
 *
 *    + --------------------------------------- +
 *    |  PProcessor.hpp                         |
 *    |                                         |
 *    |               PreProcessor              |
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

#ifndef STACKPREPROCESSOR
#define STACKPREPROCESSOR

#include "../Aliases/Aliases.hpp"
#include "../Collection/Collection.hpp"

#include "ASTree.hpp"
#include "Exceptions.hpp"

using namespace Collection;

namespace Stack {

	class PreProcessor {

		private:

		StrongList<Token> * tokens = nullptr;

		String * inputFile = nullptr;

		String fileName = "";

		void processParentesis() {
			HeapStack<Token> stack = HeapStack<Token>();
			for (UInt32 i = 0; i < tokens -> count(); i++) {
				Token t = tokens -> getNode(i);
				if (t.type == openRoundBracket) {
					t.type = closeRoundBracket;
					t.lexeme = ")";
					stack.push(t);
				} else if (t.type == openSquareBracket) {
					t.type = closeSquareBracket;
					t.lexeme = "]";
					stack.push(t);
				} else if (t.type == openCurlyBracket) {
					t.type = closeCurlyBracket;
					t.lexeme = "}";
					stack.push(t);
				} else if (t.type == closeRoundBracket) {
					Token Andrea = stack.pop();
					if (Andrea.type != closeRoundBracket) {
						throw SyntaxErrorException(
							")", Andrea.lexeme,
							getPosition(inputFile, t.position),
							fileName
						);
					}
				} else if (t.type == closeSquareBracket) {
					Token Andrea = stack.pop();
					if (Andrea.type != closeSquareBracket) {
						throw SyntaxErrorException(
							")", Andrea.lexeme,
							getPosition(inputFile, t.position),
							fileName
						);
					}
				} else if (t.type == closeCurlyBracket) {
					Token Andrea = stack.pop();
					if (Andrea.type != closeCurlyBracket) {
						throw SyntaxErrorException(
							")", Andrea.lexeme,
							getPosition(inputFile, t.position),
							fileName
						);
					}
				}
			}
		}

		void processBras() {
			
		}
		void processKets() {
			
		}
		void processBraKets() {
			
		}

		void processVectors() {
			
		}

		void processCombinedOperators() {
			
		}

		public:

		PreProcessor() { }

		StrongList<Token> * process(StrongList<Token> * t,
								    String * i = nullptr,
									String f = "Unknown File") {
			tokens = t; inputFile = i; fileName = f;
			processVectors();
			processCombinedOperators();
		}


	};

}

#endif
