
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

#include "Aliases/Aliases.hpp"
#include "Collection/Collection.hpp"
#include "Compiler/Compiler.hpp"
#include "Parser/ASTPrint.hpp"

using namespace std;

using namespace Collection;
using namespace Stack;

Int32 main(Int32 argc, Character * argv[]) {

	/*StrongList<String> files = StrongList<String>();
	String s = "main.stk"; files.link(s);
	s = "library.stk"; files.link(s);

	Transpiler transpiler = Transpiler();

	cout << "Transpiling Files ..." << endl;

	try {
		transpiler.processFiles(files);
	} catch (InvalidTokenException & i) {
		FilePosition f = i.getPosition();
		cout << "Error in '" << i.getFileName();
		cout << "' [row: " << f.row << ", col: "
		     << f.col << "];" << endl;
		cout << "Unrecognised Token!";
		cin.get();
		return exitFailure;
	} catch (BadFileException & b) {
		cout << "Invalid File '" << b.getPath() << "'!";
		cin.get();
		return exitFailure;
	} catch (SyntaxErrorException & s) {
		FilePosition f = s.getPosition();
		cout << "Error in '" << s.getFileName();
		cout << "' [row: " << f.row << ", col: "
		     << f.col << "];" << endl;
		cout << "Expected '" << s.getExpected() << "' but found '"
			 << s.getToken() << "'!";
		cin.get();
		return exitFailure;
	} catch (Exception & e) {
		cout << "We encountered an unknown exception!";
		cin.get();
		return exitFailure;
	}

	cout << "Successfully Transpiled!";*/

	return exitSuccess;
}
