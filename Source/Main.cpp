
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

#include "Aliases/Includes.hpp"

using namespace std;

Int32 main(Int32 argc, Character * argv[]) {

	/* get a list of files and parse them */

	Lexer * lexer = Lexer::self();

	Array<Token> * tokens = lexer -> tokenise(& test, "Virtual File");

	Parser * parser = Parser::self();
	FileScope * fs = nullptr;

	try {
		fs = parser -> parse(tokens, & test, "Virtual File");
	} catch (ParserErrorException & p) {
		const Array<SyntaxError> * const e = p.getErrors();
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

	if (!fs) {
		cout << "File Scope Failure!" << endl;
		cout << "Press enter to exit. ";
		waitKeyPress();
		return exitFailure;
	}

	Interpreter * interpreter = Interpreter::self();

	try {
		interpreter -> evaluate(fs, & test, "Virtual File");
	} catch (InterpreterErrorException & e) {
		cout << "[" << e.getPosition().row << ":";
		cout << e.getPosition().col << "]: " << e.getMessage() << endl;
		cout << "Press enter to exit. ";
		waitKeyPress();
		delete fs;
		return exitFailure;
	}

	delete fs;

	cout << endl << "Press enter to exit. ";
	waitKeyPress();
	
	return exitSuccess;
}
