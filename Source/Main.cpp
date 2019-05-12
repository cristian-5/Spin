
/*!
 *
 *    + --------------------------------------- +
 *    |  main.cpp                               |
 *    |                                         |
 *    |                  Main                   |
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

#include <iostream>
#include <string>

#include "Collection/Collection.hpp"

#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"

using namespace std;

using namespace Collection;
using namespace Stack;

using UInt32 = std::uint32_t;
using String = std::string;

#define Char const char *

Int32 main(Int32 argc, Char argv[]) {

	String s = "for"; // for else for else
	Lexer lexer = Lexer(s);

	StrongList<Token> tokens = StrongList<Token>();

	try {
		tokens = lexer.tokenize();
	} catch (InvalidTokenException & e) {
		cout << "Error on character: " << e.getPosition() << endl;
		cout << "Unrecognized expression!" << endl;
		cin.get();
		return EXIT_FAILURE;
	}

	for (UInt32 i = 0; i < tokens.count(); i++) {
		cout << tokens[i].value << " ";
	}
	cout << endl;

	Token * bf = new Token("sof", beginFile);
	Token * ef = new Token("eof", endFile);

	Token * ifToken = new Token("if", ifKeyword);
	Token * for1Token = new Token("for", forKeyword);
	Token * for2Token = new Token("for", forKeyword);
	Token * elseToken = new Token("else", elseKeyword);

	SRule * bfR = new SRule(bf);
	SRule * ifR = new SRule(ifToken);
	SRule * f1R = new SRule(for1Token);
	SRule * f2R = new SRule(for2Token);
	SRule * elR = new SRule(elseToken);
	SRule * efR = new SRule(ef);

	bfR -> addNextRule(ifR);
	bfR -> addNextRule(f2R);

	// first path
	ifR -> addNextRule(f1R);
	ifR -> addNextRule(ifR);
	f1R -> addNextRule(efR);

	// second path
	f2R -> addNextRule(elR);
	elR -> addNextRule(f2R);
	elR -> addNextRule(efR);

	try {
		Parser p = Parser(& tokens, bfR);
		p.parse();
	} catch (Exception e) {
		cout << "error" << endl;
	}

	cin.get();

	return EXIT_SUCCESS;
}
