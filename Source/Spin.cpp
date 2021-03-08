
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
#include "Compiler/Decompiler.hpp"
#include "Virtual/Processor.hpp"
#include "Utility/Serialiser.hpp"
#include "Utility/Arguments.hpp"

#define VERSION                          \
	"\n% Spin programming language %"    \
	"\nCurrent version: 3.0.0 beta.\n\n"

#define ERROR_01                                     \
	"\n% Spin catastrophic event %"                  \
	"\nYou forgot to specify the source file!"       \
	"\nType spin -h and I'll guide you through.\n\n"

#define ERROR_02                                         \
	"\n% Spin catastrophic event (I'm not your maid!) %" \
	"\nYou gave me too many arguments! Are you insane?"  \
	"\nType spin -h and I'll guide you through.\n\n"

#define ERROR_03                                     \
	"\n% Spin catastrophic event %"                  \
	"\nNo viable option has been selected!"          \
	"\nType spin -h and I'll guide you through.\n\n"

#define ERROR_04                                     \
	"\n% Spin catastrophic event %"                  \
	"\nInput file has invalid extension!"    \
	"\nType spin -h and I'll guide you through.\n\n"

using namespace Spin;
using namespace CommandLine;

void printBadFile(Manager::BadFileException & b);
void printProgramError(Program::Error & e);
void printReadingError(Serialiser::ReadingError & r, String path);
void printProcessorCrash(Processor::Crash & c);

Int32 processCode(String path, Boolean noAnsi, Compiler::Options options);
Int32 compileCode(String source, String destination,
				  Boolean noAnsi, Compiler::Options options);
Int32 decompileCode(String source, Boolean noAnsi);

Int32 main(Int32 argc, Character * argv[]) {

	Arguments::program = "Spin";
	Arguments::strictMode = false;

	Arguments::onHelp = [] () {
		OStream << endLine << "% Spin help magician (it's helpful) %"
				<< endLine << "Usage:"
				<< endLine << "    spin <file>"
				<< endLine << "         Compiles and executes a file."
				<< endLine << "    spin [-compile, -c] <file.spin> <file.sexy>"
				<< endLine << "         Compiles a file into a binary."
				<< endLine << "    spin [-decompile, -d] <file.sexy>"
				<< endLine << "         Decompiles a binary file."
				<< endLine << "    spin [-version, -v]"
				<< endLine << "         Shows the version number."
				<< endLine << "    .... [-noAnsi, -n]"
				<< endLine << "         Disable ansi output."
				<< endLine << "  <file>: should be the main file and"
				<< endLine << "          it should end with '.spin' or"
				<< endLine << "          '.sexy' if it's a binary file."
				<< endLine << "  <file.spin>: should be the source file."
				<< endLine << "  <file.sexy>:  should be the binary file."
				<< endLine << "I told you it was helpful!"
				<< endLine << endLine;
	};
	Arguments::onUndefined = [] (String c, String s) {
		OStream << endLine << "% Spin catastrophic event %"
				<< endLine << "Command '" << c << "' is undefined!"
				<< endLine << "Did you mean '" << s << "' instead?"
				<< endLine << "Type spin -h and I'll guide you through."
				<< endLine << endLine;
	};
	Arguments::onRedefinition = [] (String c) {
		OStream << endLine << "% Spin catastrophic event %"
				<< endLine << "Command '" << c << "' has already been defined!"
				<< endLine << "Type spin -h and I'll guide you through."
				<< endLine << endLine;
	};
	Arguments::onUnclear = [] (String c, SizeType s) {
		OStream << endLine << "% Spin catastrophic event %"
				<< endLine << "Command '" << c << "' needs at least " << s << " parameters!"
				<< endLine << "Type spin -h and I'll guide you through."
				<< endLine << endLine;
	};
	
	Parameters::onExclusion = [] (String a, String b) {
		OStream << endLine << "% Spin catastrophic event %"
				<< endLine << "Command '" << a
				<< "' is conflicting with command '" << b << "'!"
				<< endLine << "Type spin -h and I'll guide you through."
				<< endLine << endLine;
	};

	Arguments::options = {
		{   "-compile", "-c", 2 },
		{ "-decompile", "-d", 1 },
		{   "-version", "-v" },
		{    "-noAnsi", "-n" },
		{ "-noFolding", "-f" },
		{   "-sectors", "-s" },
	};

	Parameters parameters = Arguments::parse(argc, argv);
	if (Arguments::helpRaised) return ExitCodes::success;
	if   (parameters.failed()) return ExitCodes::failure;

	const Boolean noAnsi = parameters["-noAnsi"].to<Boolean>();

	Compiler::Options options = {
		parameters["-noFolding"].to<Boolean>(),
		parameters["-sectors"].to<Boolean>()
	};

	if (parameters["-version"].to<Boolean>()) {
		OStream << VERSION;
		return ExitCodes::success;
	}

	parameters.removeOptionals({
		"-version", "-noAnsi", "-noFolding", "-sectors"
	});

	if (parameters.size() == 0) {
		// Its either `spin file.spin`
		//         or `spin file.sexy`.
		switch (parameters.sizeOfFree()) {
			case 0:
				OStream << ERROR_01;
				return ExitCodes::failure;
			break;
			case 1: break;
			default:
				OStream << ERROR_02;
				return ExitCodes::failure;
			break;
		}
		return processCode(
			parameters.freeParameters.at(0),
			noAnsi, options
		);
	} else {
		// Its either `spin -compile file.spin file.sexy`
		//         or `spin -decompile file.sexy`
		String selected = parameters.mutualExclusion({
			"-compile", "-decompile"
		});
		if (parameters.exclusionFailed()) {
			return ExitCodes::failure;
		}
		if (selected.empty()) {
			OStream << ERROR_03;
			return ExitCodes::failure;
		}
		if (parameters.sizeOfFree() != 0) {
			OStream << ERROR_02;
			return ExitCodes::failure;
		}
		switch (selected[1]) {
			case 'c': {
				Array<String> cP = parameters["-compile"].toVector<String>();
				if (cP[0].ends_with(".spin")) {
					return compileCode(cP[0], cP[1], noAnsi, options);
				}
				return compileCode(cP[1], cP[0], noAnsi, options);
			} break;
			case 'd': {
				return decompileCode(
					parameters["-decompile"].to<String>(),
					noAnsi
				);
			} break;
			case 'v':
				OStream << VERSION;
				return ExitCodes::success;
			break;
			default: return ExitCodes::failure; break;
		}
	}
	
	return ExitCodes::success;
}

