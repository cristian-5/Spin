
#include "../Common/Header.hpp"

#ifndef SPIN_REGEX_HPP
#define SPIN_REGEX_HPP

#include <regex>
#include <vector>

using Regex = std::regex;
using SMatch = std::smatch;
using RegexError = std::regex_error;

namespace Spin {

	class RegexTools {
		public:
		RegexTools() = delete;
		static Boolean test(Regex & regex, const String & input);
		static String match(Regex & regex, const String & input);
		static String findFirstGroup(Regex & regex, const String & input);
		static Array<String> findAllGroups(Regex & regex, const String & input);
		static void replaceMatches(const String & mtc, String & input, const String & rpl);
	};
	
}

#endif
