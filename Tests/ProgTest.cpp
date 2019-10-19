
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
			for (SyntaxError error : * errors) {
				UInt64 line = error.getLine();
				Output << "[line " << line << "]: "
					   << error.getMessage() << endLine;
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
			   << e.getFileName() << "' [line "
			   << e.getLine() << "]:" << endLine
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
