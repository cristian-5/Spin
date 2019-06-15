
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
#include "../Parser/Parser.hpp"
#include "../Syntax/Syntax.hpp"

#define TList StrongList<Token>

namespace Stack {

	/*!
	 *   @brief Syntax Error Exception.
	 *   Raised when a token is unexpected.
	 */
	class SyntaxErrorException: public Exception {
		private:
		String token = "";
		String expected = "";
		FilePosition pos = { 0, 0 };
		String fileName = "";
		public:
		String getToken() { return token; }
		String getExpected() { return expected; }
		FilePosition getPosition() { return pos; }
		String getFileName() { return fileName; }
		SyntaxErrorException(
			String t,
			String e,
			FilePosition position,
			String name
		):
		Exception(), token(t), expected(e),
		pos(position), fileName(name) { }
	};

	class Transpiler {

		private:

		String tokensToFile(TList * tokens) {
			if (tokens == nullptr) return "";
			if (tokens -> isEmpty()) return "";
			String file = "";
			for (UInt32 i = 0; i < tokens -> count(); i++) {
				file += tokens -> getNode(i).value + " ";
			}
			return file;
		}

		void removeBeginEnd(TList * tokens) {
			if (tokens -> isEmpty()) return;
			if (tokens -> getNode(0).type == beginFile) {
				tokens -> unlinkFirst();
			}
			if (tokens -> isEmpty()) return;
			if (tokens -> getNode(tokens -> count() - 1).type == endFile) {
				tokens -> unlinkLast();
			}
		}

		UInt32 tokenCount(TList * tokens, TokenType t,
						  UInt32 start, UInt32 end) {
			UInt32 count = 0;
			if (start > end) {
				UInt32 t = start;
				start = end;
				end = t;
			}
			for (UInt32 i = start; i < end; i++) {
				if (tokens -> getNode(i).type == t) {
					count++;
				}
			}
			return count;
		}

		void tokenReplace(TList * tokens, TokenType t,
						  String value, UInt32 start,
						  UInt32 end) {
			if (start > end) {
				UInt32 t = start;
				start = end;
				end = t;
			}
			for (UInt32 i = start; i < end; i++) {
				Token token = tokens -> getNode(i);
				if (token.type == t) {
					token.value = value;
					tokens -> setNode(i, token);
				}
			}
		}

		void processLibraries(TList * tokens) {
			for (UInt32 i = 0; i < tokens -> count(); i++) {
				Token t = tokens -> getNode(i);
				if (t.type == libKeyword) {
					t.value = "namespace";
					tokens -> setNode(i, t);
				}
			}
			for (UInt32 i = 0; i < tokens -> count(); i++) {
				Token t = tokens -> getNode(i);
				if (t.type == importKeyword) {
					UInt32 end = 0;
					Grammar * g = Syntax::importGrammar();
					Parser parser = Parser(tokens, g);
					if (parser.parse(i, end)) {
						// Replace based on id count:
						UInt32 count = tokenCount(
							tokens, identifier, i, end
						);
						tokenReplace(
							tokens,
							importKeyword,
							(count > 1 ?
								"using" :
								"using namespace"),
							i, end
						);
						// Skip Tokens:
						i = end;
					}
				}
			}
		}

		Token * getNext(TList * tokens, UInt32 i) {
			if (isNextAvailable(tokens, i)) {
				Token * t = new Token();
				* t =  tokens -> getNode(i + 1);
				return t;
			}
			return nullptr;
		}

		Boolean isNextAvailable(TList * tokens, UInt32 i) {
			i++; if (tokens -> isEmpty()) return false;
			if (i >= tokens -> count()) return false;
			return true;
		}

		void processTokens(TList * tokens) {
			removeBeginEnd(tokens);
			processLibraries(tokens);
			// Include all files together
			// Functions Procedures
			// Entry Point
		}

		String * currentFileName = nullptr;
		String * currentFile = nullptr;

		public:

		Transpiler() { }

		void processFile(String file) {
			StrongList<String> files = StrongList<String>();
			files.link(file);
			processFiles(files);
		}

		void processFiles(StrongList<String> paths) {
			Lexer lexer = Lexer();
			for (UInt32 i = 0; i < paths.count(); i++) {
				String path = paths[i];
				String file = "";
				try {
					file = FileHandler::stringFromFile(path);
					currentFile = & file;
					currentFileName = & path;
					TList * tokens = nullptr;
					tokens = lexer.tokenize(& file, path);
					processTokens(tokens);
					file = tokensToFile(tokens);
					FileHandler::createNewFile(path + ".cpp", file);
				} catch (Exception & e) { throw; }
			}
		}

	};

}

#endif
