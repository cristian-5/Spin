
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

#include "Aliases/Aliases.hpp"
#include "Collection/Collection.hpp"
#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"

using namespace std;

using namespace Collection;
using namespace Stack;

Int32 main(Int32 argc, Character * argv[]) {

	String test = "";

	cout << "Insert test String: ";
	cin >> test;

	Lexer lexer = Lexer();

	StrongList<Token> * tokens = nullptr;

	try {
		tokens = lexer.tokenize(& test);
	} catch (InvalidTokenException & e) {
		FilePosition f = e.getPosition();
		cout << "Error in Main.stk [row: " << f.row;
		cout << ", col: " << f.col << "];" << endl;
		cout << "Unrecognized Token!" << endl;
		cin.get();
		return exitFailure;
	}

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
		Parser p = Parser(tokens, bfR);
		p.parse();
		cout << "Parsed Successfully!" << endl;
	} catch (Exception & e) {
		cout << "Syntax Error!" << endl;
		cin.get();
		return exitFailure;
	}

	cin.get();

	/* TODO: Delete Pointers! */

	return exitSuccess;
}
