
/*!
 *
 *    + --------------------------------------- +
 *    |  Libraries.hpp                          |
 *    |                                         |
 *    |             Native Libraries            |
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

#ifndef SPINLIBRARIES
#define SPINLIBRARIES

namespace Spin {

	void Kronos::defineLibrary(Environment * global) {
		if (!global) return;
		global -> define(
			"clock", new Object(BasicType::FunctionType,
				new NativeFunction(
					[] (Interpreter * i, Array<Object *> a, Token * t) {
						Int64 * time = new Int64(std::chrono::duration_cast
												<std::chrono::milliseconds>
												(std::chrono::system_clock::now()
												.time_since_epoch()).count());
						return new Object(BasicType::Int64Type, time);
					}, new Array<Parameter *>(),
					"<native Kronos::clock()>"
				)
			)
		);
	}

	void Maths::defineLibrary(Environment * global) {
		if (!global) return;

	}

	void Standard::defineLibrary(Environment * global) {
		if (!global) return;
		/* String Cast */
		global -> define(
			"String", new Object(BasicType::FunctionType,
				new NativeFunction(
					[] (Interpreter * i, Array<Object *> a, Token * t) {
						if (!a[0]) throw EvaluationError(
							"Dynamic Cast Exception: Invalid Argument!", * t
						);
						return new Object(
							BasicType::StringType,
							new String(a[0] -> getObjectStringValue())
						);
					}, new Array<Parameter *>({ nullptr }),
					"<dynamic ^ String ^ cast>"
					/* The nullptr literal specifies one argument
					   of any type that is going to be handled
					   later inside the lambda with a typecheck. */
				)
			)
		);
		// Define console
	}

}

#endif
