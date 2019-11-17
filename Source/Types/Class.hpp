
/*!
 *
 *    + --------------------------------------- +
 *    |  Class.hpp                              |
 *    |                                         |
 *    |               Class Type                |
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

#ifndef SPINCLASS
#define SPINCLASS

namespace Spin {

	Class::Class(String n) { name = n; }
	Object * Class::call(Interpreter * i, Array<Object *> a, Token * c) {
		return new Object(BasicType::InstanceType, new Instance(this));
	}
	String Class::stringValue() const {
		return "<class " + name + ">";
	}
	UInt32 Class::arity() const { return 0; }
	CallProtocol * Class::copy() const {
		return new Class(name);
	}

	Instance::Instance(Class * t) { type = t; }
	String Instance::stringValue() const {
		if (!type) return "<empty>";
		return "<instance " + type -> name + ">";
	}
	Instance * Instance::copy() const {
		return new Instance(type);
	}

}

#endif
