
/*!
 *
 *    + --------------------------------------- +
 *    |  Transpiler.hpp                         |
 *    |                                         |
 *    |                Transpiler               |
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

#ifndef STACKTRANSPILER
#define STACKTRANSPILER

#include <iostream>
#include <string>

#include "../Aliases/Aliases.hpp"

#include "../Linker/FileHandler.hpp"
#include "../Lexer/Lexer.hpp"

namespace Stack {

	class Transpiler {

		private:

		String tokensToFile(StrongList<Token> * tokens) {
			if (tokens == nullptr) return "";
			if (tokens -> isEmpty()) return "";
			String file = "";
			for (UInt32 i = 0; i < tokens -> count(); i++) {
				file += tokens -> getNode(i).value + " ";
			}
			return file;
		}

		void removeBeginEnd(StrongList<Token> * tokens) {
			if (tokens -> isEmpty()) return;
			if (tokens -> getNode(0).type == beginFile) {
				tokens -> unlinkFirst();
			}
			if (tokens -> isEmpty()) return;
			if (tokens -> getNode(tokens -> count() - 1).type == endFile) {
				tokens -> unlinkLast();
			}
		}

		void processTokens(StrongList<Token> * tokens) {
			removeBeginEnd(tokens);
		}

		public:

		Transpiler() { }

		void processFile(String file) {
			StrongList<String> files = StrongList<String>();
			files.link(file);
			processFiles(files);
		}

		void processFiles(StrongList<String> files) {
			Lexer lexer = Lexer();
			for (UInt32 i = 0; i < files.count(); i++) {
				String file = "";
				try {
					file = FileHandler::stringFromFile(files[i]);
					StrongList<Token> * tokens = nullptr;
					tokens = lexer.tokenize(& file);
					processTokens(tokens);
					file = tokensToFile(tokens);
					FileHandler::createNewFile(files[i] + ".cpp", file);
				} catch (Exception & e) { throw; }
			}
		}

	};

}

#endif
