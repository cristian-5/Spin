
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

	Class::Class(String n, Array<FieldStatement *> * d,
				 Dictionary<String, Pair<Modifier, Object *>> * s) {
		name = n; dynamicFields = d; staticFields = s;
	}
	void Class::defineStatic(String name, Modifier access, Object * value) {
		auto search = staticFields -> find(name);
		if (search != staticFields -> end()) {
			throw VariableRedefinitionException(
				(search -> second.second) -> getObjectName()
			);
		} else staticFields -> insert({ name, { access, value } });
	}
	Object * Class::call(Interpreter * i, Array<Object *> a, Token * c) {
		return new Object(BasicType::InstanceType, new Instance(this, i));
	}
	String Class::stringValue() const {
		return "<class " + name + ">";
	}
	UInt32 Class::arity() const { return 0; }
	CallProtocol * Class::copy() const {
		return new Class(name, dynamicFields, staticFields);
	}

	Instance::Instance(Class * t, Interpreter * i) {
		type = t;
		fields = new Dictionary<String, Pair<Modifier, Object *>>();
		fieldInitialisation(i);
	}
	Instance::Instance(Class * t, Dictionary<String, Pair<Modifier, Object *>> * f) {
		type = t;
		fields = f;
	}
	void Instance::fieldInitialisation(Interpreter * i) {
		Instance * backUp = i -> instanceDefinition;
		i -> instanceDefinition = this;
		try {
			for (FieldStatement * field : * type -> dynamicFields) {
				field -> accept(i);
			}
		} catch(Exception & e) { throw; }
		i -> instanceDefinition = backUp;
	}
	void Instance::defineDynamic(String name, Modifier access, Object * value) {
		auto search = fields -> find(name);
		if (search != fields -> end()) {
			throw VariableRedefinitionException(
				(search -> second.second) -> getObjectName()
			);
		} else fields -> insert({ name, { access, value } });
	}
	Object * Instance::getInnerReference(String & name) {
		auto search = fields -> find(name);
		if (search != fields -> end()) {
			return search -> second.second;
		}
		throw VariableNotFoundException();
	}
	Object * Instance::getReference(String & name) {
		auto search = fields -> find(name);
		if (search != fields -> end()) {
			// Returns only if field is accessible (@public):
			if (search -> second.first == Modifier::publicAccess) {
				return search -> second.second;
			}
		}
		throw VariableNotFoundException();
	}
	Object * Instance::getValue(String & name) {
		auto search = fields -> find(name);
		if (search != fields -> end()) {
			// Returns only if field is accessible (not @hidden):
			if (search -> second.first != Modifier::hiddenAccess) {
				return (search -> second.second) -> copy();
			}
		}
		throw VariableNotFoundException();
	}
	String Instance::stringValue() const {
		return "<instance " + type -> name + ">";
	}
	Instance * Instance::copyByValue() const {
		Dictionary<String, Pair<Modifier, Object *>> * f = nullptr;
		f = new Dictionary<String, Pair<Modifier, Object *>>();
		for (Pair<String, Pair<Modifier, Object *>> p : * fields) {
			f -> insert({
				p.first, {
					p.second.first,
					p.second.second -> copy()
				}
			});
		}
		return new Instance(type, f);
	}
	Instance * Instance::copy() const {
		return new Instance(type, fields);
	}
	void Instance::destroy() {
		for (Pair<String, Pair<Modifier, Object *>> p : * fields) {
			p.second.second -> safeDestroy();
		}
		delete fields;
	}

}

#endif
