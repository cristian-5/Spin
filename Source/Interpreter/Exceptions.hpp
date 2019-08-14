
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

#ifndef STACKINTERPRETEREXCEPTIONS
#define STACKINTERPRETEREXCEPTIONS

#include "../Aliases/Aliases.hpp"

namespace Stack {

	class RunTimeUnaryOperatorException: public Exception {
		private:
		String op = "";
		String type = "";
		public: RunTimeUnaryOperatorException(String o, String t):
		Exception(), op(o), type(t) { }
		String getType() { return type; }
		String getOperator() { return op; }
	};

	class RunTimeBinaryOperatorException: public Exception {
		private:
		String op = "";
		String rs = "";
		String ls = "";
		public: RunTimeBinaryOperatorException(String o, String r, String l):
		Exception(), op(o), rs(r), ls(r) { }
		String getRSide() { return rs; }
		String getLSide() { return ls; }
		String getOperator() { return op; }
	};

}

#endif
