
#include "../Aliases.hpp"

#ifndef SPIN_CONVERTER_HPP
#define SPIN_CONVERTER_HPP

#include <cmath>
#include <sstream>
#include <regex>

using Regex = std::regex;

using StringStream = std::stringstream;

namespace Spin {

	class Converter {
		private:
		static inline Bool checkBase(Regex base, String & s);
		static inline Bool test(Regex r, String & s);
		static Bool isHexChar(Character & c);
		static Character hexToChar(String & s);
		static Int64 decToInt64(String & s);
		static UInt8 charToHex(Character & c);
		static Int64 hexToInt64(String & s);
		static UInt32 hexToUInt32(String & s);
		static Int64 octToInt64(String & s);
		static Int64 binToInt64(String & s);
		public:
		Converter() = delete;
		static Bool stringToBool(String & s);
		static Int64 stringToInt64(String & s);
		static Real stringToReal(String & s);
		static Real stringToImaginary(String & s);
		static String escapeString(String & s);
		static Character escapeChar(String & s);
		static String integerToString(Int64 & i);
		// IMPROVE: please, get rid of this:
		static String realToGroupedString(Real a);
		static String imaginaryToGroupedString(Real a);
		static String realToString(Real a);
	};

}

#endif
