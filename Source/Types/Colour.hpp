
/*!
 *
 *    + --------------------------------------- +
 *    |  Colour.hpp                             |
 *    |                                         |
 *    |               Colour Type               |
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

#ifndef STACKCOLOUR
#define STACKCOLOUR

#include "../Aliases/Aliases.hpp"

namespace Stack {
		
	class Colour {
			
		public:
			
		UInt8 r = 0;
		UInt8 g = 0;
		UInt8 b = 0;

		UInt8 a = 255;
		
		Colour(UInt8 r, UInt8 g, UInt8 b, UInt8 a = 255) {
			this -> r = r;
			this -> g = g;
			this -> b = b;
			this -> a = a;
		}

		Colour() { }

		/*void operator = (UInt32 c) {
			r = (UInt8) c >> 8;
		}

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
			return Complex((a * r.a + b * r.b) / (r.a * r.a + r.b * r.b),
						   (r.a * b - a * r.b) / (r.a * r.a + r.b * r.b));
		}

		void operator /= (Complex r) {
			a = (a * r.a + b * r.b) / (r.a * r.a + r.b * r.b);
			b = (r.a * b - a * r.b) / (r.a * r.a + r.b * r.b);
		}

		String stringValue() {
			String result = "";
			if (a < 0.0) {
				result += "- ";
				result += toString(- a);
			} else {
				result += "+ ";
				result += toString(a);
			}
			if (b >= 0.0) {
				result += " + ";
				result += toString(b);
			} else {
				result += " - ";
				result += toString(- b);
			}
			return result + "i";
		}*/

	};

}

#endif
