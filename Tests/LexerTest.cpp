
/*!
 *
 *    + --------------------------------------- +
 *    |  LexerTest.cpp                          |
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

#include "../Source/Aliases/Includes.hpp"

using namespace std;

using namespace Stack;

Int32 main(Int32 argc, Character * argv[]) {

	cout << "Insert test string: ";
	String test = getInput();
	cout << endl;

	Lexer * lexer = Lexer::self();
	
	Array<Token> * tokens = lexer -> tokenise(& test, "Virtual File");

	cout << "Tokens: " << endl;
	UInt32 i = 1;
	for (Token & token : * tokens) {
		cout << padding << i << " | Type: ";
		cout << padding << token.type;
		cout << " | Token: " << token.lexeme << endl;
		i += 1;
	}

	delete tokens;

	cout << endl << "Press enter to exit. ";
	waitKeyPress();
	
	return exitSuccess;
}
