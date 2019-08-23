
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

		private:

		static String valueToHex(UInt8 value) {
			StringStream s = StringStream();
			s << uppercase << hexConversion;
			s << hexBytePadding << (UInt32) value;
			return s.str();
		}
			
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

		Colour(UInt32 rgba) {
			r = (UInt8)((rgba >> 24) & 0xFF);
			g = (UInt8)((rgba >> 16) & 0xFF);
			b = (UInt8)((rgba >> 8) & 0xFF);
			a = (UInt8)(rgba & 0xFF);
		}

		Colour() { }

		UInt32 colorValue() {
			return (UInt32)((UInt32)(r << 24) + 
							(UInt32)(g << 16) +
							(UInt32)(b << 16) +
							(UInt32)(a));
		}

		Colour operator ~ () const {
			return Colour(a - r, a - g, a - b, a);
		}

		Boolean operator == (Colour c) const {
			if (r != c.r) return false;
			if (g != c.g) return false;
			if (b != c.b) return false;
			if (a != c.a) return false;
			return true;
		}

		Boolean operator != (Colour c) const {
			return !((* this) == c);
		}


		Colour operator + (Colour r) const {
			Colour c = Colour(* this);
			// Hard Light
			return Colour();
		}

		Colour operator - (Colour r) const {
			Colour c = Colour(* this);
			// Difference
			return Colour();
		}

		Colour operator * (Colour r) const {
			Colour c = Colour(* this);
			// Multiply
			return Colour();
		}

		Colour operator / (Colour r) const {
			Colour c = Colour(* this);
			// Divide
			return Colour();
		}

		Colour operator % (Colour r) const {
			Colour c = Colour(* this);
			c.r = (c.r + r.r) / 2;
			c.g = (c.g + r.g) / 2;
			c.b = (c.b + r.b) / 2;
			c.a = (c.a + r.a) / 2;
			return c;
		}

		String stringValue() const {
			String result = "#";
			result += valueToHex(r) +
					  valueToHex(g) +
					  valueToHex(b) +
					  valueToHex(a);
			return result;
		}

	};

}

#endif
