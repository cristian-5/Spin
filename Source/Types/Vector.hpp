
/*!
 *
 *    + --------------------------------------- +
 *    |  Vector.hpp                             |
 *    |                                         |
 *    |               Vector Type               |
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

#ifndef STACKVECTOR
#define STACKVECTOR

namespace Stack {

	Vector::Vector(SizeType s) {
		size = s;
		if (size == 0) return;
		space = new Complex[size];
	}
	Vector::~Vector() {
		if (size == 0) return;
		delete [] space;
	}
	SizeType Vector::getSize() const {
		return size;
	}
	Bool Vector::isEmpty() const {
		return size == 0;
	}
	Bool Vector::isBra() const {
		return direction == braDirection;
	}
	Bool Vector::isKet() const {
		return direction == ketDirection;
	}
	inline void Vector::negate() {
		invert();
	}
	void Vector::invert() {
		for (SizeType i = 0; i < size; i++) {
			space[i] = - space[i];
		}
	}
	inline Vector * Vector::getInverse() const {
		return getAdditiveInverse();
	}
	Vector * Vector::getAdditiveInverse() const {
		Vector * result = new Vector(size);
		for (SizeType i = 0; i < size; i++) {
			result -> operator[] (i) = - space[i];
		}
		return result;
	}
	Complex & Vector::operator [] (SizeType i) {
		if (i >= size) throw InvalidIndexException();
		return space[i];
	}
	Bool Vector::operator == (Vector r) const {
		if (size != r.size) return false;
		for (SizeType i = 0; i < size; i++) {
			if (r[i] != space[i]) return false;
		}
		return true;
	}
	Bool Vector::operator != (Vector r) const {
		if (size != r.size) return true;
		for (SizeType i = 0; i < size; i++) {
			if (r[i] != space[i]) return true;
		}
		return false;
	}
	Vector Vector::operator + (Vector r) const {
		if (size != r.size) throw InvalidOperationException();
		Vector result = Vector(size);
		for (SizeType i = 0; i < size; i++) {
			result[i] = r[i] + space[i];
		}
		return result;
	}
	Vector Vector::operator - () const {
		Vector result = Vector(size);
		for (SizeType i = 0; i < size; i++) {
			result[i] = - result[i];
		}
		return result;
	}
	Vector Vector::operator - (Vector r) const {
		if (size != r.size) throw InvalidOperationException();
		Vector result = Vector(size);
		for (SizeType i = 0; i < size; i++) {
			result[i] = r[i] - space[i];
		}
		return result;
	}
	Vector Vector::operator * (Complex z) const {
		Vector result = Vector(size);
		for (SizeType i = 0; i < size; i++) {
			result[i] = space[i] * z;
		}
		return result;
	}
	inline Vector * Vector::kroneckerProduct(Vector * a, Vector * b) {
		return tensorProduct(a, b);
	}
	Vector * Vector::tensorProduct(Vector * a, Vector * b) {
		SizeType s1 = a -> getSize();
		SizeType s2 = b -> getSize();
		Vector * result = new Vector(s1 * s2);
		for (SizeType i = 0; i < s1; i++) {
			for (SizeType j = 0; j < s2; j++) {
				Complex c1 = a -> operator[] (i);
				Complex c2 = b -> operator[] (j);
				result -> operator[] ((i * s2) + j) = c1 * c2;
			}
		}
		return result;
	}
	Complex * Vector::copyAt(SizeType i) const {
		if (i >= size) throw InvalidIndexException();
		return new Complex(space[i]);
	}
	Complex * Vector::referenceAt(SizeType i) const {
		if (i >= size) throw InvalidIndexException();
		return & space[i];
	}


}

#endif
