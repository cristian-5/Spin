
/*!
 *
 *    + --------------------------------------- +
 *    |  Virtual.cpp                            |
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

#include "../Source/Common/Interface.hpp"

#include "../Source/Lexer/Lexer.hpp"
#include "../Source/Compiler/Compiler.hpp"
#include "../Source/Compiler/Decompiler.hpp"
#include "../Source/Virtual/Processor.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	String * input = new String("Integer x = 6 + 7; print x; x = 10; print x; Real y = 0.8; print y; y = 5.5; x = y; print x;");

	auto lexer = Lexer::self();

	SourceCode * code = new SourceCode(
		new CodeUnit(
			lexer -> tokenise(input),
			new String("Virtual File"),
			input
		),
		nullptr, nullptr
	);

	auto compiler = Compiler::self();
	Program * program = nullptr;
	try { program = compiler -> compile(code); }
	catch (Program::Error & error) {
		OStream << "Error: " << error.getMessage() << endLine;
		return ExitCodes::failure;
	}

	Decompiler::decompile(program -> instructions);

	auto processor = Processor::self();

	try { processor -> run(program); }
	catch (Program::Error & error) {
		OStream << "Error: " << error.getMessage() << endLine;
		return ExitCodes::failure;
	}

	return ExitCodes::success;
}
