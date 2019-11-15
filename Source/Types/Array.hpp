
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

#ifndef SPINARRAYLIST
#define SPINARRAYLIST

namespace Spin {

	ArrayList::ArrayList() { elements = new Array<Object *>(); }
	ArrayList::ArrayList(Array<Object *> * e) { elements = e; }
	ArrayList::ArrayList(Object * o) {
		elements = new Array<Object *>();
		elements -> push(o);
	}
	ArrayList::~ArrayList() {
		if (!elements) return;
		for (Object * o : * elements) delete o;
		delete elements;
	}
	Object * ArrayList::copyAt(SizeType i) const {
		if (i >= elements -> size()) return nullptr;
		return elements -> at(i) -> copy();
	}
	Object * ArrayList::referenceAt(SizeType i) const {
		if (i >= elements -> size()) return nullptr;
		return elements -> at(i);
	}
	ArrayList * ArrayList::copy() const {
		Array<Object *> * newElements = new Array<Object *>();
		for (Object * element : * elements) {
			newElements -> push(element -> copy());
		}
		return new ArrayList(newElements);
	}
	String ArrayList::stringValue() const {
		if (elements -> size() < 1) return "[ ]";
		String value = "[ " + elements -> at(0) -> getObjectStringValue();
		for (SizeType i = 1; i < elements -> size(); i += 1) {
			value += ", " + elements -> at(i) -> getObjectStringValue();
		}
		return value + " ]";
	}

}

#endif
