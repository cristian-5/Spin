
/*!
 *
 *    + --------------------------------------- +
 *    |  Libraries.hpp                          |
 *    |                                         |
 *    |             Stack Libraries             |
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

#include "../Aliases/Prototypes.hpp"

#ifndef STACKLIBRARIES
#define STACKLIBRARIES

namespace Stack {

	void Chronos::defineLibrary(Environment * global) {
		if (!global) return;
		global -> define(
			"clock", new Object(BasicType::FunctionType,
				new NativeFunction(
					[] (Interpreter * i, ArrayList<Object *> a) {
						Int64 time = std::chrono::duration_cast
									<std::chrono::milliseconds>
									(std::chrono::system_clock::now()
									.time_since_epoch()).count();
						return new Object(BasicType::Int64Type, new Int64(time));
					}
				)
			)
		);
	}

	void Maths::defineLibrary(Environment * global) {
		if (!global) return;

	}

}

#endif
