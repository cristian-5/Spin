
/*!
 *
 *    + --------------------------------------- +
 *    |  Libraries.cpp                          |
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
!*/

#include "../Aliases/Prototypes/Libraries.hpp"

#ifndef SPIN_LIBRARIES
#define SPIN_LIBRARIES

#include "../Aliases/Prototypes/Environment.hpp"
#include "../Aliases/Prototypes/Object.hpp"
#include "../Aliases/Prototypes/Class.hpp"
#include "../Aliases/Prototypes/SyntaxTree.hpp"
#include "../Aliases/Prototypes/Interpreter.hpp"
#include "../Aliases/Prototypes/Routines.hpp"

#include "../Aliases/Input.hpp"

namespace Spin {

	const Dictionary<String, Library::LibraryHandler> Library::libs = {
		{ "Console", Console::defineLibrary }
	};

	void Library::define(String name, Environment * memory) {
		auto search = libs.find(name);
		if (search != libs.end()) {
			auto handler = search -> second;
			handler(memory);
		}
	}
	Bool Library::isKnown(String name) {
		auto search = libs.find(name);
		return search != libs.end();
	}

	const String Console::name = "Console";

	void Console::defineLibrary(Environment * global) {
		if (!global) return;
		Class * dec = new Class(
			name,
			new Array<AttributeStatement *>(),
			new Dictionary<String, Pair<Modifier, Object *>>()
		);
		dec -> defineStatic("write", Modifier::publicAccess, Console::write());
		dec -> defineStatic("writeLine", Modifier::publicAccess, Console::writeLine());
		dec -> defineStatic("read", Modifier::publicAccess, Console::read());
		dec -> defineStatic("readLine", Modifier::publicAccess, Console::readLine());
		dec -> defineStatic("setBackground", Modifier::publicAccess, Console::setBackground());
		dec -> defineStatic("setForeground", Modifier::publicAccess, Console::setForeground());
		dec -> defineStatic("reset", Modifier::publicAccess, Console::reset());
		dec -> defineStatic("clean", Modifier::publicAccess, Console::clean());
		global -> define(name, new Object(BasicType::ClassType, dec));
	}
	Object * Console::write() {
		return new Object(BasicType::RoutineType, 
			new NativeProcedure(
				[] (Array<Object *> a, Token * t) {
					for (Object * o : a) {
						OStream << o -> getObjectStringValue();
					}
					return nullptr;
				}, new Array<Parameter *>(),
				"<proc Console::write>", true
			)
		);
	}
	Object * Console::writeLine() {
		return new Object(BasicType::RoutineType, 
			new NativeProcedure(
				[] (Array<Object *> a, Token * t) {
					for (Object * o : a) {
						OStream << o -> getObjectStringValue() << endLine;
					}
					return nullptr;
				}, new Array<Parameter *>(),
				"<proc Console::writeLine>", true
			)
		);
	}
	Object * Console::read() {
		return new Object(BasicType::RoutineType, 
			new NativeFunction(
				[] (Array<Object *> a, Token * t) {
					for (Object * o : a) {
						OStream << o -> getObjectStringValue();
					}
					return new Object(BasicType::StringType, new String(getInput()));
				}, new Array<Parameter *>(),
				"<func Console::read>", true
			)
		);
	}
	Object * Console::readLine() {
		return new Object(BasicType::RoutineType, 
			new NativeFunction(
				[] (Array<Object *> a, Token * t) {
					for (Object * o : a) {
						OStream << o -> getObjectStringValue() << endLine;
					}
					return new Object(BasicType::StringType, new String(getInput()));
				}, new Array<Parameter *>(),
				"<func Console::readLine>", true
			)
		);
	}
	Object * Console::setBackground() {
		return new Object(BasicType::RoutineType, 
			new NativeProcedure(
				[] (Array<Object *> a, Token * t) {
					if (a.size() == 1) {
						if (a[0] -> isInteger() || a[0] -> isByte()) {
							OStream << "\e[48;5;" << a[0] -> getObjectStringValue() << 'm';
						} else throw ParameterException();
					} else if (a.size() == 3) {
						for (Object * o : a) {
							if (!(o -> isInteger()) && !(o -> isByte())) {
								throw ParameterException();
							}
						}
						OStream << "\e[48;2;"
								<< a[0] -> getObjectStringValue() << ';'
								<< a[1] -> getObjectStringValue() << ';'
								<< a[2] -> getObjectStringValue() << 'm';
					} else throw ParameterException();
					return nullptr;
				}, new Array<Parameter *>(),
				"<proc Console::setBackground>", true
			)
		);
	}
	Object * Console::setForeground() {
		return new Object(BasicType::RoutineType, 
			new NativeProcedure(
				[] (Array<Object *> a, Token * t) {
					if (a.size() == 1) {
						if (a[0] -> isInteger() || a[0] -> isByte()) {
							OStream << "\e[38;5;" << a[0] -> getObjectStringValue() << 'm';
						} else throw ParameterException();
					} else if (a.size() == 3) {
						for (Object * o : a) {
							if (!(o -> isInteger()) && !(o -> isByte())) {
								throw ParameterException();
							}
						}
						OStream << "\e[38;2;"
								<< a[0] -> getObjectStringValue() << ';'
								<< a[1] -> getObjectStringValue() << ';'
								<< a[2] -> getObjectStringValue() << 'm';
					} else throw ParameterException();
					return nullptr;
				}, new Array<Parameter *>(),
				"<proc Console::setForeground>", true
			)
		);
	}
	Object * Console::reset() {
		return new Object(BasicType::RoutineType, 
			new NativeProcedure(
				[] (Array<Object *> a, Token * t) {
					OStream << "\e[0m";
					return nullptr;
				}, new Array<Parameter *>(),
				"<proc Console::reset>"
			)
		);
	}
	Object * Console::clean() {
		return new Object(BasicType::RoutineType, 
			new NativeProcedure(
				[] (Array<Object *> a, Token * t) {
					OStream << "\ec\e[3J";
					return nullptr;
				}, new Array<Parameter *>(),
				"<proc Console::clean>"
			)
		);
	}

	void Kronos::defineLibrary(Environment * global) {
		if (!global) return;
		global -> define(
			"clock", new Object(BasicType::RoutineType,
				new NativeFunction(
					[] (Array<Object *> a, Token * t) {
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
