
#include "../Common/Header.hpp"

#ifndef SPIN_COMPLEX_HPP
#define SPIN_COMPLEX_HPP

#include <cmath>

namespace Spin {

	class Complex {
		public:
		Real a = 0.0;
		Real b = 0.0;
		Complex(Real n, Real i);
		Complex() = default;
		static Complex fromPolar(Real m, Real a);
		void setRealPart(Real val);
		void setImaginaryPart(Real val);
		Complex getConjugate() const;
		void conjugate();
		Real getNormalised() const;
		Real getMagnitude() const;
		inline Real getModulus() const;
		Real getPhase() const;
		inline Real getAngle() const;
		void operator = (Real r);
		Boolean operator == (Complex r) const;
		Boolean operator == (Real r) const;
		Boolean operator != (Complex r) const;
		Boolean operator != (Real r) const;
		Complex operator - () const;
		Complex operator + (Complex r) const;
		void operator += (Complex r);
		Complex operator - (Complex r) const;
		void operator -= (Complex r);
		Complex operator * (Complex r) const;
		void operator *= (Complex r);
		Complex operator / (Complex r) const;
		void operator /= (Complex r);
		String toString() const;
	};

}

#endif
