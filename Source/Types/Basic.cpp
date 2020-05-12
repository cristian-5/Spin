
/*!
 *
 *    + --------------------------------------- +
 *    |  Basic.cpp                              |
 *    |                                         |
 *    |               Basic Type                |
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

#include "../Aliases/Prototypes/Basic.hpp"

#ifndef SPIN_BASIC
#define SPIN_BASIC

#include "../Aliases/Prototypes/Routines.hpp"

// This macro is extremely important since it generates
// code for the initialisation of the attributes
// of a virtual class for a given type T.
// Essentially, it will make your life easier.

#define Attributes(T) Dictionary<String, Basic ## T :: T ## Handler> Basic ## T

// These macros are specifically designed to
// help the programmers use mnemonics instead
// of gibberish that they will forget after a
// while, causing problems in later development.

#define CustomParameters true
#define PrepareCustomParameters new Array<Parameter *>()
#define NoParameters new Array<Parameter *>()
#define GenericParameter nullptr
#define PrepareParameters(P) new Array<Parameter *>({ P })
#define BasicParameter(P) new Parameter(P, nullptr, nullptr)

namespace Spin {

	const Attributes(Boolean)::attributes = {
		{
			"toggle", [] (Boolean * self) -> Object * {
				return new Object(
					BasicType::RoutineType,
					new NativeProcedure(
						[self] (Array<Object *> a, Token * t) {
							* self = !(* self);
							return nullptr;
						}, NoParameters,
						"<proc Boolean.toggle>"
					)
				);
			}
		},
		{
			"toString", [] (Boolean * self) -> Object * {
				return new Object(
					BasicType::RoutineType,
					new NativeFunction(
						[self] (Array<Object *> a, Token * t) {
							return new Object(
								BasicType::StringType,
								new String((* self) ? "true" : "false")
							);
						}, NoParameters,
						"<func Boolean.toString>"
					)
				);
			}
		}
	};
	const Attributes(String)::attributes = {
		{
			"length", [] (String * self) -> Object * {
				return new Object(
					BasicType::IntegerType,
					new Int64(self -> length())
				);
			}
		},
		{
			"append", [] (String * self) -> Object * {
				return new Object(
					BasicType::RoutineType,
					new NativeProcedure(
						[self] (Array<Object *> a, Token * t) {
							(* self) += a[0] -> getObjectStringValue();
							return nullptr;
						}, PrepareParameters(GenericParameter),
						"<func String.append>"
					)
				);
			}
		},
		{
			"contains", [] (String * self) -> Object * {
				return new Object(
					BasicType::RoutineType,
					new NativeFunction(
						[self] (Array<Object *> a, Token * t) {
							if (a[0] -> type == BasicType::StringType) {
								return new Object(
									BasicType::BooleanType,
									new Boolean(
										(self -> find(
											* (String *)(a[0] -> value)
										)) != String::npos
									)
								);
							} else if (a[0] -> type == BasicType::CharacterType) {
								return new Object(
									BasicType::BooleanType,
									new Boolean(
										(self -> find(
											* (Character *)(a[0] -> value)
										)) != String::npos
									)
								);
							} else throw ParameterException();
						}, PrepareParameters(GenericParameter),
						"<func String.contains>"
					)
				);
			}
		},
		{
			"clear", [] (String * self) -> Object * {
				return new Object(
					BasicType::RoutineType,
					new NativeProcedure(
						[self] (Array<Object *> a, Token * t) {
							self -> clear();
							return nullptr;
						}, NoParameters,
						"<proc String.clear>"
					)
				);
			}
		},
		{
			"isEmpty", [] (String * self) -> Object * {
				return new Object(
					BasicType::RoutineType,
					new NativeFunction(
						[self] (Array<Object *> a, Token * t) {
							return new Object(
								BasicType::BooleanType,
								new Boolean(self -> empty())
							);
						}, NoParameters,
						"<func String.isEmpty>"
					)
				);
			}
		}
	};

}

#endif
