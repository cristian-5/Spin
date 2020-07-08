
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

#include "../Source/Virtual/Processor.hpp"

using namespace Spin;

Int32 main(Int32 argc, Character * argv[]) {

	auto Processor = Processor::self();

	Processor -> run(nullptr);

	return ExitCodes::success;
}
