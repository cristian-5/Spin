
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

using namespace std;

Int32 main(Int32 argc, Character * argv[]) {

	String * input = new String("+ 2 - 3 * (- 4) + 5");

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
		cout << "Error: " << error.getMessage() << endl;
		return ExitCodes::failure;
	}

	Decompiler::decompile(program -> instructions);

	auto processor = Processor::self();

	try { processor -> run(program); }
	catch (Program::Error & error) {
		cout << "Error: " << error.getMessage() << endl;
		return ExitCodes::failure;
	}

	return ExitCodes::success;
}
