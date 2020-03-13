
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

#include "../Aliases/Prototypes/Wings.hpp"

#ifndef SPIN_WINGS
#define SPIN_WINGS

#include "../Aliases/Prototypes/Manager.hpp"
#include "../Aliases/Prototypes/Lexer.hpp"
#include "../Aliases/Prototypes/Libraries.hpp"
#include "../Aliases/Prototypes/Chaos.hpp"

namespace Spin {

	void Wings::replace(CodeUnit * code, TokenType type, String lexeme, TokenType newType) {
		if (!code || !(code -> tokens)) return;
		for (Token & t : * code -> tokens) {
			if (t.type == type && t.lexeme == lexeme) t.type = newType;
		}
	}

	Bool Wings::isKnownLibrary(String l) {
		return Library::isKnown(Chaos<Hash>::hash(l));
	}

	String Wings::complete(CodeUnit * code, SizeType & i) {
		Array<Token> * wing = code -> tokens;
		wing -> at(i).type = TokenType::empty;
		i += 1;
		String import;
		while (i < wing -> size()) {
			if (wing -> at(i).type == TokenType::symbol) {
				import += wing -> at(i).lexeme;
				wing -> at(i).type = TokenType::empty;
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
					case TokenType::semicolon: {
						wing -> at(i).type = TokenType::empty;
						return import;
					}
					case TokenType::doublecolon: {
						wing -> at(i).type = TokenType::empty;
						import.push_back('/');
						i += 1;
						break;
					}
					case TokenType::endFile: {
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

	Array<String> Wings::classify(CodeUnit * code, Array<Hash> * libs) {
		if (!code || !libs) return Array<String>();
		Array<Token> * file = code -> tokens;
		if (!file || file -> empty()) return Array<String>();
		Array<String> imports;
		const SizeType tokenCount = file -> size();
		for (SizeType i = 0; i < tokenCount; i += 1) {
			if (file -> at(i).type == TokenType::importKeyword) {
				try {
					const SizeType store = i;
					String import = complete(code, i);
					if (isKnownLibrary(import)) {
						Hash hash = Chaos<Hash>::hash(import);
						for (Hash & i : * libs) {
							if (i == hash) continue;
						}
						libs -> push_back(hash);
						// Replace that symbol for usage.
						replace(code, TokenType::symbol, import, TokenType::customType);
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
						replace(code, TokenType::symbol, name, TokenType::customType);
					}
				} catch (Program::Error & e) { throw; }
			}
		}
		imports.shrink_to_fit();
		return imports;
	}

	Bool Wings::isSpread(String wing, Array<CodeUnit *> * wings) {
		// TO FIX: use full path instead of relative path!
		if (!wings) return false;
		for (CodeUnit * code : * wings) {
			if (!code) continue;
			if ((* code -> name) == wing) return true;
		}
		return false;
	}

	void Wings::spreadWing(CodeUnit * code, Array<CodeUnit *> * resolved, Array<Hash> * libs) {
		if (! code || !libs) return;
		Array<String> imports;
		try { imports = classify(code, libs); }
		catch (Program::Error & e) { throw; }
		Lexer * lexer = Lexer::self();
		for (String & wing : imports) {
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
						new String(wing), // TOFIX: Add current folder prefix
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

	void Wings::prototype(CodeUnit * code) {
		if (!code || !(code -> tokens)) return;
		SizeType tokenCount = (code -> tokens -> size()) - 1;
		for (SizeType i = 0; i < tokenCount; i += 1) {
			if (code -> tokens -> at(i).isTypeType()) {
				i += 1;
				Token token = code -> tokens -> at(i);
				if (token.type == TokenType::symbol) {
					replace(code, TokenType::symbol, token.lexeme, TokenType::customType);
				}
			}
		}
	}

	void Wings::prepareWing(CodeUnit * code) {
		if (!code || !(code -> tokens)) return;
		Array<Token> * newTokens = new Array<Token>();
		for (Token token : * code -> tokens) {
			if (token.type != TokenType::empty) {
				newTokens -> push_back(token);
			}
		}
		delete code -> tokens;
		newTokens -> shrink_to_fit();
		code -> tokens = newTokens;
	}

	Program * Wings::spread(String path) {

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

		Array<Hash> * libs = new Array<Hash>();

		Array<String> imports;
		try {
			imports = classify(main, libs);
		} catch (Program::Error & e) {
			if (libs) delete libs;
			throw;
		}

		Array<CodeUnit *> * wings = new Array<CodeUnit *>();
		for (String & wing: imports) {
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

		return new Program(main, resolved, libs);

	}

}

#endif
