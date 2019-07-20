
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

}

#endif
