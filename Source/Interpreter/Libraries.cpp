
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

// These macros are specifically designed to
// help the programmer use mnemonics instead
// of gibberish that he will forget after a
// while, causing problems in later development.

#define CustomParameters true
#define PrepareCustomParameters new Array<Parameter *>()
#define NoParameters new Array<Parameter *>()
#define GenericParameter nullptr
#define PrepareParameters(P) new Array<Parameter *>({ P })
#define BasicParameter(P) new Parameter(P, nullptr, nullptr)

namespace Spin {

	const Dictionary<String, Library::LibraryHandler> Library::libs = {
		{ "Console", Console::defineLibrary }
	};

	void Library::defineVirual(Environment * memory) {
		if (!memory) return;
		VirtualBoolean::defineLibrary(memory);
		VirtualString::defineLibrary(memory);
	}
	void Library::define(String name, Environment * memory) {
		if (!memory) return;
		auto search = libs.find(name);
		if (search != libs.end()) {
			auto handler = search -> second;
			handler(memory);
		}
	}
	Boolean Library::isKnown(String name) {
		auto search = libs.find(name);
		return search != libs.end();
	}

	// Static Libraries:

	const String Console::name = "Console";

	void Console::defineLibrary(Environment * global) {
		if (!global) return;
		global -> define(name,
			new Object(BasicType::ClassType,
				new Class(name,
					new Array<AttributeStatement *>(),
					new Dictionary<String, Pair<Modifier, Object *>>({
						{
							"write", {
								Modifier::publicAccess,
								new Object(BasicType::RoutineType, 
									new NativeProcedure(
										[] (Array<Object *> a, Token * t) {
											for (Object * o : a) {
												OStream << o -> getObjectStringValue();
											}
											return nullptr;
										}, PrepareCustomParameters,
										"<proc Console::write>",
										CustomParameters
									)
								)
							}
						},
						{
							"writeLine", {
								Modifier::publicAccess,
								new Object(BasicType::RoutineType, 
									new NativeProcedure(
										[] (Array<Object *> a, Token * t) {
											for (Object * o : a) {
												OStream << o -> getObjectStringValue() << endLine;
											}
											return nullptr;
										}, PrepareCustomParameters,
										"<proc Console::writeLine>",
										CustomParameters
									)
								)
							}
						},
						{
							"read", {
								Modifier::publicAccess,
								new Object(BasicType::RoutineType, 
									new NativeFunction(
										[] (Array<Object *> a, Token * t) {
											for (Object * o : a) {
												OStream << o -> getObjectStringValue();
											}
											return new Object(BasicType::StringType, new String(getInput()));
										}, PrepareCustomParameters,
										"<func Console::read>",
										CustomParameters
									)
								)
							}
						},
						{
							"readLine", {
								Modifier::publicAccess,
								new Object(BasicType::RoutineType, 
									new NativeFunction(
										[] (Array<Object *> a, Token * t) {
											for (Object * o : a) {
												OStream << o -> getObjectStringValue() << endLine;
											}
											return new Object(BasicType::StringType, new String(getInput()));
										}, PrepareCustomParameters,
										"<func Console::readLine>",
										CustomParameters
									)
								)
							}
						},
						{
							"setBackground", {
								Modifier::publicAccess,
								new Object(BasicType::RoutineType, 
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
										}, PrepareCustomParameters,
										"<proc Console::setBackground>",
										CustomParameters
									)
								)
							}
						},
						{
							"setForeground", {
								Modifier::publicAccess,
								new Object(BasicType::RoutineType, 
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
										}, PrepareCustomParameters,
										"<proc Console::setForeground>",
										CustomParameters
									)
								)
							}
						},
						{
							"newLine", {
								Modifier::publicAccess,
								new Object(BasicType::RoutineType, 
									new NativeProcedure(
										[] (Array<Object *> a, Token * t) {
											OStream << endLine;
											return nullptr;
										}, NoParameters,
										"<proc Console::newLine>"
									)
								)
							}
						},
						{
							"reset", {
								Modifier::publicAccess,
								new Object(BasicType::RoutineType, 
									new NativeProcedure(
										[] (Array<Object *> a, Token * t) {
											OStream << "\e[0m";
											return nullptr;
										}, NoParameters,
										"<proc Console::reset>"
									)
								)
							}
						},
						{
							"clean", {
								Modifier::publicAccess,
								new Object(BasicType::RoutineType, 
									new NativeProcedure(
										[] (Array<Object *> a, Token * t) {
											OStream << "\ec\e[3J";
											return nullptr;
										}, NoParameters,
										"<proc Console::clean>"
									)
								)
							}
						}
					})
				)
			)
		);
	}

	// Static Virtual Classes:

	const String VirtualBoolean::name = "Boolean";

	void VirtualBoolean::defineLibrary(Environment * global) {
		if (!global) return;
		global -> define(name,
			new Object(BasicType::ClassType,
				new Class(name,
					new Array<AttributeStatement *>(),
					new Dictionary<String, Pair<Modifier, Object *>>({
						{
							"from", {
								Modifier::publicAccess,
								new Object(BasicType::RoutineType, 
								new NativeFunction(
										[] (Array<Object *> a, Token * t) {
											return new Object(
												BasicType::BooleanType,
												new Boolean(a[0] -> getBoolValue())
											);
										}, PrepareParameters(
											BasicParameter(BasicType::BooleanType)
										),
										"<func Boolean::from>"
									)
								)
							}
						}
					})
				)
			)
		);
	}

	const String VirtualString::name = "String";

	void VirtualString::defineLibrary(Environment * global) {
		if (!global) return;
		global -> define(name,
			new Object(BasicType::ClassType,
				new Class(name,
					new Array<AttributeStatement *>(),
					new Dictionary<String, Pair<Modifier, Object *>>({
						{
							"empty", {
								Modifier::publicAccess,
								new Object(BasicType::StringType)
							}
						},
						{
							"from", {
								Modifier::publicAccess,
								new Object(BasicType::RoutineType, 
									new NativeFunction(
										[] (Array<Object *> a, Token * t) {
											return new Object(
												BasicType::StringType,
												new String(a[0] -> getObjectStringValue())
											);
										}, PrepareParameters(GenericParameter),
										"<func String::from>"
									)
								)
							}
						}
					})
				)
			)
		);
	}

}

#endif
