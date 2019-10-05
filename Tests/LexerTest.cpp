
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

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	Output << "Insert test string: ";
	String test = getInput();
	Output << endLine;

	Lexer * lexer = Lexer::self();
	
	Array<Token> * tokens = lexer -> tokenise(& test);

	Output << "Tokens: " << endLine;
	UInt32 i = 1;
	for (Token & token : * tokens) {
		Output << padding << i << " | Type: ";
		Output << padding << token.type;
		Output << " | Token: " << token.lexeme << endLine;
		i += 1;
	}

	delete tokens;

	Output << endLine << "Press enter to exit. ";
	waitKeyPress();
	
	return exitSuccess;
}
