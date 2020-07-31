
/*!
 *
 *    + --------------------------------------- +
 *    |  Converter.cpp                          |
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
!*/

#include "Converter.hpp"

#ifndef SPIN_CONVERTER_CPP
#define SPIN_CONVERTER_CPP

#include <unordered_map>
#include <cmath>
#include <sstream>

#define ESCAPESEQUENCE "^'(?:[^\\\\]|\\\\0x[0-9A-Fa-f]{2}|\\\\['\\\\0abfnrtv])'$"
#define           REAL "^[0-9]+\\.[0-9]+(?:[eE][-]?[0-9]+)?$"
#define      IMAGINARY "^[0-9]+(?:\\.[0-9]+(?:[eE][-]?[0-9]+)?)?i$"
#define            HEX "^[A-Fa-f0-9]+$"

// IMPROVE: Benchmark the converter class and find a better way than
//          regexes for the base check. Ask Andrea to do that for u.

using StringStream = std::stringstream;

namespace Spin {

	inline Boolean Converter::checkBase(Regex base, String & s) {
		return RegexTools::test(base, s);
	}
	inline Boolean Converter::test(Regex r, String & s) {
		return RegexTools::test(r, s);
	}
	Boolean Converter::isHexChar(Character & c) {
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
	Int64 Converter::decToInteger(String & s) {
		if (s.length() == 0) return 0;
		if (!checkBase(Regex("^[0-9]+$"), s)) return 0;
		Int64 result = 0;
		for (SizeType i = 0; i < s.length(); i += 1) {
			result = result * 10 + s[i] - '0';
		}
		return result;
	}
	UInt8 Converter::charToHex(Character & c) {
		c = toupper(c);
		if (c >= '0' && c <= '9') return c - '0';
		if (c >= 'A' && c <= 'F') return c - 'A' + 0xA;
		return 0x00;
	}
	Int64 Converter::hexToInteger(String & s) {
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
	Int64 Converter::octToInteger(String & s) {
		if (s.length() == 0) return 0;
		if (!checkBase(Regex("^[0-7]+$"), s)) return 0;
		Int64 result = 0;
		for (SizeType i = 0; i < s.length(); i += 1) {
			result = result * 8 + s[i] - '0';
		}
		return result;
	}
	Int64 Converter::binToInteger(String & s) {
		if (s.length() == 0) return 0;
		if (!checkBase(Regex("^[01]+$"), s)) return 0;
		Int64 result = 0;
		for (SizeType i = 0; i < s.length(); i += 1) {
			result = result * 2 + s[i] - '0';
		}
		return result;
	}

	String Converter::typeToString(Type & t) {
		switch (t) {
			case Type::BooleanType: return "Boolean";
			case Type::CharacterType: return "Character";
			case Type::ByteType: return "Byte";
			case Type::IntegerType: return "Integer";
			case Type::RealType: return "Real";
			case Type::ImaginaryType: return "Imaginary";
			case Type::ComplexType: return "Complex";
			case Type::StringType: return "String";
			case Type::ArrayType: return "Array";
			case Type::VectorType: return "Vector";
			case Type::RoutineType: return "Routine";
			case Type::ClassType: return "Definition";
			case Type::InstanceType: return "Instance";
			case Type::VoidType: return "Void";
			default: return "Unknown";
		}
	}
	Type Converter::stringToType(String & s) {
		static Dictionary<String, Type> types = {
			{ "Boolean", Type::BooleanType },
			{ "Character", Type::CharacterType },
			{ "Byte", Type::ByteType },
			{ "Integer", Type::IntegerType },
			{ "Real", Type::RealType },
			{ "Imaginary", Type::ImaginaryType },
			{ "Complex", Type::ComplexType },
			{ "String", Type::StringType },
		};
		auto search = types.find(s);
		if (search == types.end()) return Type::VoidType;
		return search -> second;
	}
	Boolean Converter::stringToBoolean(String & s) {
		return s == "true";
	}
	Int64 Converter::stringToInteger(String & s) {
		if (s.length() == 0) return 0;
		if (s.length() > 2) {
			if (s[0] == '0') {
				switch (s[1]) {
					case 'x': {
						String hex = s.substr(2);
						return hexToInteger(hex);
					} break;
					case 'o': {
						String oct = s.substr(2);
						return octToInteger(oct);
					} break;
					case 'b': {
						String bin = s.substr(2);
						return binToInteger(bin);
					} break;
					case 'd': {
						String dec = s.substr(2);
						return decToInteger(dec);
					} break;
					default: return decToInteger(s);
				}
			}
		}
		return decToInteger(s);
	}
	Real Converter::stringToReal(String & s) {
		if (!test(Regex(REAL), s)) return 0.0;
		// IMPROVE: Create your own string to double:
		return std::stold(s);
	}
	Real Converter::stringToImaginary(String & s) {
		if (!test(Regex(IMAGINARY), s)) return 0.0;
		if (s.length() > 1) s.pop_back();
		else return 0.0;
		// IMPROVE: Create your own string to double:
		return std::stold(s);
	}
	String Converter::escapeString(String & s) {
		// OPTIMISE:
		if (s.find("\\") == String::npos) return s;
		StringStream result;
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
	String Converter::integerToString(Int64 & i) {
		// For negative numbers, print out the
		// absolute value with a leading '-'.
		// Use an array large enough for the
		// maximum number of digits + 1.
		static Character buffer[20];
		SizeType size = 20;
		Boolean negative = false;
		if (i < 0) {
			negative = true;
			i = - i;
			// When the value is Integer::minimum,
			// it overflows when made positive:
			if (i < 0) {
				size -= 1;
				buffer[size] = '0' + (-(i + 10) % 10);
				i = - (i / 10);
			}
		}
		do {
			size -= 1;
			buffer[size] = '0' + i % 10;
			i /= 10;
		} while (i > 0);
		if (negative) {
			size -= 1;
			buffer[size] = '-';
		}
		// Package constructor avoids an array copy:
		return String(buffer + size, 20 - size);
	}
	String Converter::imaginaryToString(Real a) {
		String imaginary;
		if (isinf(a)) imaginary += (a < 0 ? "- (infinity)i" : "(infinity)i");
		else if (isnan(a)) imaginary += "(undefined)i";
		else imaginary += (a < 0 ? "- " + std::to_string(- a) : std::to_string(a));
		imaginary.push_back('i');
		return imaginary;
	}
	String Converter::realToString(Real a) {
		String real;
		if (isinf(a)) real += (a < 0 ? "- infinity" : "infinity");
		else if (isnan(a)) real += "undefined";
		else real += (a < 0 ? "- " + std::to_string(- a) : std::to_string(a));
		return real;
	}

}

#endif
