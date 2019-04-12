
/*!
 *
 *    + --------------------------------------- +
 *    |  Compiler.h                             |
 *    |                                         |
 *    |                Compiler                 |
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

#ifndef STACKCOMPILER
#define STACKCOMPILER

#include <iostream>
#include <string>
#include <regex>

#include "HeapCollection.h"

using namespace std;

using namespace HeapCollection;

/*! @brief Namespace Stack */
namespace StackCompiler {

	/*! @brief Compiler Class. */
	class Compiler {

	private:

		/* MARK: - Regexes */

		/*!
		 *   @brief Matches a regex.
		 *   @param rgx Regex string.
		 *   @param input Input string.
		 *   @returns The matched string.
		 */
		string match(string rgx, string input) {
			string result;
			try {
				regex regex(rgx);
				smatch match;
				regex_search(input, match, regex);
				if (match.size() > 0) {
					result = match.str(0);
				} else return "";
			} catch (regex_error & e) {
				return "";
			}
			return result;
		}

		/*!
		 *   @brief Matches a regex and returns
		 *   the first capture group.
		 *   @param rgx Regex string.
		 *   @param input Input string.
		 *   @returns The first matched group.
		 */
		string matchClose(string rgx, string input) {
			string result;
			try {
				regex regex(rgx);
				smatch match;
				regex_search(input, match, regex);
				if (match.size() > 1) {
					result = match.str(1);
				} else return "";
			} catch (regex_error & e) {
				return "";
			}
			return result;
		}

		/*!
		 *   @brief Matches a regex and returns
		 *   all captures group in a list.
		 *   @param rgx Regex string.
		 *   @param input Input string.
		 *   @returns The matched groups.
		 */
		StrongList<string> matchGroupClose(string rgx, string input) {
			StrongList<string> result;
			try {
				regex regex(rgx);
				smatch match;
				regex_search(input, match, regex);
				if (match.size() > 1) {
					for (int i = 1; i < match.size(); i++) {
						result.link(match.str(i));
					}
				} else return StrongList<string>();
			} catch (regex_error & e) {
				return StrongList<string>();
			}
			return result;
		}

		/*!
		 *   @brief Matches a regex at the
		 *   beginning of a string.
		 *   @param rgx Regex string.
		 *   @param input Input string.
		 *   @returns The matched string.
		 */
		string matchStart(string rgx, string input) {
			return match("^" + rgx, input);
		}

		/*!
		 *   @brief Matches a regex at the
		 *   beginning of a string and returns
		 *   the first capture group.
		 *   @param rgx Regex string.
		 *   @param input Input string.
		 *   @returns The first matched group.
		 */
		string matchCloseStart(string rgx, string input) {
			return matchClose("^" + rgx, input);
		}

		/*!
		 *   @brief Replaces every occurrence of the
		 *   matched regex with the given string.
		 *   @param rgx Regex string.
		 *   @param input Input string.
		 *   @param replace Input Replace.
		 *   @returns The replaced string.
		 */
		string replaceMatches(string rgx, string input, string replace) {
			regex regex(rgx);
			string result = input;
			while (regex_search(result, regex)) {
				result = regex_replace(result, regex, replace);
			}
			return result;
		}

	public:

		static string process(string & code) {

			return code;
		}

	};



}


#endif
