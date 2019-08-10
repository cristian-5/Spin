
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

	class RunTimeUnaryOperandException: public Exception {
		private:
		String op = "";
		String type = "";
		public: RunTimeUnaryOperandException(String o, String t):
		Exception(), op(o), type(t) { }
		String getType() { return type; }
		String getOperand() { return op; }
	};

}

#endif
