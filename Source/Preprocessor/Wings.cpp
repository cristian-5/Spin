
/*!
 *
 *    + --------------------------------------- +
 *    |  Wings.cpp                              |
 *    |                                         |
 *    |            Wings Preprocessor           |
 *    |                                         |
 *    |  Created by Cristian A.                 |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
!*/

#include "Wings.hpp"

#include "../Common/Interface.hpp"

#ifndef SPIN_WINGS
#define SPIN_WINGS

#include "../Manager/Manager.hpp"
#include "../Lexer/Lexer.hpp"
#include "../Utility/Converter.hpp"
#include "../Compiler/Libraries.hpp"

namespace Spin {

	void Wings::replace(CodeUnit * code, Token::Type type, String lexeme, Token::Type newType) {
		if (!code || !(code -> tokens)) return;
		for (Token & t : * code -> tokens) {
			if (t.type == type && t.lexeme == lexeme) t.type = newType;
		}
	}
	Boolean Wings::isKnownLibrary(String l) {
		return Library::isKnown(l);
	}
	String Wings::complete(CodeUnit * code, SizeType & i) {
		Array<Token> * wing = code -> tokens;
		wing -> at(i).type = Token::Type::empty;
		i += 1;
		String import;
		while (i < wing -> size()) {
			if (wing -> at(i).type == Token::Type::symbol ||
				wing -> at(i).type == Token::Type::customType) {
				import += wing -> at(i).lexeme;
				wing -> at(i).type = Token::Type::empty;
			} else {
				throw Program::Error(
					code,
					"Expected 'identifier' but found '" + wing -> at(i).lexeme + "'!",
					wing -> at(i),
					ErrorCode::ppr
				);
			}
			i += 1;
			if (i < wing -> size()) {
				switch (wing -> at(i).type) {
					case Token::Type::semicolon: {
						wing -> at(i).type = Token::Type::empty;
						return import;
					}
					case Token::Type::doublecolon: {
						wing -> at(i).type = Token::Type::empty;
						import.push_back('/');
						i += 1;
						break;
					}
					case Token::Type::endFile: {
						i -= 1;
						throw Program::Error(
							code,
							"File ended unexpectedly when looking for ';' ending import directive!",
							wing -> at(i),
							ErrorCode::ppr
						);
					} break;
					default: throw Program::Error(
						code,
						"Expected ';' ending import directive but found '" + wing -> at(i).lexeme + "'!",
						wing -> at(i),
						ErrorCode::ppr
					);
				}
			} else break;
		}
		i -= 1;
		throw Program::Error(
			code,
			"File ended unexpectedly when looking for ';' ending import directive!",
			wing -> at(i),
			ErrorCode::ppr
		);
	}
	String Wings::parentFolder(String f) {
		if (f.empty() || f.length() <= 1) return String();
		SizeType i = f.length() - 1;
		while (f[i] != '/') {
			// If no slashes found:
			if (i - 1 < 0) return String();
			i -= 1;
		}
		String path = f.substr(0, i + 1);
		return path;
	}
	Array<String> Wings::classify(CodeUnit * code, Array<String> * libs) {
		if (!code || !libs) return Array<String>();
		Array<Token> * file = code -> tokens;
		if (!file || file -> empty()) return Array<String>();
		Array<String> imports;
		Array<String> knownLibraries;
		const SizeType tokenCount = file -> size();
		for (SizeType i = 0; i < tokenCount; i += 1) {
			if (file -> at(i).type == Token::Type::importKeyword) {
				try {
					const SizeType store = i;
					String import = complete(code, i);
					if (isKnownLibrary(import)) {
						for (String & i : knownLibraries) {
							if (i == import) {
								// Redefinition of known library:
								throw Program::Error(
									code,
									"Found repetitive import directive!",
									file -> at(store),
									ErrorCode::ppr
								);
							}
						}
						knownLibraries.push_back(import);
						Boolean listed = false;
						for (String & i : * libs) {
							if (i == import) {
								listed = true;
								break;
							}
						}
						if (!listed) libs -> push_back(import);
						// Replace that symbol for usage.
						replace(code, Token::Type::symbol, import, Token::Type::customType);
					} else {
						String fileName = import + ".spin";
						for (String & s : imports) {
							if (s == fileName) {
								throw Program::Error(
									code,
									"Found repetitive import directive!",
									file -> at(store),
									ErrorCode::ppr
								);
							}
						}
						imports.push_back(fileName);
						// Replace that symbol for usage.
						String name;
						for (SizeType i = import.length() - 1; i <= 0; i -= 1) {
							if (import[i] == '/') break;
							name = import[i] + name;
						}
						replace(code, Token::Type::symbol, name, Token::Type::customType);
					}
				} catch (Program::Error & e) { throw; }
			} else if (file -> at(i).type != Token::Type::beginFile) break;
		}
		imports.shrink_to_fit();
		return imports;
	}
	Boolean Wings::isSpread(String wing, Array<CodeUnit *> * wings) {
		if (!wings) return false;
		for (CodeUnit * code : * wings) {
			if (!code) continue;
			if ((* code -> name) == wing) return true;
		}
		return false;
	}
	void Wings::spreadWing(CodeUnit * code, Array<CodeUnit *> * resolved, Array<String> * libs) {
		if (! code || !libs) return;
		Array<String> imports;
		try { imports = classify(code, libs); }
		catch (Program::Error & e) { throw; }
		Lexer * lexer = Lexer::self();
		for (String & wing : imports) {
			wing = parentFolder(*(code -> name)) + wing;
			// We need to check if wing has not been
			// already spread in resolved:
			if (isSpread(wing, resolved)) continue;
			String * unitFile = nullptr;
			try {
				unitFile = Manager::stringFromFile(wing);
			} catch (Manager::BadFileException & b) { throw; }
			try {
				spreadWing(
					new CodeUnit(
						lexer -> tokenise(unitFile),
						new String(wing),
						unitFile
					),
					resolved, libs
				);
			} catch (Program::Error & e) { throw; }
		}
		prototype(code);   // Run type classification.
		prepareWing(code); // Delete empty tokens.
		// Avoid empty code units:
		if (code -> tokens -> size() <= 2) return;
		// After resolution add code to 'resolved':
		resolved -> push_back(code);
	}
	Token::Type Wings::token(CodeUnit * code, SizeType i) {
		if (i >= code -> tokens -> size()) return Token::Type::endFile;
		return code -> tokens -> at(i).type;
	}
	void Wings::prototype(CodeUnit * code) {
		if (!code || !(code -> tokens)) return;
		// Class prototyping:
		SizeType tokenCount = (code -> tokens -> size()) - 1;
		for (SizeType i = 0; i < tokenCount; i += 1) {
			if (code -> tokens -> at(i).isTypeType()) {
				i += 1;
				Token token = code -> tokens -> at(i);
				if (token.type == Token::Type::symbol) {
					replace(code, Token::Type::symbol, token.lexeme, Token::Type::customType);
				}
			}
		}
		// Routine prototyping:
		tokenCount = (code -> tokens -> size()) - 1;
		for (SizeType i = 0; i < tokenCount; i += 1) {
			if (code -> tokens -> at(i).isRoutineKeyword()) {
				Boolean proc = (token(code, i) == Token::Type::procKeyword);
				i += 1;
				Prototype prototype;
				if (token(code, i) != Token::Type::symbol) { i -= 1; continue; }
				prototype.name = code -> tokens -> at(i).lexeme; i += 1;
				if (token(code, i) != Token::Type::openParenthesis) { i -= 1; continue; }
				i += 1;
				Boolean broken = false;
				if (token(code, i) != Token::Type::closeParenthesis) {
					i -= 1;
					do {
						i += 1;
						Parameter parameter;
						if (token(code, i) != Token::Type::symbol) {
							broken = true;
							break;
						}
						parameter.name = code -> tokens -> at(i).lexeme;
						i += 1;
						if (token(code, i) != Token::Type::colon) {
							broken = true;
							break;
						}
						i += 1;
						if (token(code, i) != Token::Type::basicType) {
							broken = true;
							break;
						}
						parameter.type = Converter::stringToType(
							code -> tokens -> at(i).lexeme
						);
						i += 1;
						prototype.parameters.push_back(parameter);
					} while (token(code, i) == Token::Type::comma);
					if (broken) { i -= 1; continue; }
				}
				if (token(code, i) != Token::Type::closeParenthesis) { i -= 1; continue; }
				i += 1;
				if (proc) {
					if (token(code, i) != Token::Type::openBrace) { i -= 1; continue; }
					code -> prototypes.push_back(prototype);
					i -= 1; continue;
				}
				if (token(code, i) != Token::Type::arrow) { i -= 1; continue; }
				i += 1;
				if (token(code, i) != Token::Type::basicType) { i -= 1; continue; }
				prototype.returnType = Converter::stringToType(
					code -> tokens -> at(i).lexeme
				);
				i += 1;
				if (token(code, i) != Token::Type::openBrace) { i -= 1; continue; }
				code -> prototypes.push_back(prototype);
			}
		}

	}
	void Wings::prepareWing(CodeUnit * code) {
		if (!code || !(code -> tokens)) return;
		Array<Token> * newTokens = new Array<Token>();
		for (Token token : * code -> tokens) {
			if (token.type != Token::Type::empty) {
				newTokens -> push_back(token);
			}
		}
		delete code -> tokens;
		newTokens -> shrink_to_fit();
		code -> tokens = newTokens;
	}
	SourceCode * Wings::spread(String path) {

		// Getting the main file:
		String * file = nullptr;
		try {
			file = Manager::stringFromFile(path);
		} catch (Manager::BadFileException & b) { throw; }

		Lexer * lexer = Lexer::self();

		// Lexing the main file:
		CodeUnit * main = new CodeUnit(
			lexer -> tokenise(file),
			new String(path),
			file
		);

		// Classify import statements:

		Array<String> * libs = new Array<String>();

		Array<String> imports;
		try {
			imports = classify(main, libs);
		} catch (Program::Error & e) {
			if (libs) delete libs;
			throw;
		}

		Array<CodeUnit *> * wings = new Array<CodeUnit *>();
		for (String & wing: imports) {
			wing = parentFolder(path) + wing;
			String * unitFile = nullptr;
			try {
				unitFile = Manager::stringFromFile(wing);
			} catch (Manager::BadFileException & b) { throw; }
			wings -> push_back(
				new CodeUnit(
					lexer -> tokenise(unitFile),
					new String(wing),
					unitFile
				)
			);
		}

		// Main preparation:

		prototype(main);   // Run type classification.
		prepareWing(main); // Delete empty tokens.

		// Wings classification:

		Array<CodeUnit *> * resolved = new Array<CodeUnit *>();

		try {
			for (CodeUnit * unit : * wings) {
				spreadWing(unit, resolved, libs);
			}
		} catch (Exception & e) { delete wings; throw; }

		// Delete wings but not their content:
		delete wings;

		return new SourceCode(main, resolved, libs);

	}

}

#endif
