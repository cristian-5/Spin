
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
#include "../Types/Object.hpp"

#define ESCAPESEQUENCE "^'(?:[^\\\\]|\\\\0x[0-9A-Fa-f]{2}|\\\\['\\\\0abfnrtv])'$"
#define REAL "^[0-9]+\\.[0-9]+(?:[eE][0-9]+)?$"
#define IMAGINARY "^[0-9]+(?:\\.[0-9]+(?:[eE][0-9]+)?)?i$"
#define RGBFULL "#[A-Fa-f0-9]{6}"
#define RGBSHORT "#[A-Fa-f0-9]{3}"
#define RGBAFULL "#[A-Fa-f0-9]{8}"
#define RGBASHORT "#[A-Fa-f0-9]{4}"

namespace Stack {

	class Converter {

		private:

		Converter() { }

		static BasicType shortestForm(UInt64 n) {
			if (n <= 0xFF) return BasicType::UInt8Type;
			else if (n <= 0xFFFF) return BasicType::UInt16Type;
			else if (n <= 0xFFFFFFFF) return BasicType::UInt32Type;
			else return BasicType::UInt64Type;
		}

		static Boolean checkBase(String base, String & s) {
			return RegexTools::test(base, s);
		}

		static Character hexToChar(String & s) {
			if (s.length() == 0) return 0x00;
			if (!checkBase("^[0-9A-Fa-f]+$", s)) return 0x00;
			Character result = 0;
    		for (UInt32 i = 0; i < s.length(); i++) {
				result = result * 16 + charToHex(s[i]);
			}
			return result;
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

		static UInt8 charToHex(Character & c) {
			c = toUppercase(c);
			if (c >= '0' && c <= '9') return c - '0';
			if (c >= 'A' && c <= 'F') return c - 'A';
			return 0;
		}

		static UInt64 hexToUInt64(String & s) {
			if (s.length() == 0) return 0;
			if (!checkBase("^[0-9A-Fa-f]+$", s)) return 0;
			UInt64 result = 0;
    		for (UInt32 i = 0; i < s.length(); i++) {
				result = result * 16 + charToHex(s[i]);
			}
			return result;
		}

		static UInt32 hexToUInt32(String & s) {
			if (s.length() == 0) return 0;
			if (!checkBase("^[0-9A-Fa-f]+$", s)) return 0;
			UInt32 result = 0;
    		for (UInt32 i = 0; i < s.length(); i++) {
				result = result * 16 + charToHex(s[i]);
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

		static Object * literalToObject(Token * t) {
			Object * o = new Object();
			if (t == nullptr) return o;
			if (!t -> isTypeLiteral()) return o;
			switch (t -> type) {
				case TokenType::intLiteral: {
					UInt64 v = stringToUInt64(t -> lexeme);
					o -> type = shortestForm(v);
					switch (o -> type) {
						case BasicType::UInt8Type: {
							UInt8 * i = new UInt8;
							* i = (UInt8) v;
							o -> value = i;
						} break;
						case BasicType::UInt16Type: {
							UInt16 * i = new UInt16;
							* i = (UInt16) v;
							o -> value = i;
						} break;
						case BasicType::UInt32Type: {
							UInt32 * i = new UInt32;
							* i = (UInt32) v;
							o -> value = i;
						} break;
						case BasicType::UInt64Type: {
							UInt64 * i = new UInt64;
							* i = (UInt64) v;
							o -> value = i;
						} break;
						default: break;
					}
				} break;
				case TokenType::stringLiteral: {
					o -> type = BasicType::StringType;
					String * v = new String;
					t -> lexeme = t -> lexeme.subString(1,
								  t -> lexeme.length() - 2);
					* v = escapeString(t -> lexeme);
					o -> value = v;
				} break;
				case TokenType::boolLiteral: {
					o -> type = BasicType::BooleanType;
					Boolean * v = new Boolean;
					* v = stringToBool(t -> lexeme);
					o -> value = v;
				} break;
				case TokenType::charLiteral: {
					o -> type = BasicType::CharacterType;
					Character * v = new Character;
					t -> lexeme = t -> lexeme.subString(1,
								  t -> lexeme.length() - 2);
					* v = escapeChar(t -> lexeme);
					o -> value = v;
				} break;
				case TokenType::realLiteral: {
					o -> type = BasicType::RealType;
					Real * v = new Real;
					* v = stringToReal(t -> lexeme);
					o -> value = v;
				} break;
				case TokenType::imaginaryLiteral: {
					o -> type = BasicType::ImaginaryType;
					Real * v = new Real;
					* v = stringToImaginary(t -> lexeme);
					o -> value = v;
				} break;
				case TokenType::nullLiteral: {
					o -> type = BasicType::ClassType;
					o -> value = nullptr;
				} break;
				case TokenType::colourLiteral: {
					o -> type = BasicType::ColourType;
					Colour * c = new Colour(stringToColour(t -> lexeme));
					o -> value = c;
				} break;
				default: return o;
			}
			return o;
		}

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
						default: return decToUInt64(s);
					}
				}
			}
			return decToUInt64(s);
		}

		static Real stringToReal(String & s) {
			if (!RegexTools::test(REAL, s)) return 0.0;
			return stringToLongDouble(s);
		}

		static Real stringToImaginary(String & s) {
			if (!RegexTools::test(IMAGINARY, s)) return 0.0;
			if (s.length() > 1) s.pop();
			return stringToLongDouble(s);
		}

		static String escapeString(String & s) {
			s = RegexTools::replaceMatches("\\\"", s, "\"");
			s = RegexTools::replaceMatches("\\\\", s, "\\");
			return s; // TODO: Properly Escape.
		}

		static Colour stringToColour(String & s) {
			if (s.length() > 2) s = s.substr(1, s.size() - 2);
			if (s.length() == 0) return Colour();
			if (RegexTools::test(RGBFULL, s)) {
				String x = s + "FF";
				UInt32 c = hexToUInt32(x);
				return Colour(c);
			} else if (RegexTools::test(RGBSHORT, s)) {
				StringStream x = StringStream();
				x << s[0] << s[0] << s[1] << s[1] <<
					 s[2] << s[2] << "FF";
				String final = x.str();
				UInt32 c = hexToUInt32(final);
				return Colour(c);
			} else if (RegexTools::test(RGBAFULL, s)) {
				UInt32 c = hexToUInt32(s);
				return Colour(c);
			} else if (RegexTools::test(RGBASHORT, s)) {
				StringStream x = StringStream();
				x << s[0] << s[0] << s[1] << s[1] <<
					 s[2] << s[2] << s[3] << s[3];
				String final = x.str();
				UInt32 c = hexToUInt32(final);
				return Colour(c);
			}
			return Colour();
		}

		static Character escapeChar(String & s) {
			if (s.length() == 0) return 0x00;
			if (RegexTools::test(ESCAPESEQUENCE, s)) {
				if (s == "\\\\") return '\\';
				if (s == "\\0") return 0x00;
				if (s.length() < 2) return 0x00;
				if (s[0] == '\\') {
					switch (s[1]) {
						case '0': {
							if (s.length() != 5) return 0x00;
							if (s[2] != 'x') return 0x00;
							String hex = "";
							hex += s[3]; hex += s[4];
							return hexToChar(hex);
						} break;
						case 'a': return '\a';
						case 'b': return '\b';
						case 'f': return '\f';
						case 'n': return '\n';
						case 'r': return '\r';
						case 't': return '\t';
						case 'v': return '\v';
						default: return 0x00;
					}
				}
				return 0x00;
			}
			return s[0];
		}

	};

}

#endif
