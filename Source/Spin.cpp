
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

#include "../Source/Aliases/Prototypes/Manager.hpp"
#include "../Source/Aliases/Prototypes/Wings.hpp"
#include "../Source/Aliases/Prototypes/SyntaxTree.hpp"
#include "../Source/Aliases/Prototypes/Parser.hpp"
#include "../Source/Aliases/Prototypes/Interpreter.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	argc -= 1; argv += 1;

	String main;

	switch (argc) {
		case 0: {
			OStream << endLine << "% Spin Catastrophic Event %"
				<< endLine << "You forgot to specify the source file!"
				<< endLine << "Type spin -h and we'll guide you through."
				<< endLine << endLine;
			return ExitCodes::failure;
		} break;
		case 1: {
			String argument = String(argv[0]);
			if (argument.length() == 0) {
				OStream << endLine << "% Spin Catastrophic Event %"
					<< endLine << "You forgot to specify the source file!"
					<< endLine << "Type spin -h and we'll guide you through."
					<< endLine << endLine;
				return ExitCodes::failure;
			}
			if (argument == "-v" || argument == "--version") {
				OStream << endLine << "% Spin Version 3.0.0 %" << endLine
						<< endLine << "Andrea, please don't break this."
						<< endLine << "Seriously, if u break it u fix it!"
						<< endLine << endLine;
				return ExitCodes::success;
			} else if (argument == "-h" || argument == "--help") {
				OStream << endLine << "% Spin help magician (it's helpful) %"
						<< endLine << "  Usage: spin <File>"
						<< endLine << "         <File>: should be the main file and"
						<< endLine << "                 it should end with '.spin'."
						<< endLine << "  I told you it was helpful."
						<< endLine << endLine;
				return ExitCodes::success;
			} else main = argument;
		} break;
		default: {
			OStream << endLine << "% Spin Catastrophic Event (I'm not your maid!) %"
				<< endLine << "You gave me too many arguments. Go parse them yourself!"
				<< endLine << "Type spin -h and we'll guide you through."
				<< endLine << endLine;
			return ExitCodes::failure;
		} break;
	}

	Parser * parser = Parser::self();
	Interpreter * interpreter = Interpreter::self();

	Program * program = nullptr;
	SyntaxTree * syntaxTree = nullptr;

	try {
		program = Wings::spread(main);
		syntaxTree = parser -> parse(program);
		interpreter -> evaluate(syntaxTree);
	} catch (Program::Error & e) {
		OStream << endLine << endLine << "% " << e.getErrorCode()
				<< " Error on line " << e.getLine() << " of ['"
				<< e.getFile() << "'] %" << endLine
				<< e.getMessage() << endLine << endLine;
		if (program) delete program;
		if (syntaxTree) delete syntaxTree;
		return ExitCodes::failure;
	} catch (Manager::BadFileException & b) {
		OStream << endLine << endLine <<  "% PPR Catastrophic Event %"
				<< endLine << "Couldn't open file ['"
				<< b.getPath() << "']!" << endLine << endLine;
		if (program) delete program;
		if (syntaxTree) delete syntaxTree;
		return ExitCodes::failure;
	}

	delete program;
	delete syntaxTree;
	
	return ExitCodes::success;
}
