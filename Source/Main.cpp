
/*!
 *
 *    + --------------------------------------- +
 *    |  main.cpp                               |
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
 */

#include "Aliases/Includes.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	if (argc == 2 && String(argv[1]) == "-v") {
		Output << "Spin Language Version: 3.0 beta.";
	}

	Array<FileFrame *> * files = new Array<FileFrame *>();
	for (UInt64 i = 1; i < argc; i++) {
		String * file = nullptr;
		try {
			file = Linker::stringFromFile(argv[i]);
			files -> push(
				new FileFrame(
					nullptr,
					new String(argv[i]),
					file
				)
			);
		} catch (BadFileException & exc) {
			for (FileFrame * f : * files) delete f; delete files;
			Output << "Bad Access! Invalid input file at path '"
				   << exc.getPath() << "'!" << endLine;
			Output << "Press enter to exit. ";
			waitKeyPress();
			return exitFailure;
		}
	}

	/* TO FIX: Filescope */

	Lexer * lexer = Lexer::self();

	for (FileFrame * file : * files) {
		file -> tokens = lexer -> tokenise(
			file -> contents
		);
	}

	Parser * parser = Parser::self();
	FileScope * fs = nullptr;


	

	try {
		fs = parser -> parse(tokens, & test, "Virtual File");
	} catch (ParserErrorException & p) {
		const Array<SyntaxError> * const e = p.getErrors();
		Output << "Found " << e -> size() << " errors in '"
			   << p.getFileName() << "'!" << endLine;
		UInt32 i = 1;
		for (SyntaxError s : * e) {
			FilePosition f = s.getPosition();
			Output << padding << i << " [" << f.row
				   << ":" << f.col << "]: "
				   << s.getMessage() << endLine;
			i += 1;
		}
		Output << endLine << "Press enter to exit. ";
		waitKeyPress();
		delete tokens;
		return exitFailure;
	}

	delete tokens;

	if (!fs) {
		Output << "File Scope Failure!" << endLine;
		Output << "Press enter to exit. ";
		waitKeyPress();
		return exitFailure;
	}

	Interpreter * interpreter = Interpreter::self();

	try {
		interpreter -> evaluate(fs, & test, "Virtual File");
	} catch (InterpreterErrorException & e) {
		Output << "[" << e.getPosition().row << ":";
		Output << e.getPosition().col << "]: " << e.getMessage() << endLine;
		Output << "Press enter to exit. ";
		waitKeyPress();
		delete fs;
		return exitFailure;
	}

	delete fs;*/

	Output << endLine << "Press enter to exit. ";
	waitKeyPress();
	
	return exitSuccess;
}
