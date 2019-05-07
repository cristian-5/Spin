
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

	String s = "for (Int32 i = 0; i < x; i++)";
	Lexer lexer = Lexer(s);

	StrongList<Token> tokens = StrongList<Token>();

	try {
		tokens = lexer.tokenize();
	} catch (InvalidTokenException & e) {
		cout << "Error on character: " << e.getPosition() << endl;
		cout << "Unrecognized expression!" << endl;
		int x = 0;
		cin >> x;
		return EXIT_FAILURE;
	}

	/*for (UInt32 i = 0; i < tokens.count(); i++) {
		cout << tokens[i].type << ":    " << tokens[i].value << endl;
	}*/

	Grammar * ifStatement = new Grammar();

	Token * ifToken = new Token("if", ifKeyword);
	Token * forToken = new Token("for", forKeyword);
	Token * elseToken = new Token("else", elseKeyword);
	Token * breakToken = new Token("break", breakKeyword);
	Token * semi = new Token(";", semicolon);

	Rule * sR = new Rule(semi);









	cin.get();

	return EXIT_SUCCESS;
}
