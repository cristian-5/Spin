
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

#ifndef SPINVECTOR
#define SPINVECTOR

namespace Spin {

	// bra * ket = inner product (Complex in result)
	// ket * bra = outer product (tensor product) (Matrix in result)
	// bra +- bra (same size) = bra (Vector in result)
	// ket +- ket (same size) = ket (Vector in result)
	// bra +- ket = error TODO: Better error handling in processor
	// ket +- bra = error

	Vector::Vector(SizeType s, Bool d) {
		size = s;
		if (size == 0) return;
		space = new Complex[size];
		direction = d;
	}
	Vector::Vector(Bool d) {
		direction = d;
	}
	Vector::~Vector() {
		if (size == 0) return;
		delete [] space;
	}
	Bool Vector::getDirection() const {
		return direction;
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
	void Vector::setDirection(Bool d) {
		direction = d;
	}
	inline void Vector::negate() {
		invert();
	}
	void Vector::invert() {
		for (SizeType i = 0; i < size; i += 1) {
			space[i] = - space[i];
		}
	}
	inline Vector * Vector::getInverse() const {
		return getAdditiveInverse();
	}
	Vector * Vector::getAdditiveInverse() const {
		Vector * result = new Vector(size, direction);
		for (SizeType i = 0; i < size; i += 1) {
			result -> operator[] (i) = - space[i];
		}
		return result;
	}
	void Vector::conjugate() {
		for (SizeType i = 0; i < size; i += 1) {
			space[i].conjugate();
		}
	}
	Vector * Vector::getConjugate() const {
		Vector * result = new Vector(size, direction);
		for (SizeType i = 0; i < size; i += 1) {
			result -> operator[] (i) = space[i].getConjugate();
		}
		return result;
	}
	void Vector::transpose() {
		direction = !direction;
	}
	Vector * Vector::getTransposed() const {
		Vector * result = copy();
		result -> transpose();
		return result;
	}
	inline void Vector::dagger() {
		conjugateTranspose();
	}
	inline void Vector::adjoint() {
		conjugateTranspose();
	}
	void Vector::conjugateTranspose() {
		direction = !direction;
		for (SizeType i = 0; i < size; i += 1) {
			space[i].conjugate();
		}
	}
	inline Vector * Vector::getDagger() const {
		return getConjugateTranspose();
	}
	inline Vector * Vector::getAdjoint() const {
		return getConjugateTranspose();
	}
	Vector * Vector::getConjugateTranspose() const {
		Vector * result = new Vector(size, !direction);
		for (SizeType i = 0; i < size; i += 1) {
			result -> operator[] (i) = space[i].getConjugate();
		}
		return result;
	}
	void Vector::inBraForm() {
		if (direction) return;
		conjugateTranspose();
	}
	void Vector::inKetForm() {
		if (!direction) return;
		conjugateTranspose();
	}
	Complex & Vector::operator [] (SizeType i) {
		if (i >= size) throw InvalidIndexException();
		return space[i];
	}
	Complex & Vector::at(SizeType i) {
		if (i >= size) throw InvalidIndexException();
		return space[i];
	}
	Bool Vector::operator == (Vector r) const {
		if (size != r.size ||
			direction != r.direction) {
			return false;
		}
		for (SizeType i = 0; i < size; i += 1) {
			if (r[i] != space[i]) return false;
		}
		return true;
	}
	Bool Vector::operator != (Vector r) const {
		if (size != r.size ||
			direction != r.direction) {
			return true;
		}
		for (SizeType i = 0; i < size; i += 1) {
			if (r[i] != space[i]) return true;
		}
		return false;
	}
	Vector Vector::operator + (Vector r) const {
		if (size != r.size || direction != r.direction) {
			throw InvalidOperationException();
		}
		Vector result = Vector(size, direction);
		for (SizeType i = 0; i < size; i += 1) {
			result[i] = r[i] + space[i];
		}
		return result;
	}
	Vector Vector::operator - () const {
		Vector result = Vector(size, direction);
		for (SizeType i = 0; i < size; i += 1) {
			result[i] = - result[i];
		}
		return result;
	}
	Vector Vector::operator - (Vector r) const {
		if (size != r.size || direction != r.direction) {
			throw InvalidOperationException();
		}
		Vector result = Vector(size, direction);
		for (SizeType i = 0; i < size; i += 1) {
			result[i] = r[i] - space[i];
		}
		return result;
	}
	Vector Vector::operator * (Complex z) const {
		Vector result = Vector(size, direction);
		for (SizeType i = 0; i < size; i += 1) {
			result[i] = space[i] * z;
		}
		return result;
	}
	void Vector::multiplyByScalarComplex(Complex & z) {
		for (SizeType i = 0; i < size; i += 1) {
			space[i] = space[i] * z;
		}
	}
	void Vector::multiplyByScalarImaginary(Real & i) {
		for (SizeType i = 0; i < size; i += 1) {
			space[i] = Complex(-i * space[i].b, i * space[i].a);
		}
	}
	void Vector::multiplyByScalarReal(Real & r) {
		for (SizeType i = 0; i < size; i += 1) {
			space[i] = Complex(space[i].a * r, space[i].b * r);
		}
	}
	void Vector::multiplyByScalarInteger(Int64 & i) {
		for (SizeType i = 0; i < size; i += 1) {
			space[i] = Complex(space[i].a * (Real)(i), space[i].b * (Real)(i));
		}
	}
	Vector * Vector::basis(Bool d, Bool s) {
		Vector * b = new Vector(2, d);
		if (s) b -> at(0) = Complex(1, 0);
		else b -> at(1) = Complex(1, 0);
		return b;
	}
	Vector * Vector::copy() const {
		Vector * result = new Vector(size, direction);
		for (SizeType i = 0; i < size; i += 1) {
			result -> operator[] (i) = space[i];
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
	String Vector::stringValue() const {
		if (size == 0) return direction ? "[ ]" : "[\n]";
		StringStream result;
		if (direction) {
			/* Print Bra */
			result << "[ ";
			for (SizeType i = 0; i < size - 1; i += 1) {
				result << space[i].stringValue() << ", ";
			}
			result << space[size - 1].stringValue() << " ]";
		} else {
			/* Print Ket */
			result << "[\n";
			for (SizeType i = 0; i < size - 1; i += 1) {
				result << "    " << space[i].stringValue() << ",\n";
			}
			result << "    " << space[size - 1].stringValue() << ",\n]";
		}
		return result.str();
	}

}

#endif
