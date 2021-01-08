
#include "../Common/Header.hpp"

#ifndef SPIN_CONVERTER_HPP
#define SPIN_CONVERTER_HPP

#include "Regex.hpp"

#include "../Compiler/Program.hpp"

using Regex = std::regex;

namespace Spin {

	class Converter {
		private:
		static inline Boolean checkBase(Regex base, String & s);
		static inline Boolean test(Regex r, String & s);
		static Boolean isHexChar(Character & c);
		static Character hexToChar(String & s);
		static UInt64 decToNatural(String & s);
		static UInt8 charToHex(Character & c);
		static UInt64 hexToNatural(String & s);
		static UInt32 hexToUInt32(String & s);
		static UInt64 octToNatural(String & s);
		static UInt64 binToNatural(String & s);
		public:
		Converter() = delete;
		static String typeToString(Type & t);
		static Type stringToType(String & s);
		static Boolean stringToBoolean(String & s);
		static UInt64 stringToNatural(String & s);
		static Real stringToReal(String & s);
		static Real stringToImaginary(String & s);
		static String escapeString(String & s);
		static Character escapeChar(String & s);
		static String integerToString(Int64 & i);
		static String imaginaryToString(Real a);
		static String realToString(Real a);
	};

}

#endif
