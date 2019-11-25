
/*!
 *
 *    + --------------------------------------- +
 *    |  Converter.hpp                          |
 *    |                                         |
 *    |             Object Converter            |
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

#ifndef SPINCONVERTER
#define SPINCONVERTER

#define ESCAPESEQUENCE "^'(?:[^\\\\]|\\\\0x[0-9A-Fa-f]{2}|\\\\['\\\\0abfnrtv])'$"
#define           REAL "^[0-9]+\\.[0-9]+(?:[eE][-]?[0-9]+)?$"
#define      IMAGINARY "^[0-9]+(?:\\.[0-9]+(?:[eE][-]?[0-9]+)?)?i$"
#define            HEX "^[A-Fa-f0-9]+$"
#define        RGBFULL "^[A-Fa-f0-9]{6}$"
#define       RGBSHORT "^[A-Fa-f0-9]{3}$"
#define       RGBAFULL "^[A-Fa-f0-9]{8}$"
#define      RGBASHORT "^[A-Fa-f0-9]{4}$"

namespace Spin {

	inline Bool Converter::checkBase(Regex base, String & s) {
		return RegexTools::test(base, s);
	}
	inline Bool Converter::test(Regex r, String & s) {
		return RegexTools::test(r, s);
	}
	Bool Converter::isHexChar(Character & c) {
		return ((c >= '0') && (c <= '9')) ||
				((c >= 'a') && (c <= 'f')) ||
				((c >= 'A') && (c <= 'F'));
	}
	Character Converter::hexToChar(String & s) {
		if (s.length() == 0) return 0x00;
		if (!checkBase(Regex(HEX), s)) return 0x00;
		Character result = 0;
		for (SizeType i = 0; i < s.length(); i += 1) {
			result = result * 16 + charToHex(s[i]);
		}
		return result;
	}
	Int64 Converter::decToInt64(String & s) {
		if (s.length() == 0) return 0;
		if (!checkBase(Regex("^[0-9]+$"), s)) return 0;
		Int64 result = 0;
		for (SizeType i = 0; i < s.length(); i += 1) {
			result = result * 10 + s[i] - '0';
		}
		return result;
	}
	UInt8 Converter::charToHex(Character & c) {
		c = toUppercase(c);
		if (c >= '0' && c <= '9') return c - '0';
		if (c >= 'A' && c <= 'F') return c - 'A' + 0xA;
		return 0x00;
	}
	Int64 Converter::hexToInt64(String & s) {
		if (s.length() == 0) return 0;
		if (!checkBase(Regex(HEX), s)) return 0;
		Int64 result = 0;
		for (SizeType i = 0; i < s.length(); i += 1) {
			result = result * 16 + charToHex(s[i]);
		}
		return result;
	}
	UInt32 Converter::hexToUInt32(String & s) {
		if (s.length() == 0) return 0;
		if (!checkBase(Regex(HEX), s)) return 0;
		UInt32 result = 0;
		for (SizeType i = 0; i < s.length(); i += 1) {
			result = result * 16 + charToHex(s[i]);
		}
		return result;
	}
	Int64 Converter::octToInt64(String & s) {
		if (s.length() == 0) return 0;
		if (!checkBase(Regex("^[0-7]+$"), s)) return 0;
		Int64 result = 0;
		for (SizeType i = 0; i < s.length(); i += 1) {
			result = result * 8 + s[i] - '0';
		}
		return result;
	}
	Int64 Converter::binToInt64(String & s) {
		if (s.length() == 0) return 0;
		if (!checkBase(Regex("^[01]+$"), s)) return 0;
		Int64 result = 0;
		for (SizeType i = 0; i < s.length(); i += 1) {
			result = result * 2 + s[i] - '0';
		}
		return result;
	}
	Object * Converter::literalToObject(Token * t) {
		Object * o = new Object();
		if (!t) return o;
		if (!t -> isTypeLiteral()) return o;
		switch (t -> type) {
			case TokenType::intLiteral: {
				o -> type = BasicType::Int64Type;
				o -> value = new Int64(stringToInt64(t -> lexeme));
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
				o -> type = BasicType::BoolType;
				o -> value = new Bool(stringToBool(t -> lexeme));
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
				o -> value = new Real(stringToReal(t -> lexeme));
			} break;
			case TokenType::imaginaryLiteral: {
				o -> type = BasicType::ImaginaryType;
				o -> value = new Real(stringToImaginary(t -> lexeme));
			} break;
			case TokenType::emptyLiteral: {
				o -> type = BasicType::ClassType;
				o -> value = nullptr;
			} break;
			case TokenType::colourLiteral: {
				o -> type = BasicType::ColourType;
				o -> value = new Colour(stringToColour(t -> lexeme));
			} break;
			case TokenType::basisBraLiteral: {
				o -> type = BasicType::VectorType;
				Bool state = (t -> lexeme)[1] == '0' ? 0 : 1;
				o -> value = Vector::basis(Vector::braDirection, state);
			} break;
			case TokenType::basisKetLiteral: {
				o -> type = BasicType::VectorType;
				Bool state = (t -> lexeme)[1] == '0' ? 0 : 1;
				o -> value = Vector::basis(Vector::ketDirection, state);
			} break;
			default: return o;
		}
		return o;
	}
	Bool Converter::stringToBool(String & s) {
		return s == "true";
	}
	Int64 Converter::stringToInt64(String & s) {
		if (s.length() == 0) return 0;
		if (s.length() > 2) {
			if (s[0] == '0') {
				switch (s[1]) {
					case 'x': {
						String hex = s.subString(2);
						return hexToInt64(hex);
					} break;
					case 'o': {
						String oct = s.subString(2);
						return octToInt64(oct);
					} break;
					case 'b': {
						String bin = s.subString(2);
						return binToInt64(bin);
					} break;
					case 'd': {
						String dec = s.subString(2);
						return decToInt64(dec);
					} break;
					default: return decToInt64(s);
				}
			}
		}
		return decToInt64(s);
	}
	Real Converter::stringToReal(String & s) {
		if (!test(Regex(REAL), s)) return 0.0;
		return stringToLongDouble(s);
	}
	Real Converter::stringToImaginary(String & s) {
		if (!test(Regex(IMAGINARY), s)) return 0.0;
		if (s.length() > 1) s.pop();
		else return 0.0;
		return stringToLongDouble(s);
	}
	String Converter::escapeString(String & s) {
		// OPTIMISE:
		if (s.find("\\") == String::npos) return s;
		StringStream result = StringStream();
		const SizeType length = s.length();
		SizeType i = 0;
		while (i < length) {
			Character c = s[i];
			i += 1;
			if (c == '\\' && (i < length)) {
				c = s[i]; i += 1;
				switch (c) {
					case 'a': c = '\a'; break;
					case 'b': c = '\b'; break;
					case 'f': c = '\f'; break;
					case 'n': c = '\n'; break;
					case 'r': c = '\r'; break;
					case 't': c = '\t'; break;
					case 'v': c = '\v'; break;
					case '\\': c = '\\'; break;
					case '\'': c = '\''; break;
					case '"': c = '"'; break;
					case '0': {
						if (i >= length) { c = '?'; break; }
						c = s[i]; i += 1;
						if (c == 'x' && (i < length)
							&& isHexChar(s[i])) {
							UInt8 a = charToHex(s[i]);
							i += 1;
							if ((i < length) && isHexChar(s[i])) {
								a = a << 4;
								a = a | charToHex(s[i]);
								i += 1; c = a; break;
							}
						}
						c = '?';
					} break;
					default: c = '?'; break;
				}
			}
			result << c;
		}
		return result.str();
	}
	Colour Converter::stringToColour(String & s) {
		// OPTIMISE:
		if (s.length() > 3) s = s.substr(1, s.size() - 1);
		if (s.length() < 3) return Colour();
		if (test(Regex(RGBFULL), s)) {
			s += "FF";
			return Colour(hexToUInt32(s));
		} else if (test(Regex(RGBSHORT), s)) {
			StringStream x = StringStream();
			x << s[0] << s[0] << s[1] << s[1] <<
					s[2] << s[2] << "FF";
			String final = x.str();
			return Colour(hexToUInt32(final));
		} else if (test(Regex(RGBAFULL), s)) {
			return Colour(hexToUInt32(s));
		} else if (test(Regex(RGBASHORT), s)) {
			StringStream x = StringStream();
			x << s[0] << s[0] << s[1] << s[1] <<
					s[2] << s[2] << s[3] << s[3];
			String final = x.str();
			return Colour(hexToUInt32(final));
		}
		return Colour();
	}
	Character Converter::escapeChar(String & s) {
		// OPTIMISE:
		if (s.length() == 0) return 0x00;
		if (test(Regex(ESCAPESEQUENCE), s)) {
			if (s == "\\\\") return '\\';
			if (s == "\\0") return 0x00;
			if (s.length() < 2) return 0x00;
			if (s[0] == '\\') {
				switch (s[1]) {
					case '0': {
						if (s.length() != 5) return 0x00;
						if (s[2] != 'x') return 0x00;
						String hex;
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
	BasicType Converter::typeFromString(String & s) {
		if (s == "Integer") return BasicType::Int64Type;
		if (s == "Real") return BasicType::RealType;
		if (s == "String") return BasicType::StringType;
		if (s == "Imaginary") return BasicType::ImaginaryType;
		if (s == "Complex") return BasicType::ComplexType;
		if (s == "Bool") return BasicType::BoolType;
		if (s == "Character") return BasicType::CharacterType;
		if (s == "Byte") return BasicType::ByteType;
		if (s == "Colour") return BasicType::ColourType;
		return BasicType::UnknownType;
	}

}

#endif
