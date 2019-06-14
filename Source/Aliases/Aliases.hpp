
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
#include <string>
#include <fstream>
#include <regex>

using Exception = std::exception;

using IFStream = std::ifstream;
using OFStream = std::ofstream;

using UInt8 = std::uint8_t;
using Int8 = std::int8_t;
using UInt16 = std::uint16_t;
using Int16 = std::int16_t;
using UInt32 = std::uint32_t;
using Int32 = std::int32_t;
using UInt64 = std::uint64_t;
using Int64 = std::int64_t;

using String = std::string;

using SMatch = std::smatch;
using Regex = std::regex;
using RegexError = std::regex_error;

#define regexSearch std::regex_search
#define regexReplace std::regex_replace

#define subString substr
#define stringValue c_str

#define Character char
#define Double double
#define Float float
#define Real long double
#define Boolean bool

const Int32 exitSuccess = 0;
const Int32 exitFailure = 1;

#endif
