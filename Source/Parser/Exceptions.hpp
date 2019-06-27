
/*!
 *
 *    + --------------------------------------- +
 *    |  Exceptions.hpp                         |
 *    |                                         |
 *    |            Stack Exceptions             |
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

#ifndef STACKEXCEPTIONS
#define STACKEXCEPTIONS

#include "../Aliases/Aliases.hpp"
#include "../Linker/FileHandler.hpp"

namespace Stack {

    /*!
	 *   @brief Syntax Error Exception.
	 *   Raised when a token is unexpected.
	 */
	class SyntaxErrorException: public Exception {
		private:
		String token = "";
		String expected = "";
		FilePosition pos = { 0, 0 };
		String fileName = "";
		public:
		String getToken() { return token; }
		String getExpected() { return expected; }
		FilePosition getPosition() { return pos; }
		String getFileName() { return fileName; }
		SyntaxErrorException(
			String t,
			String e,
			FilePosition position,
			String name
		):
		Exception(), token(t), expected(e),
		pos(position), fileName(name) { }
	};

    /*!
	 *   @brief Static Scope Exception.
	 *   Raised when a the scope is violated.
	 */
	class StaticScopeException: public Exception {
		private:
		String token = "";
		FilePosition pos = { 0, 0 };
		String fileName = "";
        String message = "";
		public:
		String getToken() { return token; }
		FilePosition getPosition() { return pos; }
		String getFileName() { return fileName; }
        String getMessage() { return message; }
		StaticScopeException(
			String t,
			String m,
			FilePosition position,
			String name
		):
		Exception(), token(t), message(m),
		pos(position), fileName(name) { }
	};

    /*!
	 *   @brief Static Cast Exception.
	 *   Raised when implicit casting fails.
	 */
	class StaticCastException: public Exception {
		private:
		FilePosition pos = { 0, 0 };
		String fileName = "";
        String message = "";
		public:
		FilePosition getPosition() { return pos; }
		String getFileName() { return fileName; }
        String getMessage() { return message; }
		StaticCastException(
			String m,
			FilePosition position,
			String name
		):
		Exception(), message(m),
		pos(position), fileName(name) { }
	};

}

#endif
