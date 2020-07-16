
/*!
 *
 *    + --------------------------------------- +
 *    |  Complex.cpp                            |
 *    |                                         |
 *    |               Complex Type              |
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

#include "Complex.hpp"

#ifndef SPIN_COMPLEX_CPP
#define SPIN_COMPLEX_CPP

namespace Spin {

	Complex::Complex(Real n, Real i) {
		a = n;
		b = i;
	}
	Complex Complex::fromPolar(Real m, Real a) {
		return Complex(m * cos(a), m * sin(a));
	}
	void Complex::setRealPart(Real val) {
		a = val;
	}
	void Complex::setImaginaryPart(Real val) {
		b = val;
	}
	Complex Complex::getConjugate() const {
		return Complex(a, -b);
	}
	void Complex::conjugate() {
		b = -b;
	}
	Real Complex::getNormalised() const {
		return a * a + b * b;
	}
	Real Complex::getMagnitude() const {
		return sqrt(a * a + b * b);
	}
	inline Real Complex::getModulus() const {
		return getMagnitude();
	}
	Real Complex::getPhase() const {
		return atan2(b, a);
	}
	inline Real Complex::getAngle() const {
		return getPhase();
	}
	void Complex::operator = (Real r) {
		a = r;
		b = 0;
	}
	Boolean Complex::operator == (Complex r) const {
		if (a != r.a) return false;
		if (b != r.b) return false;
		return true;
	}
	Boolean Complex::operator == (Real r) const {
		if (a != r) return false;
		if (b != 0) return false;
		return true;
	}
	Boolean Complex::operator != (Complex r) const {
		if (a == r.a) return false;
		if (b == r.b) return false;
		return true;
	}
	Boolean Complex::operator != (Real r) const {
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
		return Complex((a * r.a + b * r.b) / d,
					   (r.a * b - a * r.b) / d);
	}
	void Complex::operator /= (Complex r) {
		a = (a * r.a + b * r.b) / (r.a * r.a + r.b * r.b);
		b = (r.a * b - a * r.b) / (r.a * r.a + r.b * r.b);
	}
	String Complex::toString() const {
		String complex;
		if (isinf(a)) complex += (a < 0 ? "- infinity " : "infinity ");
		else if (isnan(a)) complex += "undefined ";
		else complex += (a < 0 ? "- " + std::to_string(- a) : std::to_string(a));
		complex.push_back(' ');
		if (isinf(b)) complex += (b < 0 ? "- (infinity)i" : "+ (infinity)i");
		else if (isnan(b)) complex += "+ (undefined)i";
		else complex += (b < 0 ? "- " + std::to_string(- b) : "+ " + std::to_string(b));
		complex.push_back('i');
		return complex;
	}

}

#endif
