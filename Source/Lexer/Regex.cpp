
/*!
 *
 *    + --------------------------------------- +
 *    |  Regex.cpp                              |
 *    |                                         |
 *    |               Regex Tools               |
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

#include "../Aliases/Prototypes/Regex.hpp"

#ifndef SPIN_REGEX
#define SPIN_REGEX

namespace Spin {

	Boolean RegexTools::test(Regex & regex, const String & input) {
		try {
			SMatch match;
			std::regex_search(input, match, regex);
			if (match.size() > 0) return true;
			else return false;
		} catch (RegexError & e) {
			return false;
		}
	}
	String RegexTools::match(Regex & regex, const String & input) {
		try {
			SMatch match;
			std::regex_search(input, match, regex);
			if (match.size() > 0) {
				return match.str(0);
			} else return String();
		} catch (RegexError & e) {
			return String();
		}
	}
	String RegexTools::findFirstGroup(Regex & regex, const String & input) {
		String result;
		try {
			SMatch match;
			std::regex_search(input, match, regex);
			if (match.size() > 1) {
				result = match.str(1);
			} else return String();
		} catch (RegexError & e) {
			return String();
		}
		return result;
	}
	Array<String> RegexTools::findAllGroups(Regex & regex, const String & input) {
		Array<String> result = Array<String>();
		try {
			SMatch match;
			std::regex_search(input, match, regex);
			if (match.size() > 1) {
				for (SizeType i = 1; i < match.size(); i += 1) {
					result.push_back(match.str(i));
				}
			} else return Array<String>();
		} catch (RegexError & e) {
			return Array<String>();
		}
		return result;
	}
	void RegexTools::replaceMatches(const String & mtc, String & input, const String & rpl) {
		if (mtc.empty() || input.empty()) return;
		SizeType position = input.find(mtc);
		while (position != String::npos) {
			input.replace(position, mtc.length(), rpl);
			position = input.find(mtc);
		}
	}

};

#endif
