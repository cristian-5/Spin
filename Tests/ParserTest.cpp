
/*!
 *
 *    + --------------------------------------- +
 *    |  ParserTest.cpp                         |
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

#include "../Source/Aliases/Aliases.hpp"
#include "../Source/Collection/StrongList.hpp"
#include "../Source/Lexer/Lexer.hpp"
#include "../Source/Parser/Parser.hpp"
#include "../Source/Parser/ASTPrint.hpp"

using namespace std;

using namespace Collection;
using namespace Stack;

Int32 main(Int32 argc, Character * argv[]) {

	Lexer * lexer = new Lexer();

	cout << "Insert test string: ";
	String test = getInput();

	StrongList<Token> * tokens = nullptr;

	try {
		tokens = lexer -> tokenise(& test, "Virtual File");
	} catch (InvalidTokenException & e) {
		cout << "Error in " << e.getFileName() << "!" << endl;
		cout << "Position [row: " << e.getPosition().row << ", ";
		cout << "col: " << e.getPosition().col << "] Invalid Token!" << endl;
		cout << "Press any key to exit. ";
		waitKeyPress();
		delete lexer;
		delete tokens;
		return exitFailure;
	}

	for (UInt32 i = 0; i < tokens -> count(); i++) {
		cout << "Token " << i + 1 << ": ";
		cout << tokens -> getNode(i).lexeme << endl;
	}

    // Parser Test:

    tokens -> unlinkFirst();
    tokens -> unlinkLast();

    Parser * parser = new Parser();
    Expression * ex = nullptr;

    try {
        ex = parser -> parse(tokens, & test, "Virtual File");
    } catch (SyntaxErrorException & s) {
        FilePosition f = s.getPosition();
		cout << "Error in '" << s.getFileName();
		cout << "' [row: " << f.row << ", col: "
		     << f.col << "];" << endl;
		cout << "Expected '" << s.getExpected() << "' but found '"
			 << s.getToken() << "'!" << endl;
		cout << "Press any key to exit. ";
	    waitKeyPress();
		delete parser;
		delete lexer;
		delete tokens;
		return exitFailure;
    } catch (UnexpectedEndException & u) {
        FilePosition f = u.getPosition();
		cout << "Error in '" << u.getFileName();
		cout << "' [row: " << f.row << ", col: "
		     << f.col << "];" << endl;
		cout << "Sequence ended unexpectedly with token '"
             << u.getToken() << "'!" << endl;
		cout << "Press any key to exit. ";
    	waitKeyPress();
		delete parser;
		delete lexer;
		delete tokens;
		return exitFailure;
    } catch (EmptyUnitException & e) {
		cout << "Error in '" << e.getFileName() << "'!"
			 << endl << "The code unit is empty!" << endl;
		cout << "Press any key to exit. ";
		waitKeyPress();
		delete parser;
		delete lexer;
		delete tokens;
		return exitFailure;
	}

    if (ex != nullptr) {
        cout << "Syntax Tree:" << endl;
        ASTPrinter * printer = new ASTPrinter();
        cout << printer -> print(ex) << endl;
        delete printer;
		delete ex;
    } else {
        cout << "Syntax Tree Failure!" << endl;
        cout << "Press any key to exit. ";
        waitKeyPress();
		delete lexer;
		delete tokens;
        return exitFailure;
    }

	cout << endl << "Press any key to exit. ";
	waitKeyPress();

	delete lexer;
    delete parser;
	delete tokens;
	
	return exitSuccess;
}
