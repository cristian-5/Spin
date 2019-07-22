
/*!
 *
 *    + --------------------------------------- +
 *    |  Interpreter.hpp                        |
 *    |                                         |
 *    |            Stack Interpreter            |
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

#ifndef STACKINTERPRETER
#define STACKINTERPRETER

#include "../Aliases/Aliases.hpp"
#include "../Parser/ASTree.hpp"

#include "Object.hpp"

namespace Stack {

	class Interpreter: public Expression::Visitor {

		private:

		Object literalToObject(Token * t) {
			Object o = Object();
			if (t == nullptr) return o;
			if (!t -> isTypeLiteral()) return o;
			switch (t -> type) {
				case TokenType::boolLiteral: {
					o.type = BasicType::BooleanType;
					Boolean * v = new Boolean;
					* v = t -> lexeme == "true";
					o.value = v;
				} break;
				case TokenType::intLiteral: {
					o.type = BasicType::Int64Type;
					UInt64 * v = new UInt64;
					* v = 0;
					// TODO: Various Conversions based on base
					o.value = v;
				} break;
				default: return o;
			}

		}


		void visitLiteralExpression(Literal * e) override {
			
		}

		public:

		Interpreter() { }



	};

}

#endif
