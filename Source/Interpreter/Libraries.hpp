
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
						return new Object(BasicType::IntegerType, time);
					}, new Array<Parameter *>(),
					"<native Kronos::clock()>"
				)
			)
		);
	}

	void Maths::defineLibrary(Environment * global) {
		if (!global) return;
	}

	void Foundation::defineLibrary(Environment * global) {
		if (!global) return;

		/* String Cast 
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
					 The nullptr literal specifies one argument
					   of any type that is going to be handled
					   later inside the lambda with a typecheck.
				)
			)
		);*/

		Class * dec = new Class(
			"Console",
			new Array<AttributeStatement *>(),
			new Dictionary<String, Pair<Modifier, Object *>>()
		);
		dec -> defineStatic("write", Modifier::publicAccess, Console::write());
		dec -> defineStatic("writeLine", Modifier::publicAccess, Console::writeLine());

		global -> define("Console", new Object(BasicType::ClassType, dec));

	}

	Object * Foundation::Console::write() {
		return new Object(BasicType::FunctionType, 
			new NativeProcedure(
				[] (Interpreter * i, Array<Object *> a, Token * t) {
					if (!a[0]) return nullptr;
					Output << a[0] -> getObjectStringValue();
					return nullptr;
				}, new Array<Parameter *>({ nullptr }),
				/* The nullptr literal specifies one argument
				of any type that is going to be handled
				later inside the lambda with a typecheck. */
				"<proc Console::write>"
			)
		);
	}
	Object * Foundation::Console::writeLine() {
		return new Object(BasicType::FunctionType, 
			new NativeProcedure(
				[] (Interpreter * i, Array<Object *> a, Token * t) {
					if (!a[0]) return nullptr;
					Output << a[0] -> getObjectStringValue() << endLine;
					return nullptr;
				}, new Array<Parameter *>({ nullptr }),
				/* The nullptr literal specifies one argument
				of any type that is going to be handled
				later inside the lambda with a typecheck. */
				"<proc Console::writeLine>"
			)
		);
	}
	Object * Foundation::Console::read() {
		return nullptr;
	}
	Object * Foundation::Console::readLine() {
		return nullptr;
	}

}

#endif
