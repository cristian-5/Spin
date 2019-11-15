
/*!
 *
 *    + --------------------------------------- +
 *    |  Regex.hpp                              |
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
 */

#include "../Aliases/Prototypes.hpp"

#ifndef REGEXTOOLS
#define REGEXTOOLS

namespace Spin {

	Bool RegexTools::test(Regex & regex, const String & input) {
		try {
			SMatch match;
			regexSearch(input, match, regex);
			if (match.size() > 0) return true;
			else return false;
		} catch (RegexError & e) { }
		return false;
	}

	String RegexTools::match(Regex & regex, const String & input) {
		String result;
		try {
			SMatch match;
			regexSearch(input, match, regex);
			if (match.size() > 0) {
				result = match.str(0);
			} else return String();
		} catch (RegexError & e) {
			return String();
		}
		return result;
	}

	String RegexTools::findFirstGroup(Regex & regex, const String & input) {
		String result;
		try {
			SMatch match;
			regexSearch(input, match, regex);
			if (match.size() > 1) {
				result = match.str(1);
			} else return String();
		} catch (RegexError & e) {
			return String();
		}
		return result;
	}

	// TODO: Check this function out.... temporary push....
	Array<String> RegexTools::findAllGroups(Regex & regex, const String & input) {
		Array<String> result = Array<String>();
		try {
			SMatch match;
			regexSearch(input, match, regex);
			if (match.size() > 1) {
				for (SizeType i = 1; i < match.size(); i += 1) {
					String temp = String(match.str(i));
					result.push(temp);
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
