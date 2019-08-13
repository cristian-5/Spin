
/*!
 *
 *    + --------------------------------------- +
 *    |  EvalTest.cpp                           |
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
#include "../Source/Lexer/Lexer.hpp"
#include "../Source/Parser/Parser.hpp"
#include "../Source/Parser/ASTPrint.hpp"
#include "../Source/Interpreter/Interpreter.hpp"

using namespace std;

using namespace Stack;

Int32 main(Int32 argc, Character * argv[]) {

	cout << "Insert test string: ";
	String test = getInput();
	cout << endl;

	Lexer * lexer = new Lexer();
	ArrayList<Token> * tokens = nullptr;

	try {
		tokens = lexer -> tokenise(& test, "Virtual File");
	} catch (InvalidTokenException & e) {
		cout << "Error in " << e.getFileName() << "!" << endl;
		cout << "Position [row: " << e.getPosition().row << ", ";
		cout << "col: " << e.getPosition().col << "] Invalid Token!" << endl;
		cout << "Press enter to exit. ";
		waitKeyPress();
		delete lexer;
		return exitFailure;
	}

	delete lexer;

	cout << "Tokens: " << endl;
	UInt32 i = 1;
	for (Token & token : * tokens) {
		cout << padding << i << " | Type: ";
		cout << padding << token.type;
		cout << " | Token: " << token.lexeme << endl;
		i += 1;
	}

	cout << endl;

	tokens -> erase(tokens -> begin());
	tokens -> pop();

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
		cout << "Press enter to exit. ";
		waitKeyPress();
		delete tokens;
		delete parser;
		return exitFailure;
	} catch (UnexpectedEndException & u) {
		FilePosition f = u.getPosition();
		cout << "Error in '" << u.getFileName();
		cout << "' [row: " << f.row << ", col: "
			 << f.col << "];" << endl;
		cout << "Sequence ended unexpectedly with token '"
			 << u.getToken() << "'!" << endl;
		cout << "Press enter to exit. ";
		waitKeyPress();
		delete tokens;
		delete parser;
		return exitFailure;
	} catch (EmptyUnitException & e) {
		cout << "Error in '" << e.getFileName() << "'!"
			 << endl << "The code unit is empty!" << endl;
		cout << "Press enter to exit. ";
		waitKeyPress();
		delete tokens;
		delete parser;
		return exitFailure;
	}

	delete tokens;
	delete parser;

	if (ex == nullptr) {
		cout << "Syntax Tree Failure!" << endl;
		cout << "Press enter to exit. ";
		waitKeyPress();
		return exitFailure;
	}

	cout << "Syntax Tree:" << endl;
	ASTPrinter * printer = new ASTPrinter();
	cout << printer -> print(ex) << endl;
	delete printer;

	cout << endl;

	// Interpreter Test:

	Interpreter * interpreter = new Interpreter();
	Object * result = nullptr;

	try {
		result = interpreter -> evaluate(ex);
	} catch (Exception & e) {
		cout << "Error in file!" << endl;
		cout << "Press enter to exit. ";
		waitKeyPress();
		delete ex;
		delete interpreter;
		return exitFailure;
	}

	delete ex;
	delete interpreter;

	if (result == nullptr) {
		cout << "Evaluation Failure!" << endl;
		cout << "Press enter to exit. ";
		waitKeyPress();
		return exitFailure;
	}

	cout << "ResultType: " << result -> getObjectName() << endl;
	cout << "Result: " << result -> getObjectStringValue() << endl;

	delete result;

	cout << endl << "Press enter to exit. ";
	waitKeyPress();
	
	return exitSuccess;
}
