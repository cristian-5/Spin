
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
					Token * token = getNext(tokens, i);
					if (token != nullptr) {
						i++;
						token = getNext(tokens, i);
						UInt32 idCount = 0;
						while (token -> type == identifier) {
							idCount++;
							i++;
							token = getNext(tokens, i);
							if (token -> type == dot) {
								token -> value = "::";
								i++;
								token = getNext(tokens, i);
							} else if (token -> type == semicolon) {
								break;
							} else {
								throw SyntaxErrorException(
									token -> value, ";",
									FilePosition(
										getPosition(currentFile, token -> position)
									),
									* currentFileName
								);
							}
						}
						if (idCount == 0) {
							throw SyntaxErrorException(
								token -> value, "identifier",
								FilePosition(
									getPosition(currentFile, token -> position)
								),
								* currentFileName
							);
						}
						if (token -> type == semicolon) {
							if (idCount > 1) t.value = "using";
							else t.value = "using namespace";
						} else {
							throw SyntaxErrorException(
								token -> value, ";",
								FilePosition(
									getPosition(currentFile, token -> position)
								),
								* currentFileName
							);
						}
					} else {
						throw SyntaxErrorException(
							"end of file", ";",
							FilePosition(
								getPosition(currentFile, t.position)
							),
							* currentFileName
						);
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
