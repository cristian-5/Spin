
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

#ifndef STACKPARSEREXCEPTIONS
#define STACKPARSEREXCEPTIONS

#include "../Aliases/Aliases.hpp"
#include "../Linker/Linker.hpp"

namespace Stack {

    /*!
	 *   @brief Syntax Error Exception.
	 *   Raised when a token is unexpected.
	 */
	class SyntaxErrorException: public Exception {
		private:
		String token = "";
		String expected = "";
		Linker::FilePosition pos = { 0, 0 };
		String fileName = "";
		public:
		String getToken() { return token; }
		String getExpected() { return expected; }
		Linker::FilePosition getPosition() { return pos; }
		String getFileName() { return fileName; }
		SyntaxErrorException(
			String t,
			String e,
			Linker::FilePosition position,
			String name
		):
		Exception(), token(t), expected(e),
		pos(position), fileName(name) { }
	};

	/*!
	 *   @brief Unexpected Expression Exception.
	 *   Raised when the syntax tree ends unexpectedly.
	 */
	class InvalidExpressionException: public Exception {
		private:
		String token = "";
		Linker::FilePosition pos = { 0, 0 };
		String fileName = "";
		public:
		String getToken() { return token; }
		Linker::FilePosition getPosition() { return pos; }
		String getFileName() { return fileName; }
		InvalidExpressionException(
			String t,
			Linker::FilePosition position,
			String name
		):
		Exception(), token(t),
		pos(position), fileName(name) { }
	};

	/*!
	 *   @brief Unexpected End Exception.
	 *   Raised when the code ends unexpectedly.
	 */
	class UnexpectedEndException: public Exception {
		private:
		String token = "";
		Linker::FilePosition pos = { 0, 0 };
		String fileName = "";
		public:
		String getToken() { return token; }
		Linker::FilePosition getPosition() { return pos; }
		String getFileName() { return fileName; }
		UnexpectedEndException(
			String t,
			Linker::FilePosition position,
			String name
		):
		Exception(), token(t),
		pos(position), fileName(name) { }
	};

	/*!
	 *   @brief Empty Unit Exception.
	 *   Raised when the code unit is empty.
	 */
	class EmptyUnitException: public Exception {
		private: String fileName = "";
		public: EmptyUnitException(String name):
		Exception(), fileName(name) { }
		String getFileName() { return fileName; }
	};

}

#endif
