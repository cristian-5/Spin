
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
#include "../Aliases/Prototypes/Object.hpp"
#include "../Aliases/Prototypes/Environment.hpp"

namespace Spin {

	Object * BasicString::handleGetValue(String * self, String & name) {
		if (name == "isEmpty") return getIsEmptyMethod(self);
		else if (name == "length") return getLengthProperty(self);
		throw Environment::VariableNotFoundException();
	}

	inline Object * BasicString::getLengthProperty(String * self) {
		return new Object(
			BasicType::IntegerType,
			new Int64(self -> length())
		);
	}

	inline Object * BasicString::getIsEmptyMethod(String * self) {
		return new Object(
			BasicType::RoutineType,
			new NativeFunction(
				[self] (Array<Object *> a, Token * t) {
					return new Object(
						BasicType::BoolType,
						new Bool(self -> empty())
					);
				}, new Array<Parameter *>(),
				"<func String.isEmpty>"
			)
		);
	}

}

#endif
