
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
#include <math.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>

using Exception = std::exception;

using IFStream = std::ifstream;
using OFStream = std::ofstream;

using StringStream = std::stringstream;

using UInt8 = std::uint8_t;
using Int8 = std::int8_t;
using UInt16 = std::uint16_t;
using Int16 = std::int16_t;
using UInt32 = std::uint32_t;
using Int32 = std::int32_t;
using UInt64 = std::uint64_t;
using Int64 = std::int64_t;

using SizeType = std::size_t;

using String = std::string;

#define ArrayList std::vector

using SMatch = std::smatch;
using Regex = std::regex;
using RegexError = std::regex_error;

#define regexSearch std::regex_search
#define regexReplace std::regex_replace

#define reallocation std::realloc
#define allocation std::malloc

#define toString std::to_string
#define stringToLongDouble std::stold

#define subString substr
#define stringValue c_str
#define push push_back
#define pop pop_back

#define Character char
#define Double double
#define Float float
#define Real long double
#define Boolean bool

#define padding std::setw(4) << std::setfill('0')

inline void waitKeyPress() { std::cin.get(); }

const Int32 exitSuccess = 0;
const Int32 exitFailure = 1;

String getInput() {
	String input = "";
	getline(std::cin, input);
	return input;
}

#endif
