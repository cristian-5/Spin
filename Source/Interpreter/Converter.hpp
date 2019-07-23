
/*!
 *
 *    + --------------------------------------- +
 *    |  Converter.hpp                          |
 *    |                                         |
 *    |             Stack Converter             |
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

#ifndef STACKCONVERTER
#define STACKCONVERTER

#include "../Aliases/Aliases.hpp"
#include "../Lexer/Regex.hpp"

#include "Object.hpp"

namespace Stack {

	class Converter {

		private:

		Converter() { }

		static Boolean checkBase(String base, String & s) {
			return RegexTools::test(base, s);
		}

		static UInt64 decToUInt64(String & s) {
			if (s.length() == 0) return 0;
			if (!checkBase("^[0-9]+$", s)) return 0;
			UInt64 result = 0;
    		for (UInt32 i = 0; i < s.length(); i++) {
				result = result * 10 + s[i] - '0';
			}
			return result;
		}

		static UInt64 hexToUInt64(String & s) {
			if (s.length() == 0) return 0;
			if (!checkBase("^[0-9A-Fa-f]+$", s)) return 0;
			UInt64 result = 0;
    		for (UInt32 i = 0; i < s.length(); i++) {
				result = result * 16 + s[i] - '0';
			}
			return result;
		}

		static UInt64 octToUInt64(String & s) {
			if (s.length() == 0) return 0;
			if (!checkBase("^[0-7]+$", s)) return 0;
			UInt64 result = 0;
    		for (UInt32 i = 0; i < s.length(); i++) {
				result = result * 8 + s[i] - '0';
			}
			return result;
		}

		static UInt64 binToUInt64(String & s) {
			if (s.length() == 0) return 0;
			if (!checkBase("^[01]+$", s)) return 0;
			UInt64 result = 0;
    		for (UInt32 i = 0; i < s.length(); i++) {
				result = result * 2 + s[i] - '0';
			}
			return result;
		}

		public:

		static Boolean stringToBool(String & s) {
			return s == "true";
		}

		static UInt64 stringToUInt64(String & s) {
			if (s.length() == 0) return 0;
			if (s.length() > 2) {
				if (s[0] == '0') {
					switch (s[1]) {
						case 'x': {
							String hex = s.subString(2, s.length() - 3);
							return hexToUInt64(hex);
						} break;
						case 'o': {
							String oct = s.subString(2, s.length() - 3);
							return octToUInt64(oct);
						} break;
						case 'b': {
							String bin = s.subString(2, s.length() - 3);
							return binToUInt64(bin);
						} break;
						case 'd': {
							String dec = s.subString(2, s.length() - 3);
							return decToUInt64(dec);
						} break;
						default: return decToUInt64(s); break;
					}
				}
			}
			return decToUInt64(s);
		}

		static String escapeString(String & s) {
			return s; // TODO: Properly Escape.
		}

		static Character escapeChar(String & s) {
			return s[0]; // TODO: Properly Escape.
		}

	};

}

#endif
