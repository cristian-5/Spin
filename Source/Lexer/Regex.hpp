
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

	/*!
	*   @brief Tests a regex against a String.
	*   @param rgx Regex String.
	*   @param input Input String.
	*   @returns true if it matches the regex.
	*/
	Bool RegexTools::test(String rgx, String & input) {
		try {
			Regex regex(rgx);
			SMatch match;
			regexSearch(input, match, regex);
			if (match.size() > 0) return true;
			else return false;
		} catch (RegexError & e) { return false; }
		return false;
	}

	/*!
	*   @brief Matches a regex.
	*   @param rgx Regex String.
	*   @param input Input String.
	*   @returns The matched String.
	*/
	String RegexTools::match(String rgx, String & input) {
		String result = "";
		try {
			Regex regex(rgx);
			SMatch match;
			regexSearch(input, match, regex);
			if (match.size() > 0) {
				result = match.str(0);
			} else return "";
		} catch (RegexError & e) {
			return "";
		}
		return result;
	}

	/*!
	*   @brief Matches a regex and returns
	*   the first capture group.
	*   @param rgx Regex String.
	*   @param input Input String.
	*   @returns The first matched group.
	*/
	String RegexTools::matchClose(String rgx, String & input) {
		String result = "";
		try {
			Regex regex(rgx);
			SMatch match;
			regexSearch(input, match, regex);
			if (match.size() > 1) {
				result = match.str(1);
			} else return "";
		} catch (RegexError & e) {
			return "";
		}
		return result;
	}

	/*!
	*   @brief Matches a regex and returns
	*   all captures group in a list.
	*   @param rgx Regex String.
	*   @param input Input String.
	*   @returns The matched groups.
	*/
	Array<String> RegexTools::matchGroupClose(String rgx, String & input) {
		Array<String> result = Array<String>();
		try {
			Regex regex(rgx);
			SMatch match;
			regexSearch(input, match, regex);
			if (match.size() > 1) {
				for (Int32 i = 1; i < match.size(); i++) {
					String temp = String(match.str(i));
					result.push(temp);
				}
			} else return Array<String>();
		} catch (RegexError & e) {
			return Array<String>();
		}
		return result;
	}

	/*!
	*   @brief Matches a regex at the
	*   beginning of a String.
	*   @param rgx Regex String.
	*   @param input Input String.
	*   @returns The matched String.
	*/
	inline String RegexTools::matchStart(String rgx, String & input) {
		return match("^" + rgx, input);
	}

	/*!
	*   @brief Matches a regex at the
	*   beginning of a String and returns
	*   the first capture group.
	*   @param rgx Regex String.
	*   @param input Input String.
	*   @returns The first matched group.
	*/
	inline String RegexTools::matchCloseStart(String rgx, String & input) {
		return matchClose("^" + rgx, input);
	}

	/*!
	*   @brief Replaces every occurrence of the
	*   match with the given String.
	*   @param mtc Match String.
	*   @param input Input String.
	*   @param rpl Input Replace.
	*   @returns The replaced String.
	*/
	String RegexTools::replaceMatches(String mtc, String & input, String rpl) {
		if (mtc.length() == 0) return input;
		if (input.length() == 0) return input;
		Int32 position = input.find(mtc);
		while (position != String::npos) {
			input.replace(position, mtc.length(), rpl);
			position = input.find(mtc);
		}
		return input;
	}

};

#endif
