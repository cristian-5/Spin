
/*!
 *
 *    + --------------------------------------- +
 *    |  Array.hpp                              |
 *    |                                         |
 *    |               Array Type                |
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

#ifndef STACKARRAY
#define STACKARRAY

namespace Stack {

	Array::Array() { elements = new ArrayList<Object *>(); }
	Array::Array(ArrayList<Object *> * e) { elements = e; }
	Array::Array(Object * o) {
		elements = new ArrayList<Object *>();
		elements -> push(o);
	}
	Array::~Array() {
		if (!elements) return;
		for (Object * o : * elements) delete o;
		delete elements;
	}
	Object * Array::copyAt(SizeType i) {
		if (i >= elements -> size()) return nullptr;
		return elements -> at(i) -> copy();
	}
	Object * Array::referenceAt(SizeType i) {
		if (i >= elements -> size()) return nullptr;
		return elements -> at(i);
	}
	Array * Array::copy() {
		ArrayList<Object *> * newElements = new ArrayList<Object *>();
		for (Object * element : * elements) {
			newElements -> push(element -> copy());
		}
		return new Array(newElements);
	}
	String Array::stringValue() {
		if (elements -> size() < 1) return "[ ]";
		String value = "[ " + elements -> at(0) -> getObjectStringValue();
		for (SizeType i = 1; i < elements -> size(); i++) {
			value += ", " + elements -> at(i) -> getObjectStringValue();
		}
		return value + " ]";
	}

}

#endif