void printBadFile(Manager::BadFileException & b) {
	OStream << endLine <<  "% SYS Catastrophic Event %"
			<< endLine << "Couldn't open or read file ['"
			<< b.getPath() << "']!" << endLine << endLine;
}
void printProgramError(Program::Error & e) {
	OStream << endLine << "% " << e.getErrorCode()
			<< " Error on line " << e.getLine() << " of ['"
			<< e.getFile() << "'] %" << endLine
			<< e.getMessage() << endLine << endLine;
}
void printReadingError(Serialiser::ReadingError & r, String path) {
	OStream << endLine <<  "% PPR Catastrophic Event %"
			<< endLine << "Couldn't read invalid file ['"
			<< path << "']!" << endLine << endLine;
}
void printProcessorCrash(Processor::Crash & c) {
	OStream << endLine << "% EVL Error on address [0x"
			<< hexadecimal << padding(8) << c.getAddress()
			<< "] %" << endLine << "Crash bytecode: "
			<< hexadecimal << padding(2)
			<< c.getInstruction().code << " "
			<< hexadecimal << padding(16)
			<< hexadecimal << c.getInstruction().as.index
			<< endLine << endLine;
}

Int32 processCode(String path, Boolean noAnsi, Compiler::Options options) {
	Program * program = nullptr;
	if (path.ends_with(".spin")) {
		Compiler * compiler = Compiler::self();
		compiler -> options = options;
		SourceCode * code = nullptr;
		try {
			code = Wings::spread(path);
			program = compiler -> compile(code);
		} catch (Program::Error & e) {
			printProgramError(e);
			if (code) delete code;
			if (program) delete program;
			return ExitCodes::failure;
		} catch (Manager::BadFileException & b) {
			printBadFile(b);
			if (code) delete code;
			if (program) delete program;
			return ExitCodes::failure;
		}
		delete code;
	} else if (path.ends_with(".sexy")) {
		try { program = Program::from(path); }
		catch (Serialiser::ReadingError & r) {
			printReadingError(r, path);
			return ExitCodes::failure;
		} catch (Manager::BadFileException & b) {
			printBadFile(b);
			return ExitCodes::failure;
		}
	} else {
		OStream << ERROR_04;
		return ExitCodes::failure;
	}
	Processor * processor = Processor::self();
	try { processor -> run(program); }
	catch (Processor::Crash & c) {
		printProcessorCrash(c);
		if (program) delete program;
		return ExitCodes::failure;
	}
	delete program;
	return ExitCodes::success;
}
Int32 compileCode(String source, String destination,
				  Boolean noAnsi, Compiler::Options options) {
	if (!source.ends_with(".spin")) {
		OStream << ERROR_04;
		return ExitCodes::failure;
	}
	Compiler * compiler = Compiler::self();
	compiler -> options = options;
	SourceCode * code = nullptr;
	Program * program = nullptr;
	try {
		code = Wings::spread(source);
		program = compiler -> compile(code);
		delete code; code = nullptr;
		program -> serialise(destination);
	} catch (Program::Error & e) {
		printProgramError(e);
		if (code) delete code;
		if (program) delete program;
		return ExitCodes::failure;
	} catch (Manager::BadFileException & b) {
		printBadFile(b);
		if (code) delete code;
		if (program) delete program;
		return ExitCodes::failure;
	}
	return ExitCodes::success;
}
Int32 decompileCode(String source, Boolean noAnsi) {
	if (!source.ends_with(".sexy")) {
		OStream << ERROR_04;
		return ExitCodes::failure;
	}
	Program * program = nullptr;
	try { program = Program::from(source); }
	catch (Manager::BadFileException & b) {
		printBadFile(b);
		return ExitCodes::failure;
	} catch (Serialiser::ReadingError & r) {
		printReadingError(r, source);
		return ExitCodes::failure;
	}
	Decompiler::decompile(program, noAnsi);
	delete program;
	return ExitCodes::success;
}

#undef VERSION
#undef ERROR_01
#undef ERROR_02
#undef ERROR_03
#undef ERROR_04
