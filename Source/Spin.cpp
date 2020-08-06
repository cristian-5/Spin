
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

#include "Common/Interface.hpp"

#include "Manager/Manager.hpp"
#include "Preprocessor/Wings.hpp"
#include "Compiler/Compiler.hpp"
#include "Virtual/Processor.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	argc -= 1; argv += 1;

	String main;

	switch (argc) {
		case 0: {
			OStream << endLine << "% Spin Catastrophic Event %"
				<< endLine << "You forgot to specify the source file!"
				<< endLine << "Type spin -h and I'll guide you through."
				<< endLine << endLine;
			return ExitCodes::failure;
		} break;
		case 1: {
			String argument = String(argv[0]);
			if (argument.length() == 0) {
				OStream << endLine << "% Spin Catastrophic Event %"
					<< endLine << "You forgot to specify the source file!"
					<< endLine << "Type spin -h and I'll guide you through."
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
						<< endLine << "  Usage: spin <file>"
						<< endLine << "         <file>: should be the main file and"
						<< endLine << "                 it should end with '.spin'."
						<< endLine << "  I told you it was helpful."
						<< endLine << endLine;
				return ExitCodes::success;
			} else main = argument;
		} break;
		default: {
			OStream << endLine << "% Spin Catastrophic Event (I'm not your maid!) %"
				<< endLine << "You gave me too many arguments. Go parse them yourself!"
				<< endLine << "Type spin -h and I'll guide you through."
				<< endLine << endLine;
			return ExitCodes::failure;
		} break;
	}

	Compiler * compiler = Compiler::self();
	Processor * processor = Processor::self();

	SourceCode * code = nullptr;
	Program * program = nullptr;

	try {
		code = Wings::spread(main);
		program = compiler -> compile(code);
		processor -> run(program);
	} catch (Program::Error & e) {
		OStream << endLine << endLine << "% " << e.getErrorCode()
				<< " Error on line " << e.getLine() << " of ['"
				<< e.getFile() << "'] %" << endLine
				<< e.getMessage() << endLine << endLine;
		if (code) delete code;
		if (program) delete program;
		return ExitCodes::failure;
	} catch (Processor::Crash & c) {
		OStream << endLine << "% EVL Error on address [0x"
				<< hexadecimal << padding(8) << c.getAddress()
				<< "] %" << endLine << "Crash bytecode: "
				<< hexadecimal << padding(2)
				<< c.getInstruction().code << " "
				<< hexadecimal << padding(16)
				<< hexadecimal << c.getInstruction().as.index
				<< endLine << endLine;
		if (code) delete code;
		if (program) delete program;
		return ExitCodes::failure;
	} catch (Manager::BadFileException & b) {
		OStream << endLine << endLine <<  "% PPR Catastrophic Event %"
				<< endLine << "Couldn't open file ['"
				<< b.getPath() << "']!" << endLine << endLine;
		if (code) delete code;
		if (program) delete program;
		return ExitCodes::failure;
	}

	delete code; delete program;
	
	return ExitCodes::success;
}
