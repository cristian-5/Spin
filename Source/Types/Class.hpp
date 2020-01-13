
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

	Instance::Instance(Class * t) {
		fields = new Dictionary<String, Object *>();
		type = t;
	}
	Instance::Instance(Class * t, Dictionary<String, Object *> * f) {
		fields = f;
		type = t;
	}
	Object * Instance::getReference(String & name) {
		// TODO: create a function to edit the variable
		//       from the inside of the class.
		auto search = fields -> find(name);
		if (search != fields -> end()) {
			// TODO: Only if field is
			//       accessible (@public):
			return search -> second;
		}
		throw VariableNotFoundException();
	}
	Object * Instance::getValue(String & name) {
		auto search = fields -> find(name);
		if (search != fields -> end()) {
			// TODO: Only if field is
			//       accessible (not @hidden):
			return (search -> second) -> copy();
		}
		throw VariableNotFoundException();
	}
	String Instance::stringValue() const {
		if (!type) return "<empty>";
		return "<instance " + type -> name + ">";
	}
	Instance * Instance::copy() const {
		return new Instance(type, fields);
	}
	void Instance::destroy() {
		// TODO: Need to destroy every variable man...
		delete fields;
	}

}

#endif
