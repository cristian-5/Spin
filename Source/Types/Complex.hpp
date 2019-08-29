
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

#ifndef STACKCOMPLEX
#define STACKCOMPLEX

#include "../Aliases/Aliases.hpp"

namespace Stack {

	class ComplexDBZException: public Exception {
		public: ComplexDBZException(): Exception() { }
	};
	
	class Complex {
			
		public:
			
		Real a = 0.0;
		
		Real b = 0.0;
		
		Complex(Real n, Real i) {
			a = n; b = i;
		}

		Complex() { }
		
		static Complex fromPolar(Real m, Real a) {
			return Complex(m * cos(a), m * sin(a));
		}
		
		void setRealPart(Real val) { a = val; }
		void setImaginaryPart(Real val) { b = val; }
		
		Complex getConjugate() { return Complex(a, -b); }
		void conjugate() { b = -b; }
		
		Real getNormalized() { return a * a + b * b; }
		Real getMagnitude() { return sqrt(a * a + b * b); }
		inline Real getModulus() { return getMagnitude(); }
		Real getPhase() { return atan2(b, a); }
		inline Real getAngle() { return getPhase(); }

		void operator = (Real r) { a = r; b = 0; }

		Boolean operator == (Complex r) const {
			if (a != r.a) return false;
			if (b != r.b) return false;
			return true;
		}

		Boolean operator == (Real r) const {
			if (a != r) return false;
			if (b != 0) return false;
			return true;
		}

		Boolean operator != (Complex r) const {
			if (a == r.a) return false;
			if (b == r.b) return false;
			return true;
		}

		Boolean operator != (Real r) const {
			if (a == r) return false;
			if (b != 0) return false;
			return true;
		}

		Complex operator - () const {
			return Complex(- a, - b);
		}

		Complex operator + (Complex r) const {
			return Complex(a + r.a, b + r.b);
		}

		void operator += (Complex r) {
			a += r.a; b += r.b;
		}

		Complex operator - (Complex r) const {
			return Complex(a - r.a, b - r.b);
		}

		void operator -= (Complex r) {
			a -= r.a; b -= r.b;
		}

		Complex operator * (Complex r) const {
			return Complex(a * r.a - b * r.b, r.a * b + a * r.b);
		}

		void operator *= (Complex r) {
			a = a * r.a - b * r.b;
			b = r.a * b + a * r.b;
		}

		Complex operator / (Complex r) const {
			Real d = r.a * r.a + r.b * r.b;
			if (d == 0) throw ComplexDBZException();
			return Complex((a * r.a + b * r.b) / d,
						   (r.a * b - a * r.b) / d);
		}

		void operator /= (Complex r) {
			a = (a * r.a + b * r.b) / (r.a * r.a + r.b * r.b);
			b = (r.a * b - a * r.b) / (r.a * r.a + r.b * r.b);
		}

		String stringValue() const {
			return realToString(a) + " " +
				   realToString(b) + "i";
		}

	};

	Complex operator "" _i(Real i) {
		return Complex(0, (Real)i);
	}
	
	const Complex i = Complex(0, 1);
	const Complex nullComplex = Complex(0, 0);

}

#endif
