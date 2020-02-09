
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

	void Console::defineLibrary(Environment * global) {
		if (!global) return;
		Class * dec = new Class(
			"Console",
			new Array<AttributeStatement *>(),
			new Dictionary<String, Pair<Modifier, Object *>>()
		);
		dec -> defineStatic("write", Modifier::publicAccess, Console::write());
		dec -> defineStatic("writeLine", Modifier::publicAccess, Console::writeLine());
		dec -> defineStatic("read", Modifier::publicAccess, Console::read());
		dec -> defineStatic("readLine", Modifier::publicAccess, Console::readLine());
		dec -> defineStatic("clean", Modifier::publicAccess, Console::clean());
		global -> define("Console", new Object(BasicType::ClassType, dec));
	}
	Object * Console::write() {
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
	Object * Console::writeLine() {
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
	Object * Console::read() {
		return new Object(BasicType::FunctionType, 
			new NativeFunction(
				[] (Interpreter * i, Array<Object *> a, Token * t) {
					if (!a[0]) return new Object(BasicType::StringType, new String());
					Output << a[0] -> getObjectStringValue();
					return new Object(BasicType::StringType, new String(getInput()));
				}, new Array<Parameter *>({ nullptr }),
				/* The nullptr literal specifies one argument
				of any type that is going to be handled
				later inside the lambda with a typecheck. */
				"<func Console::read>"
			)
		);
	}
	Object * Console::readLine() {
		return new Object(BasicType::FunctionType, 
			new NativeFunction(
				[] (Interpreter * i, Array<Object *> a, Token * t) {
					if (!a[0]) return new Object(BasicType::StringType, new String());
					Output << a[0] -> getObjectStringValue() << endLine;
					return new Object(BasicType::StringType, new String(getInput()));
				}, new Array<Parameter *>({ nullptr }),
				/* The nullptr literal specifies one argument
				of any type that is going to be handled
				later inside the lambda with a typecheck. */
				"<func Console::readLine>"
			)
		);
	}
	Object * Console::setBackground() {
		return nullptr;
	}
	Object * Console::setForeground() {
		return nullptr; // \x1b[38;2;0;0;0m <--- black
	}
	Object * Console::reset() {
		return nullptr; //\x1b[0m
	}
	Object * Console::clean() {
		return new Object(BasicType::FunctionType, 
			new NativeProcedure(
				[] (Interpreter * i, Array<Object *> a, Token * t) {
					Output << "\ec\e[3J";
					return nullptr;
				}, new Array<Parameter *>({ }),
				"<func Console::clean>"
			)
		);
	}

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

}

#endif
