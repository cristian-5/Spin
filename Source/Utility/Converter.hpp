
#include "../Common/Header.hpp"

#ifndef SPIN_CONVERTER_HPP
#define SPIN_CONVERTER_HPP

#include "Regex.hpp"

#include "../Compiler/Program.hpp"

using Regex = std::regex;
using StringStream = std::stringstream;

namespace Spin {

	class Converter {
		private:
		static inline Boolean checkBase(Regex base, String & s);
		static inline Boolean test(Regex r, String & s);
		static Boolean isHexChar(Character & c);
		static Character hexToChar(String & s);
		static Int64 decToInteger(String & s);
		static UInt8 charToHex(Character & c);
		static Int64 hexToInteger(String & s);
		static UInt32 hexToUInt32(String & s);
		static Int64 octToInteger(String & s);
		static Int64 binToInteger(String & s);
		public:
		Converter() = delete;
		static String typeToString(Type & t);
		static Type stringToType(String & s);
		static Boolean stringToBoolean(String & s);
		static Int64 stringToInteger(String & s);
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
