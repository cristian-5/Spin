
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
!*/

#include "Aliases/Input.hpp"

#include "Aliases/Prototypes/Manager.hpp"
#include "Aliases/Prototypes/Lexer.hpp"
#include "Aliases/Prototypes/Parser.hpp"
#include "Aliases/Prototypes/Interpreter.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	if (argc == 2 && String(argv[1]) == "-v") {
		OStream << endLine << "Spin Language Version: 3.0 beta." << endLine;
		return ExitCodes::success;
	}

	Array<CodeUnit *> * units = new Array<CodeUnit *>();
	for (UInt64 i = 1; i < argc; i += 1) {
		String * file = nullptr;
		try {
			file = Manager::stringFromFile(argv[i]);
			if (!file || (file -> length() == 0)) continue;
			units -> push_back(
				new CodeUnit(
					nullptr,
					new String(argv[i]),
					file
				)
			);
		} catch (Manager::BadFileException & exc) {
			for (CodeUnit * u : * units) delete u; delete units;
			OStream << "Bad Access! Invalid input file at path '"
				   << exc.getPath() << "'!" << endLine;
			return ExitCodes::failure;
		}
	}

	if (units -> size() == 0) return ExitCodes::success;

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
			OStream << "Found " << errors -> size()
				   << " errors in '" << unit -> getName()
				   << "':" << endLine;
			for (SyntaxError error : * errors) {
				UInt64 line = error.getLine();
				OStream << " [line " << line << "]: "
					   << error.getMessage() << endLine;
			}
			OStream << endLine;
		}
		return ExitCodes::failure;
	}

	if (!syntaxTree) {
		OStream << "File Scope Failure!" << endLine;
		return ExitCodes::failure;
	}

	Interpreter * interpreter = Interpreter::self();

	try {
		interpreter -> evaluate(syntaxTree);
	} catch (InterpreterErrorException & e) {
		OStream << "Found evaluation error in file '"
			   << e.getFileName() << "' [line "
			   << e.getLine() << "]:" << endLine
			   << e.getMessage() << endLine;
		delete syntaxTree;
		return ExitCodes::failure;
	}

	for (CodeUnit * unit : * units) delete unit;

	delete syntaxTree;

	return ExitCodes::success;
}
