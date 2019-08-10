
/*!
 *
 *    + --------------------------------------- +
 *    |  VMachineTest.cpp                       |
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
#include "../Source/Processor/OPCode.hpp"

using namespace std;

using namespace Stack;

Int32 main(Int32 argc, Character * argv[]) {

	CodeChunk code = CodeChunk();

	code.append((UInt8)OPCode::RET);

	cout << code.disassembly();

	return exitSuccess;
}
