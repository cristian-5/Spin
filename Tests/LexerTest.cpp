
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
!*/

#include "../Source/Aliases/Input.hpp"

#include "../Source/Aliases/Prototypes/Lexer.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	OStream << "Insert test string: ";
	String * test = new String(getInput());
	OStream << endLine;

	Lexer * lexer = Lexer::self();
	
	Array<Token> * tokens = lexer -> tokenise(test);

	OStream << "Tokens: " << endLine << endLine;

	UInt32 i = 1;
	for (Token & token : * tokens) {
		OStream << padding(4) << i << " | Type: ";
		OStream << padding(4) << token.type;
		OStream << " | Token: " << token.lexeme << endLine;
		i += 1;
	}

	delete tokens;

	OStream << endLine << "Press enter to exit. ";
	waitKeyPress();

	return ExitCodes::success;
}
