
/*!
 *
 *    + --------------------------------------- +
 *    |  Class.cpp                              |
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
!*/

#include "../Aliases/Prototypes/Class.hpp"

#ifndef SPIN_CLASS
#define SPIN_CLASS

#include "../Aliases/Prototypes/SyntaxTree.hpp"
#include "../Aliases/Prototypes/Environment.hpp"
#include "../Aliases/Prototypes/Interpreter.hpp"

namespace Spin {

	Class::Class(String n, Array<AttributeStatement *> * d, Dictionary<String, Pair<Modifier, Object *>> * s) {
		name = n;
		dynamicAttributes = d;
		staticAttributes = s;
	}
	void Class::defineStatic(String name, Modifier access, Object * value) {
		auto search = staticAttributes -> find(name);
		if (search != staticAttributes -> end()) {
			throw Environment::VariableRedefinitionException(
				(search -> second.second) -> getObjectName()
			);
		} else staticAttributes -> insert({ name, { access, value } });
	}
	Object * Class::call(Interpreter * i, Array<Object *> a, Token * c) {
		return new Object(BasicType::InstanceType, new Instance(this, i));
	}
	Object * Class::getInnerReference(String & name) {
		auto search = staticAttributes -> find(name);
		if (search != staticAttributes -> end()) {
			return search -> second.second;
		}
		throw Environment::VariableNotFoundException();
	}
	Object * Class::getInnerValue(String & name) {
		auto search = staticAttributes -> find(name);
		if (search != staticAttributes -> end()) {
			return (search -> second.second) -> copy();
		}
		throw Environment::VariableNotFoundException();
	}
	Object * Class::getReference(String & name) {
		auto search = staticAttributes -> find(name);
		if (search != staticAttributes -> end()) {
			// Returns only if attribute is accessible (@public):
			if (search -> second.first == Modifier::publicAccess) {
				return search -> second.second;
			}
		}
		throw Environment::VariableNotFoundException();
	}
	Object * Class::getValue(String & name) {
		auto search = staticAttributes -> find(name);
		if (search != staticAttributes -> end()) {
			// Returns only if attribute is accessible (not @hidden):
			if (search -> second.first != Modifier::hiddenAccess) {
				return (search -> second.second) -> copy();
			}
		}
		throw Environment::VariableNotFoundException();
	}
	String Class::stringValue() const {
		return "<class " + name + ">";
	}
	UInt32 Class::arity() const { return 0; }
	CallProtocol * Class::copy() const {
		return new Class(name, dynamicAttributes, staticAttributes);
	}
	void Class::destroy() {
		for (Pair<String, Pair<Modifier, Object *>> p : * staticAttributes) {
			p.second.second -> safeDestroy();
		}
		delete staticAttributes;
		// No need to delete dynamicAttributes
		// because they are still in the syntax
		// tree and will be automatically removed.
	}

	Instance::Instance(Class * t, Interpreter * i) {
		type = t;
		attributes = new Dictionary<String, Pair<Modifier, Object *>>();
		attributesInitialisation(i);
	}
	Instance::Instance(Class * t, Dictionary<String, Pair<Modifier, Object *>> * a) {
		type = t;
		attributes = a;
	}
	void Instance::attributesInitialisation(Interpreter * i) {
		Instance * backUp = i -> instanceDefinition;
		i -> instanceDefinition = this;
		try {
			for (AttributeStatement * attribute : * type -> dynamicAttributes) {
				attribute -> accept(i);
			}
		} catch(Exception & e) { throw; }
		i -> instanceDefinition = backUp;
	}
	void Instance::defineDynamic(String name, Modifier access, Object * value) {
		auto search = attributes -> find(name);
		if (search != attributes -> end()) {
			throw Environment::VariableRedefinitionException(
				(search -> second.second) -> getObjectName()
			);
		} else attributes -> insert({ name, { access, value } });
	}
	Object * Instance::getInnerReference(String & name) {
		auto search = attributes -> find(name);
		if (search != attributes -> end()) {
			return search -> second.second;
		}
		throw Environment::VariableNotFoundException();
	}
	Object * Instance::getInnerValue(String & name) {
		auto search = attributes -> find(name);
		if (search != attributes -> end()) {
			return (search -> second.second) -> copy();
		}
		throw Environment::VariableNotFoundException();
	}
	Object * Instance::getReference(String & name) {
		auto search = attributes -> find(name);
		if (search != attributes -> end()) {
			// Returns only if attribute is accessible (@public):
			if (search -> second.first == Modifier::publicAccess) {
				return search -> second.second;
			}
		}
		throw Environment::VariableNotFoundException();
	}
	Object * Instance::getValue(String & name) {
		auto search = attributes -> find(name);
		if (search != attributes -> end()) {
			// Returns only if attribute is accessible (not @hidden):
			if (search -> second.first != Modifier::hiddenAccess) {
				return (search -> second.second) -> copy();
			}
		}
		throw Environment::VariableNotFoundException();
	}
	String Instance::stringValue() const {
		return "<instance " + type -> name + ">";
	}
	Instance * Instance::copyByValue() const {
		Dictionary<String, Pair<Modifier, Object *>> * f = nullptr;
		f = new Dictionary<String, Pair<Modifier, Object *>>();
		for (Pair<String, Pair<Modifier, Object *>> p : * attributes) {
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
		return new Instance(type, attributes);
	}
	void Instance::destroy() {
		for (Pair<String, Pair<Modifier, Object *>> p : * attributes) {
			p.second.second -> safeDestroy();
		}
		delete attributes;
	}

}

#endif
