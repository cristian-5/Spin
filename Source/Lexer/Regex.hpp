
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

#ifndef REGEXTOOLS
#define REGEXTOOLS

#include "../Aliases/Aliases.hpp"
#include "../Collection/StrongList.hpp"

using Collection::StrongList;

/*!
 *   @brief Namespace RegexTools.
 *   Contains classes designed for
 *   handling regexes and strings.
 *   @author Cristian A.
 */
namespace RegexTools {

	/*!
	 *   @brief Tests a regex against a String.
	 *   @param rgx Regex String.
	 *   @param input Input String.
	 *   @returns true if it matches the regex.
	 */
	Boolean test(String rgx, String & input) {
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
	String match(String rgx, String & input) {
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
	String matchClose(String rgx, String & input) {
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
	StrongList<String> matchGroupClose(String rgx, String & input) {
		StrongList<String> result = StrongList<String>();
		try {
			Regex regex(rgx);
			SMatch match;
			regexSearch(input, match, regex);
			if (match.size() > 1) {
				for (Int32 i = 1; i < match.size(); i++) {
					String temp = String(match.str(i));
					result.link(temp);
				}
			} else return StrongList<String>();
		} catch (RegexError & e) {
			return StrongList<String>();
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
	inline String matchStart(String rgx, String & input) {
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
	inline String matchCloseStart(String rgx, String & input) {
		return matchClose("^" + rgx, input);
	}

	/*!
	 *   @brief Replaces every occurrence of the
	 *   match with the given String.
	 *   @param match Match String.
	 *   @param input Input String.
	 *   @param replace Input Replace.
	 *   @returns The replaced String.
	 */
	String replaceMatches(String & match,
						  String & input,
						  String & replace) {
		if (match.length() == 0) return input;
		if (input.length() == 0) return input;
		Int32 position = input.find(match);
		while (position != String::npos) {
			input.replace(position, match.length(), replace);
			position = input.find(match);
		}
		return input;
	}

};

#endif
