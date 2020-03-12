
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

#include "../Source/Aliases/Prototypes/Manager.hpp"
#include "../Source/Aliases/Prototypes/Wings.hpp"
#include "../Source/Aliases/Prototypes/SyntaxTree.hpp"
#include "../Source/Aliases/Prototypes/Parser.hpp"
#include "../Source/Aliases/Prototypes/Interpreter.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	OStream << "Insert test path: ";
	String test = getInput();
	OStream << endLine;

	Parser * parser = Parser::self();
	Interpreter * interpreter = Interpreter::self();

	Program * program = nullptr;
	SyntaxTree * syntaxTree = nullptr;

	try {
		program = Wings::spread(test);
		syntaxTree = parser -> parse(program);
		interpreter -> evaluate(syntaxTree);
	} catch (Program::Error & e) {
		OStream << "% " << e.getErrorCode()
				<< " Error on line " << e.getLine() << " of ['"
				<< e.getFile() << "'] %" << endLine
				<< e.getMessage() << endLine << endLine;
		if (program) delete program;
		if (syntaxTree) delete syntaxTree;
		return ExitCodes::failure;
	} catch (Manager::BadFileException & b) {
		OStream << "% PPR Catastrophic Event %"
				<< endLine << "Couldn't open file ['"
				<< b.getPath() << "']!" << endLine << endLine;
		if (program) delete program;
		if (syntaxTree) delete syntaxTree;
		return ExitCodes::failure;
	}

	delete program;
	delete syntaxTree;

	OStream << endLine;
	
	return ExitCodes::success;
}
