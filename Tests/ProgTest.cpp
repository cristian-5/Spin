
/*!
 *
 *    + --------------------------------------- +
 *    |  ProgTest.cpp                           |
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
#include "../Source/Lexer/Lexer.hpp"
#include "../Source/Parser/Parser.hpp"
#include "../Source/Interpreter/Interpreter.hpp"

using namespace std;

using namespace Stack;

Int32 main(Int32 argc, Character * argv[]) {

	cout << "Insert test string: ";
	String test = getInput();
	cout << endl;

	Lexer * lexer = Lexer::self();

	ArrayList<Token> * tokens = lexer -> tokenise(& test, "Virtual File");

	Parser * parser = Parser::self();
	ArrayList<Statement *> * st = nullptr;

	try {
		st = parser -> parse(tokens, & test, "Virtual File");
	} catch (ParserErrorException & p) {
		const ArrayList<SyntaxError> * const e = p.getErrors();
		cout << "Found " << e -> size() << " errors in '"
			 << p.getFileName() << "'!" << endl;
		UInt32 i = 1;
		for (SyntaxError s : * e) {
			FilePosition f = s.getPosition();
			cout << padding << i << " [" << f.row
				 << ":" << f.col << "]: "
				 << s.getMessage() << endl;
			i += 1;
		}
		cout << endl << "Press enter to exit. ";
		waitKeyPress();
		delete tokens;
		return exitFailure;
	}

	delete tokens;

	if (st == nullptr) {
		cout << "Syntax Tree Failure!" << endl;
		cout << "Press enter to exit. ";
		waitKeyPress();
		return exitFailure;
	}

	// Interpreter Test:

	Interpreter * interpreter = Interpreter::self();

	try {
		interpreter -> evaluate(st, & test, "Virtual File");
	} catch (InterpreterErrorException & e) {
		cout << "[" << e.getPosition().row << ":";
		cout << e.getPosition().col << "]: " << e.getMessage() << endl;
		cout << "Press enter to exit. ";
		waitKeyPress();
		delete st;
		return exitFailure;
	}

	delete st;

	cout << endl << "Press enter to exit. ";
	waitKeyPress();
	
	return exitSuccess;
}
