
/*!
 *
 *    + --------------------------------------- +
 *    |  Spin.cpp                               |
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

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	if (argc == 2 && String(argv[1]) == "-v") {
		Output << endLine << "Spin Language Version: 3.0 beta." << endLine;
		return exitSuccess;
	}

	Array<CodeUnit *> * units = new Array<CodeUnit *>();
	for (UInt64 i = 1; i < argc; i += 1) {
		String * file = nullptr;
		try {
			file = Linker::stringFromFile(argv[i]);
			if (!file || (file -> length() == 0)) continue;
			units -> push(
				new CodeUnit(
					nullptr,
					new String(argv[i]),
					file
				)
			);
		} catch (BadFileException & exc) {
			for (CodeUnit * u : * units) delete u; delete units;
			Output << "Bad Access! Invalid input file at path '"
				   << exc.getPath() << "'!" << endLine;
			return exitFailure;
		}
	}

	if (units -> size() == 0) return exitSuccess;

	Lexer * lexer = Lexer::self();

	for (CodeUnit * unit : * units) {
		unit -> tokens = lexer -> tokenise(
			unit -> contents
		);
	}

	Parser * parser = Parser::self();

	SyntaxTree * syntaxTree = nullptr;

	try {
		syntaxTree = parser -> parse(units);
	} catch (ParserErrorException & p) {
		auto units = p.getUnitErrors();
		for (UnitError * unit : * units) {
			auto errors = unit -> getErrors();
			Output << "Found " << errors -> size()
				   << " errors in '" << unit -> getName()
				   << "':" << endLine;
			for (SyntaxError error : * errors) {
				UInt64 line = error.getLine();
				Output << " [line " << line << "]: "
					   << error.getMessage() << endLine;
			}
			Output << endLine;
		}
		return exitFailure;
	}

	if (!syntaxTree) {
		Output << "File Scope Failure!" << endLine;
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
		delete syntaxTree;
		return exitFailure;
	}

	for (CodeUnit * unit : * units) delete unit;

	delete syntaxTree;

	return exitSuccess;
}
