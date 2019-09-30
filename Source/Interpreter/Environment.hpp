
/*!
 *
 *    + --------------------------------------- +
 *    |  Environment.hpp                        |
 *    |                                         |
 *    |            Stack Environment            |
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

#ifndef STACKENVIRONMENT
#define STACKENVIRONMENT

namespace Stack {

	Environment::Environment(Environment * enclosing) {
		this -> enclosing = enclosing;
	}
	Environment::~Environment() {
		for (Pair<String, Object *> value : values) {
			delete value.second;
		}
	}
	void Environment::define(String name, Object * value) {
		auto search = values.find(name);
		if (search != values.end()) {
			throw VariableRedefinitionException(
				(search -> second) -> getObjectName()
			);
		} else values.insert({ name, value });
	}
	void Environment::forget(String name) {
		auto search = values.find(name);
		if (search == values.end()) {
			throw VariableNotFoundException();
		}
		values.erase(name);
	}
	Object * Environment::getReference(String name) {
		auto search = values.find(name);
		if (search != values.end()) {
			return search -> second;
		}
		if (enclosing) return enclosing -> getReference(name);
		throw VariableNotFoundException();
	}
	Object * Environment::getValue(String name) {
		auto search = values.find(name);
		if (search != values.end()) {
			return (search -> second) -> copy();
		}
		if (enclosing) return enclosing -> getValue(name);
		throw VariableNotFoundException();
	}

}

#endif
