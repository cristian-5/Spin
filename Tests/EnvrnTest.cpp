
/*!
 *
 *    + --------------------------------------- +
 *    |  EnvrnTest.cpp                          |
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
#include "../Source/Interpreter/Environment.hpp"

using namespace std;

using namespace Stack;

Int32 main(Int32 argc, Character * argv[]) {

	Environment * var = new Environment();

	var -> define("x", new Object(BasicType::Int64Type, new Int64(7)));
	var -> define("y", new Object(BasicType::StringType, new String("f")));

	cout << var -> getReference("x") -> getObjectStringValue();

	cout << endl << "Press enter to exit. ";
	waitKeyPress();
	
	return exitSuccess;
}
