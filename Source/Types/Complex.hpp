
/*!
 *
 *    + --------------------------------------- +
 *    |  Complex.hpp                            |
 *    |                                         |
 *    |             Complex Numbers             |
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

#ifndef STACKCOMPLEX
#define STACKCOMPLEX

namespace Stack {

	Complex::Complex(Real n, Real i) {
		a = n; b = i;
	}
	Complex Complex::fromPolar(Real m, Real a) {
		return Complex(m * cos(a), m * sin(a));
	}
	void Complex::setRealPart(Real val) { a = val; }
	void Complex::setImaginaryPart(Real val) { b = val; }
	Complex Complex::getConjugate() { return Complex(a, -b); }
	void Complex::conjugate() { b = -b; }
	Real Complex::getNormalized() { return a * a + b * b; }
	Real Complex::getMagnitude() { return sqrt(a * a + b * b); }
	inline Real Complex::getModulus() { return getMagnitude(); }
	Real Complex::getPhase() { return atan2(b, a); }
	inline Real Complex::getAngle() { return getPhase(); }
	void Complex::operator = (Real r) { a = r; b = 0; }
	Bool Complex::operator == (Complex r) const {
		if (a != r.a) return false;
		if (b != r.b) return false;
		return true;
	}
	Bool Complex::operator == (Real r) const {
		if (a != r) return false;
		if (b != 0) return false;
		return true;
	}
	Bool Complex::operator != (Complex r) const {
		if (a == r.a) return false;
		if (b == r.b) return false;
		return true;
	}
	Bool Complex::operator != (Real r) const {
		if (a == r) return false;
		if (b != 0) return false;
		return true;
	}
	Complex Complex::operator - () const {
		return Complex(- a, - b);
	}
	Complex Complex::operator + (Complex r) const {
		return Complex(a + r.a, b + r.b);
	}
	void Complex::operator += (Complex r) {
		a += r.a; b += r.b;
	}
	Complex Complex::operator - (Complex r) const {
		return Complex(a - r.a, b - r.b);
	}
	void Complex::operator -= (Complex r) {
		a -= r.a; b -= r.b;
	}
	Complex Complex::operator * (Complex r) const {
		return Complex(a * r.a - b * r.b, r.a * b + a * r.b);
	}
	void Complex::operator *= (Complex r) {
		a = a * r.a - b * r.b;
		b = r.a * b + a * r.b;
	}
	Complex Complex::operator / (Complex r) const {
		Real d = r.a * r.a + r.b * r.b;
		if (d == 0) throw ComplexDBZException();
		return Complex((a * r.a + b * r.b) / d,
						(r.a * b - a * r.b) / d);
	}
	void Complex::operator /= (Complex r) {
		a = (a * r.a + b * r.b) / (r.a * r.a + r.b * r.b);
		b = (r.a * b - a * r.b) / (r.a * r.a + r.b * r.b);
	}
	String Complex::stringValue() const {
		return realToGroupedString(a) +
				imaginaryToGroupedString(b);
	}

	Complex operator "" _i(Real i) {
		return Complex(0, (Real)i);
	}
	
	const Complex i = Complex(0, 1);
	const Complex nullComplex = Complex(0, 0);

}

#endif
