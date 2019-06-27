
/*!
 *
 *    + --------------------------------------- +
 *    |  ASTree.hpp                             |
 *    |                                         |
 *    |           Abstract Syntax Tree          |
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

#ifndef STACKASTREE
#define STACKASTREE

#include "../Aliases/Aliases.hpp"

#include "STable.hpp"

namespace Stack {

	class Expression {

		public:

		virtual ~Expression() = default;

		class Visitor {
			public:

		};

		virtual void accept(Visitor &) const { }

	};

	class Binary: public Expression {

		private:

		Expression * r = nullptr;
		Expression * l = nullptr;
		Token * o = nullptr;

		public:

		Binary(Expression * ls, Token * op, Expression * rs) {
			r = rs, l = ls; o = op;
		}

	};

}

#endif
