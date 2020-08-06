
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

#include "Benchmark/Benchmark.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	Compiler * compiler = Compiler::self();
	Processor * processor = Processor::self();

	SourceCode * code = nullptr;
	Program * program = nullptr;

	try {
		code = Wings::spread("Examples/Test.spin");
		program = compiler -> compile(code);
		Decompiler::decompile(program);
		Timer::start();
		processor -> run(program);
		Timer::stop();
	} catch (Program::Error & e) {
		OStream << endLine << "% " << e.getErrorCode()
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
		OStream << endLine <<  "% PPR Catastrophic Event %"
				<< endLine << "Couldn't open file ['"
				<< b.getPath() << "']!" << endLine << endLine;
		if (code) delete code;
		if (program) delete program;
		return ExitCodes::failure;
	}

	OStream << endLine << "% BMK Benchmark Time %"
			<< endLine << "Execution time: " << Timer::time
			<< "ms." << endLine << endLine;

	delete code; delete program;

	return ExitCodes::success;
}
