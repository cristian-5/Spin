
/*!
 *
 *    + --------------------------------------- +
 *    |  Aliases.hpp                            |
 *    |                                         |
 *    |               Code Aliases              |
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

#ifndef SPIN_ALIASES
#define SPIN_ALIASES

	#include <iostream>
	#include <string>

	using Byte = std::uint_fast8_t;
	using UInt8 = std::uint_fast8_t;
	using Int8 = std::int_fast8_t;
	using UInt16 = std::uint_fast16_t;
	using Int16 = std::int_fast16_t;
	using UInt32 = std::uint_fast32_t;
	using Int32 = std::int_fast32_t;
	using UInt64 = std::uint_fast64_t;
	using Int64 = std::int_fast64_t;

	using Hash = std::uint_fast64_t;

	using SizeType = std::size_t;

	using String = std::string;

	using Boolean = bool;
	using Character = char;
	using Real = double;

	#define Array std::vector
	#define Set std::set
	#define Dictionary std::unordered_map

	#define InitialiserList std::initializer_list

	#define Pair std::pair

	#define Lambda std::function

	#define DynamicCast dynamic_cast

#endif
