
/*!
 *
 *    + --------------------------------------- +
 *    |  Test.cpp                               |
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
#include "../Source/Aliases/Prototypes/Parser.hpp"
#include "../Source/Aliases/Prototypes/Interpreter.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	OStream << "Insert test string: ";
	String * test = new String(getInput());
	OStream << endLine;

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
			OStream << "Found " << errors -> size()
				   << " errors in '" << unit -> getName()
				   << "':" << endLine;
			for (SyntaxError error : * errors) {
				UInt64 line = error.getLine();
				OStream << "[line " << line << "]: "
					   << error.getMessage() << endLine;
			}
			OStream << endLine;
		}
		return ExitCodes::failure;
	}

	if (!syntaxTree) {
		OStream << "File Scope Failure!" << endLine << endLine;
		return ExitCodes::failure;
	}

	Interpreter * interpreter = Interpreter::self();

	try {
		interpreter -> evaluate(syntaxTree);
	} catch (InterpreterErrorException & e) {
		OStream << "Found evaluation error in file '"
			   << e.getFileName() << "' [line "
			   << e.getLine() << "]:" << endLine
			   << e.getMessage() << endLine << endLine;
		delete syntaxTree;
		return ExitCodes::failure;
	}

	OStream << endLine;

	delete unit;
	delete syntaxTree;
	
	return ExitCodes::success;
}
