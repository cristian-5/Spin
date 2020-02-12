
/*!
 *
 *    + --------------------------------------- +
 *    |  Conventions.hpp                        |
 *    |                                         |
 *    |             Code Convetions             |
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

#ifndef ALIASES
#define ALIASES

#include <iostream>
#include <iomanip>
#include <functional>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <unordered_map>
#include <chrono>

#define Input std::cin
#define Output std::cout
#define endLine std::endl

using IFStream = std::ifstream;
using OFStream = std::ofstream;

using StringStream = std::stringstream;

using UInt8 = std::uint_fast8_t;
using Int8 = std::int_fast8_t;
using UInt16 = std::uint_fast16_t;
using Int16 = std::int_fast16_t;
using UInt32 = std::uint_fast32_t;
using Int32 = std::int_fast32_t;
using UInt64 = std::uint_fast64_t;
using Int64 = std::int_fast64_t;

using SizeType = std::size_t;

using String = std::string;
using WString = std::wstring;

#define Array std::vector
#define Dictionary std::unordered_map

#define Pair std::pair

#define Lambda std::function

using SMatch = std::smatch;
using Regex = std::regex;
using RegexError = std::regex_error;

#define regexSearch std::regex_search
#define regexReplace std::regex_replace

#define reallocation std::realloc
#define allocation std::malloc

#define stringToLongDouble std::stold

#define subString substr
#define stringValue c_str
#define push push_back
#define pop pop_back
#define shrinkToFit shrink_to_fit

#define toUppercase toupper

#define Character char
#define Real double
#define Bool bool

#define hexConversion std::hex

#define padding std::setw(4) << std::setfill('0')
#define hexBytePadding std::setw(2) << std::setfill('0')
#define uppercase std::uppercase

#define DynamicCast dynamic_cast

inline String getInput() {
	String input;
	getline(Input, input);
	return input;
}

inline void waitKeyPress() { std::cin.get(); }

const Int32 exitSuccess = 0;
const Int32 exitFailure = 1;

namespace Spin {

	static String realToGroupedString(Real a) {
		StringStream result;
		result << (a >= 0 ? "+ " : "- ");
		Int64 i = (Int64) a;
		if (i < 0) { i = -i; a = -a; }
		result << i;
		a -= (Real)i;
		a *= 100;
		i = (Int64) a;
		if (i == 0) result << ".0";
		else result << '.' << i;
		if (i < 10) result << '0';
		return result.str();
	}

	static inline String realToString(Real a) {
		return realToGroupedString(a);
	}

	static String imaginaryToGroupedString(Real a) {
		StringStream result;
		result << (a >= 0 ? "+ " : "- ");
		Int64 i = (Int64) a;
		if (i < 0) { i = -i; a = -a; }
		result << i;
		a -= (Real)i;
		a *= 100;
		i = (Int64) a;
		if (i < 10) result << '.' << i << '0';
		else result << '.' << i;
		result << 'i'; return result.str();
	}

	static Real abs(Real x) {
		if (x < 0) return - x;
		else return x;
	}

}

#endif
