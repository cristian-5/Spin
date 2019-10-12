
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

#include "../Source/Aliases/Includes.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	Output << "Insert test string: ";
	String * test = new String(getInput());
	Output << endLine;

	Lexer * lexer = Lexer::self();

	CodeUnit * unit = new CodeUnit(
		nullptr,
		new String("Virtual File"),
		test
	);
	
	unit -> tokens = lexer -> tokenise(unit -> contents);

	Parser * parser = Parser::self();

	SyntaxTree * syntaxTree = nullptr;

	try {
		syntaxTree = parser -> parse(unit);
	} catch (ParserErrorException & p) {
		auto units = p.getUnitErrors();
		for (UnitError * unit : * units) {
			auto errors = unit -> getErrors();
			Output << "Found " << errors -> size()
				   << " errors in '" << unit -> getName()
				   << "':" << endLine;
			UInt32 i = 1;
			for (SyntaxError error : * errors) {
				FilePosition f = error.getPosition();
				Output << padding << i << " [" << f.row
					   << ":" << f.col << "]: "
					   << error.getMessage() << endLine;
				i += 1;
			}
			Output << endLine;
		}
		Output << "Press enter to exit. ";
		waitKeyPress();
		return exitFailure;
	}

	if (!syntaxTree) {
		Output << "File Scope Failure!" << endLine;
		Output << "Press enter to exit. ";
		waitKeyPress();
		return exitFailure;
	}

	Interpreter * interpreter = Interpreter::self();

	try {
		interpreter -> evaluate(syntaxTree);
	} catch (InterpreterErrorException & e) {
		Output << "Found evaluation error in file '"
			   << e.getFileName() << "' at position "
			   << "[" << e.getPosition().row << ":"
			   << e.getPosition().col << "]:" << endLine
			   << e.getMessage() << endLine;
		Output << endLine << "Press enter to exit. ";
		waitKeyPress();
		delete syntaxTree;
		return exitFailure;
	}

	delete unit;
	delete syntaxTree;

	Output << endLine << "Press enter to exit. ";
	waitKeyPress();
	
	return exitSuccess;
}
