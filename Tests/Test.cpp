
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

#include "../Source/Manager/Manager.hpp"
#include "../Source/Preprocessor/Wings.hpp"
#include "../Source/Compiler/Compiler.hpp"
#include "../Source/Compiler/Decompiler.hpp"
#include "../Source/Virtual/Processor.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	Compiler * compiler = Compiler::self();
	Processor * processor = Processor::self();

	SourceCode * code = nullptr;
	Program * program = nullptr;

	try {
		code = Wings::spread("Examples/Test.spin");
		program = compiler -> compile(code);
		Decompiler::decompile(program -> instructions);
		processor -> run(program);
	} catch (Program::Error & e) {
		OStream << endLine << endLine << "% " << e.getErrorCode()
				<< " Error on line " << e.getLine() << " of ['"
				<< e.getFile() << "'] %" << endLine
				<< e.getMessage() << endLine << endLine;
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
