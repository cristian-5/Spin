
/*!
 *
 *    + --------------------------------------- +
 *    |  Environment.hpp                        |
 *    |                                         |
 *    |            Memory Environment           |
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

#ifndef SPINENVIRONMENT
#define SPINENVIRONMENT

namespace Spin {

	Environment::Environment(Environment * enclosing) {
		this -> enclosing = enclosing;
	}
	Environment::~Environment() {
		for (const Pair<String, Object *> & value : values) {
			(value.second) -> safeDestroy();
		}
		collect();
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
		(search -> second) -> safeDestroy();
		values.erase(name);
	}
	void Environment::lose(Object * something) {
		if (!something) return;
		lostAndFound.push(something);
	}
	void Environment::collect() {
		for (Object * object : lostAndFound) {
			delete object;
		}
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
