
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

#include "../Source/Aliases/Aliases.hpp"
#include "../Source/Collection/StrongList.hpp"
#include "../Source/Lexer/Lexer.hpp"

using namespace std;

using namespace Collection;
using namespace Stack;

Int32 main(Int32 argc, Character * argv[]) {

	Lexer * lexer = new Lexer();

	cout<< "Insert test string: ";
	String test = getInput();

	StrongList<Token> * tokens = nullptr;

	try {
		tokens = lexer -> tokenize(& test, "Virtual File");
	} catch (InvalidTokenException & e) {
		cout << "Error in " << e.getFileName() << "!" << endl;
		cout << "Position [row: " << e.getPosition().row << ", ";
		cout << "col: " << e.getPosition().col << "] Invalid Token!" << endl;
		cout << "Press any key to exit. ";
		waitKeyPress();
		return exitFailure;
	}

	for (UInt32 i = 0; i < tokens -> count(); i++) {
		cout << "Token " << i + 1 << ": ";
		cout << tokens -> getNode(i).lexeme << endl;
	}

	cout << "Press any key to exit. ";
	waitKeyPress();
	
	return exitSuccess;
}
